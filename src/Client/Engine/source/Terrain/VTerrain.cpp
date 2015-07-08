

#include "VTerrain.h"
#include "VTerrainSerializer.h"
#include "VisionXMLParser.h"
#include "VisionXMLWriter.h"
#include "VTerrainInfo.h"
#include "VTerrainTile.h"
#include "VEngine.h"
#include "VGraphicsSystem.h"


const VUINT32 TERRAIN_HEIGHTMAP_MAGIC = 'FEHM';
const VUINT32 TERRAIN_HEIGHTMAP_VERSION = 0x00100000;

const VUINT32 TERRAIN_LIGHTMAP_MAGIC = 'FELM';
const VUINT32 TERRAIN_LIGHTMAP_VERSION = 0x00100002;

const VUINT32 TERRAIN_GRIDINFO_MAGIC = 'FEGI';
const VUINT32 TERRAIN_GRIDINFO_VERSION = 0x00100003;

const VUINT32 TERRAIN_TEXTURE_ATLASE_W = TERRAIN_ATLASE_IMAGE_SIZE * TERRAIN_ATLASE_COL;
const VUINT32 TERRAIN_TEXTURE_ATLASE_H = TERRAIN_ATLASE_IMAGE_SIZE * TERRAIN_ATLASE_ROW;

const VINT32 TERRAIN_TEXTURE_MAX_MIPMAP = 3;

/**
 * 高度图文件头信息
 */
struct HeightMapHeader
{
	VDWORD magic;	// 文件标识
	VDWORD version; // 文件版本号
	VINT32 width;	// 高度图宽
	VINT32 depth;	// 高度图深度
};

/**
 * 网格文件头信息
 */
struct GridHeader
{
	VDWORD magic;	// 文件标识
	VDWORD version; // 文件版本号
	VINT32 width;	// 高度图宽
	VINT32 depth;	// 高度图深度
};

/**
 * 阴影图文件头信息
 */
struct LightmapHeader
{
	VDWORD magic;	// 文件标识
	VDWORD version; // 文件版本号
	VINT32 width;	// 高度图宽
	VINT32 depth;	// 高度图深度
};


VTerrain::VTerrain()
	: mSceneMgr(VNULL)
	, mTerrainNode(VNULL)
	, mInfo(VNULL)
	, mHeightmap()
	, mGridInfos()
	, mLightmapImage(VNULL)
	, mLayer(VNULL)
	, mLightmap(VNULL)
	, mMaterial(VNULL)
	, mTiles()
{
	mInfo = new VTerrainInfo();

	mSceneMgr = Ogre::Root::getSingleton().getSceneManager("VisionSceneManager");
	assert(mSceneMgr);
}

VTerrain::~VTerrain()
{
	delete mInfo;
	mInfo = VNULL;

	delete mLightmapImage;
	mLightmapImage = VNULL;

	_destroyTiles();
	_destroyMaterials();
	_destroyTextures();

	if (mTerrainNode)
	{
		mSceneMgr->destroySceneNode(mTerrainNode);
		mTerrainNode = VNULL;
	}
}


void VTerrain::load(XMLParser *parser, const VString &filename, const VString &groupName, VBOOL hasLightmap)
{
	_reset();

	mGroupName = groupName;

	// 加载基本地形信息
	_loadTerrainInfo(parser, filename, groupName);

	// 加载高度图信息
	_loadHeightmap(mInfo->mHeightmapFilename, mInfo->mHeightmapType, groupName);

	// 加载网格信息
	_loadGridInfo(mInfo->mGridInfoFilename, groupName);

	// 加载阴影图信息
	if (hasLightmap)
	{
		_loadLightmap(mInfo->mLightmapFilename, mInfo->mLightmapType, groupName);
	}

	// 设置地形起点偏移位置
	mOffset.x = -mInfo->mXSize * mInfo->mScale.x / 2;
	mOffset.y = 0;
	mOffset.z = -mInfo->mZSize * mInfo->mScale.z / 2;
}

void VTerrain::save(XMLWriter *writer, const VString &filename) const
{

}

void VTerrain::create()
{
	assert(mSceneMgr && mInfo);

	VGraphicsSystem *pGfxSystem = VENGINE.getGfxSystem();

	Ogre::String nodeName = mInfo->mName + "/Terrain";
	if (mSceneMgr->hasSceneNode(nodeName))
		mTerrainNode = mSceneMgr->getSceneNode(nodeName);
	else
		mTerrainNode = pGfxSystem->getBaseSceneNode()->createChildSceneNode(nodeName);

	_createTextures();
	_createMaterials();
	_createTiles();
}

void VTerrain::destroy()
{
	_destroyTiles();
	_destroyMaterials();
	_destroyTextures();

	if (mTerrainNode)
	{
		mSceneMgr->destroySceneNode(mTerrainNode);
		mTerrainNode = VNULL;
	}
}

void VTerrain::_loadTerrainInfo(XMLParser *parser, const VString &filename, const VString groupName)
{
	assert(parser);

	if (filename.empty())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"Filename supplied for terrain loading must be valid",
			"VTerrain::_loadTerrainInfo");
	}

	//--------------------------------------------------------------------

	// setup handler object
	VTerrainXMLHandler handler(mInfo);

	// parse the terrain XML file
	parser->parseXMLFile(handler, filename, groupName);
}

void VTerrain::_saveTerrainInfo(XMLWriter *writer, const VString &filename) const
{

}

void VTerrain::_loadHeightmap(const VString &filename, const VString &type, const VString &groupName)
{
	assert(mInfo);
	mHeightmap.resize((mInfo->mXSize + 1) * (mInfo->mZSize + 1));

	if (filename.empty())
	{
		std::fill(mHeightmap.begin(), mHeightmap.end(), (Ogre::Real)0);
	}
	else if (type == "image")
	{
		// 图像格式

		Ogre::Image image;
		image.load(filename, groupName);

		Ogre::PixelFormat pf = image.getFormat();
		if (pf != Ogre::PF_L8 && pf != Ogre::PF_L16)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Error: Image is not a grayscale image.",
				"VTerrain::_loadHeightmap" );
		}

		if (image.getWidth() != mInfo->mXSize + 1 
			|| image.getHeight() != mInfo->mZSize + 1 
			|| image.getDepth() != 1)
		{
			Ogre::String err = "Error: Invalid heightmap size : " +
				Ogre::StringConverter::toString(image.getWidth()) + "x" +
				Ogre::StringConverter::toString(image.getHeight()) + "x" +
				Ogre::StringConverter::toString(image.getDepth()) +
				". Should be " +
				Ogre::StringConverter::toString(mInfo->mXSize+1) + "x" +
				Ogre::StringConverter::toString(mInfo->mZSize+1);
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, err,
				"VTerrain::_loadHeightmap");
		}

		const void *src = image.getData();
		VBOOL is16bit = (pf == Ogre::PF_L16);
		Ogre::Real *dst = &mHeightmap[0];
		VINT32 width = mInfo->mXSize+1;
		VINT32 depth = mInfo->mZSize+1;
		VBOOL flipTerrain = VFALSE;

		if (is16bit)
		{
			// 16位灰度图，转成浮点数存储
			const Ogre::Real invScale = 1.0f / 65535.0f;
			const VWORD *src16 = reinterpret_cast<const VWORD*>(src);
			if (flipTerrain)
			{
				src16 += depth * (width + 1);
			}

			for (VINT32 j = 0; j <= depth; ++j)
			{
				for (VINT32 i = 0; i <= width; ++i)
				{
					*dst++ = Ogre::Real(*src16++) * invScale;
				}

				if (flipTerrain)
				{
					src16 -= 2 * (width + 1);
				}
			}
		}
		else
		{
			// 8位灰度图，转成浮点数存储
			const Ogre::Real invScale = 1.0f / 255.0f;
			const VBYTE *src8 = reinterpret_cast<const VBYTE*>(src);
			if (flipTerrain)
			{
				src8 += depth * (width + 1);
			}

			for (VINT32 j = 0; j <= depth; ++j)
			{
				for (VINT32 i = 0; i <= width; ++i)
				{
					*dst++ = Ogre::Real(*src8++) * invScale;
				}

				if (flipTerrain)
				{
					src8 -= 2 * (width - 1);
				}
			}
		}
	}
	else if (type == "standard")
	{
		// 标准格式
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);
		
		// 读取文件头
		HeightMapHeader header;
		stream->read(&header, sizeof(header));

		if (header.magic != TERRAIN_HEIGHTMAP_MAGIC)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid magic of heightmap file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadHeightmap");
		}

		if (header.version != TERRAIN_HEIGHTMAP_VERSION)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid version of heightmap file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadHeightmap");
		}

		if (header.width != mInfo->mXSize + 1 || header.depth != mInfo->mZSize + 1)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Dimensions of heightmap file '" + filename + "' doesn't match the dimensions of the terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadHeightmap");
		}

		stream->read(&mHeightmap[0], mHeightmap.size()*sizeof(VFLOAT));

		if (stream->tell() != stream->size())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid heightmap file '" + filename + "'.",
				"VTerrain::_loadHeightmap");
		}
	}
	else
	{
		// 原始数据格式
		assert(type == "raw");

		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);
		stream->read(&mHeightmap[0], mHeightmap.size());

		if (stream->tell() != stream->size())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid heightmap file '" + filename + "'.",
				"VTerrain::_loadHeightmap");
		}
	}
}

void VTerrain::_saveHeightmap(const VString &filename) const
{
	
}

void VTerrain::_loadGridInfo(const VString &filename, const VString &groupName)
{
	assert(mInfo);
	mGridInfos.resize(mInfo->mXSize * mInfo->mZSize);

	if (filename.empty())
	{
		GridInfo gridInfo = {0, 0, 0, 0, 0};
		if (!mInfo->mPixmaps.empty())
		{
			gridInfo.layers[0].pixmapID = 1;
		}
		std::fill(mGridInfos.begin(), mGridInfos.end(), gridInfo);
	}
	else
	{
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);

		GridHeader header;
		stream->read(&header, sizeof(header));

		if (header.magic != TERRAIN_GRIDINFO_MAGIC)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid magic of grid info file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadGridInfo");
		}
		if (header.version > TERRAIN_GRIDINFO_VERSION)
		{
			Ogre::StringUtil::StrStreamType str;
			str << "A newest version (" << std::hex << header.version << ")"
				<< " of grid info file '" << filename << "'"
				<< " found while loading terrain '" << mInfo->mName << "'." << std::endl
				<< "Current support version is (" << std::hex << TERRAIN_GRIDINFO_VERSION << ")." << std::endl
				<< "You should run lastest version software to support this version grid info file.";
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				str.str(),
				"VTerrain::_loadGridInfo");
		}
		if (header.version < 0x00100000)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid version of grid info file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadGridInfo");
		}
		if (header.version != TERRAIN_GRIDINFO_VERSION)
		{
			Ogre::StringUtil::StrStreamType str;
			str << "WARNING: " << filename
				<< " is an older format version (" << std::hex << header.version << "),"
				<< " current version (" << std::hex << TERRAIN_GRIDINFO_VERSION << ");"
				<< " you should upgrade it as soon as possible when you re-save the file.";
			Ogre::LogManager::getSingleton().logMessage(str.str());
		}
		if (header.width != mInfo->mXSize || header.depth != mInfo->mZSize)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Dimensions of grid info file '" + filename + "' doesn't match the dimensions of the terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadGridInfo");
		}

		VBOOL pixmapId16Bits = VFALSE;
		if (header.version >= 0x00100002)
		{
			stream->read(&pixmapId16Bits, sizeof(VBOOL));
		}

		VINT32 numPixmaps = mInfo->mPixmaps.size();
		for (GridInfoArray::iterator itr = mGridInfos.begin(); itr != mGridInfos.end(); ++itr)
		{
			GridInfo &gridInfo = *itr;
			for (VINT32 i = 0; i < NumLayers; ++i)
			{
				if (pixmapId16Bits)
				{
					VWORD pixmapID;
					VBYTE orientation;
					stream->read(&pixmapID, sizeof(pixmapID));
					stream->read(&orientation, sizeof(orientation));
					gridInfo.layers[i].pixmapID = pixmapID;
					gridInfo.layers[i].orientation = orientation;
				}
				else
				{
					VBYTE pixmapID;
					VBYTE orientation;
					stream->read(&pixmapID, sizeof(pixmapID));
					stream->read(&orientation, sizeof(orientation));
					gridInfo.layers[i].pixmapID = pixmapID;
					gridInfo.layers[i].orientation = orientation;
				}

				if (gridInfo.layers[i].pixmapID > numPixmaps)
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
						"Invalid pixmapId in grid info '" + filename + "'.",
						"VTerrain::_loadGridInfo");
				}
			}

			gridInfo.flags = 0;
			if (header.version >= 0x00100001)
			{
				VBYTE flags;
				stream->read(&flags, sizeof(flags));
				gridInfo.flags = flags;
			}

			if (header.version >= 0x00100003)
			{
				VBOOL hasDiffuse = VFALSE;
				stream->read(&hasDiffuse, sizeof(hasDiffuse));
				if (hasDiffuse)
				{
					VBYTE col[3];
					stream->read(&col, sizeof(col));

					Ogre::ColourValue color = Ogre::ColourValue::White;
					color.r = (VFLOAT) col[0] / 256.0f;
					color.g = (VFLOAT) col[1] / 256.0f;
					color.b = (VFLOAT) col[2] / 256.0f;
				}
			}

		}
	}
}

void VTerrain::_saveGridInfo(const VString &filename) const
{

}

void VTerrain::_loadLightmap(const VString &filename, const VString &type, const VString &groupName)
{
	assert(!mLightmapImage);

	if (filename.empty())
		return;

	if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(groupName, filename))
		return;

	if (type == "image")
	{
		mLightmapImage = new Ogre::Image();
		try
		{
			mLightmapImage->load(filename, groupName);
		}
		catch (const Ogre::Exception &e)
		{
			delete mLightmapImage;
			mLightmapImage = VNULL;
			return;
		}

		if (mLightmapImage->getDepth() != 1 
			|| mLightmapImage->getNumFaces() != 1 
			|| mLightmapImage->getWidth() != mInfo->mXSize * 8
			|| mLightmapImage->getHeight() != mInfo->mZSize * 8)
		{
			delete mLightmapImage;
			mLightmapImage = VNULL;
			Ogre::LogManager::getSingleton().logMessage("============================NOTICE===============================");
			Ogre::LogManager::getSingleton().logMessage("Terrain lightmap out of date, please update terrain lightmap.");
			Ogre::LogManager::getSingleton().logMessage("=================================================================");
		}
	}
	else if (type == "standard")
	{
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);

		LightmapHeader header;
		stream->read(&header, sizeof(header));

		if (header.magic != TERRAIN_LIGHTMAP_MAGIC)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid magic of lightmap file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadLightmap");
		}

		if (header.version != TERRAIN_LIGHTMAP_VERSION)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid version of lightmap file '" + filename + "'while loading terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadLightmap");
		}

		if (header.width != mInfo->mXSize * 8 || header.depth != mInfo->mZSize * 8)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Dimensions of lightmap file '" + filename + "' doesn't match the dimensions of the terrain '" + mInfo->mName + "'.",
				"VTerrain::_loadLightmap");
		}

		if (stream->tell() + header.width * header.depth != stream->size())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
				"Invalid lightmap file '" + filename + "'.",
				"VTerrain::_loadLightmap");
		}

		mLightmapImage = new Ogre::Image();
		mLightmapImage->loadRawData(stream, header.width, header.depth, Ogre::PF_L8);
	}
}

void VTerrain::_saveLightmap(const VString &filename) const
{

}

void VTerrain::_createTiles()
{
	// create a root node for the terrain tiles
	mTerrainNode = 0;
	VString nodeName = mInfo->mName + "/Terrain";
	if (mSceneMgr->hasSceneNode(nodeName))
		mTerrainNode = mSceneMgr->getSceneNode(nodeName);
	else
		mTerrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	
	// create the tile grid
	VINT32 width = mInfo->getXSize();
	VINT32 depth = mInfo->getZSize();
	VINT32 tileSize = mInfo->getTileSize();
	VINT32 numTilesCol = width / tileSize;
	VINT32 numTilesRow = depth / tileSize;
	VINT32 total = numTilesCol * numTilesRow;
	mTiles.assign(total, (VTerrainTile*)VNULL);

	// for each tile, create a scene node to attach to and create the tile
	VINT32 index = 0;
	for (VINT32 row = 0; row < numTilesRow; ++row)
	{
		for (VINT32 col = 0; col < numTilesCol; ++col)
		{
			VString name = nodeName + "/Tile[" + Ogre::StringConverter::toString(row) + "][" + Ogre::StringConverter::toString(col) + "]";
			Ogre::SceneNode *node = VNULL;

			if (mSceneMgr->hasSceneNode(name))
			{
				node = mSceneMgr->getSceneNode(name);
				if (node->getParentSceneNode() != mTerrainNode)
					mTerrainNode->addChild(node);
			}
			else
			{
				node = mTerrainNode->createChildSceneNode(name);
			}

			VTerrainTile *tile = new VTerrainTile(name, this, col*tileSize, row*tileSize, tileSize, tileSize);
			node->attachObject(tile);
			mTiles[index++] = tile;
		}
	}
}

void VTerrain::_destroyTiles()
{
	TileArray::const_iterator itr;
	for (itr = mTiles.begin(); itr != mTiles.end(); ++itr)
	{
		VTerrainTile *tile = *itr;
		delete tile;
	}
	mTiles.clear();
}

void VTerrain::_createTextures()
{
	assert(mInfo);

	assert(mLayer.isNull());

	// 创建第一层纹理，TERRAIN_TEXTURE_MAX_MIPMAP个mipmap，自己手动创建
	mLayer = Ogre::TextureManager::getSingletonPtr()->createManual("<layer0>", 
		mGroupName, Ogre::TEX_TYPE_2D, TERRAIN_TEXTURE_ATLASE_W, TERRAIN_TEXTURE_ATLASE_H, 
		1, TERRAIN_TEXTURE_MAX_MIPMAP, Ogre::PF_A8R8G8B8, Ogre::TU_WRITE_ONLY);

	// 第二层纹理和第一层使用相同的，直接将第二层指向第一层的纹理

	// 合成一整张纹理图，同时生成mipmap
	VINT32 imgWidth = 0, imgHeight = 0;
	VINT32 index = 0;
	TextureArray::const_iterator itr;
	Ogre::Image image;
	for (itr = mInfo->mTextures.begin(); itr != mInfo->mTextures.end(); ++itr)
	{
		image.load(*itr, mGroupName);
		imgWidth = image.getWidth();
		imgHeight = image.getHeight();
		// 将贴图大小调整为256*256，把大贴图填满，这样似乎能缓解一些地形裂缝的问题?
		image.resize(TERRAIN_ATLASE_IMAGE_SIZE, TERRAIN_ATLASE_IMAGE_SIZE);
		imgWidth = image.getWidth();
		imgHeight = image.getHeight();
		// 添加的纹理中，并生成mipmap
		_addImageToTexture(mLayer, index, image, TERRAIN_ATLASE_IMAGE_SIZE);
		++index;
	}

	// 生成一张透明贴图放在最后，用于只有一层纹理的地形使用
	image.load("transparent.tga", mGroupName);
	image.resize(TERRAIN_ATLASE_IMAGE_SIZE, TERRAIN_ATLASE_IMAGE_SIZE);
	_addImageToTexture(mLayer, index, image, TERRAIN_ATLASE_IMAGE_SIZE);

	// 生成阴影纹理
	if (hasLightmap())
	{
		assert(mLightmap.isNull());
		mLightmap = Ogre::TextureManager::getSingletonPtr()->createManual("<lightmap>", 
			mGroupName, Ogre::TEX_TYPE_2D, 512, 512, 1, Ogre::PF_A8R8G8B8);

		assert(mLightmapImage);
		mLightmap->getBuffer()->blitFromMemory(mLightmapImage->getPixelBox());
	}
}

void VTerrain::_destroyTextures()
{
	Ogre::TextureManager *textureManager = Ogre::TextureManager::getSingletonPtr();
	if (textureManager)
	{
		if (!mLayer.isNull())
		{
			Ogre::ResourceHandle handle = mLayer->getHandle();
			textureManager->remove(handle);
			mLayer.setNull();
		}

		if (!mLightmap.isNull())
		{
			Ogre::ResourceHandle handle = mLightmap->getHandle();
			textureManager->remove(handle);
			mLightmap.setNull();
		}
	}
}

void VTerrain::_createMaterials()
{
	assert(mInfo);
	if (hasLightmap())
	{
		mMaterial = Ogre::MaterialManager::getSingleton().getByName("Terrain/TwoLayerLightmap");
	}
	else
	{
		mMaterial = Ogre::MaterialManager::getSingleton().getByName("Terrain/TwoLayer");
	}

	// layer0和layer1用的相同的纹理。
	mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName("<layer0>");
}

void VTerrain::_destroyMaterials()
{
	Ogre::MaterialManager *materialManager = Ogre::MaterialManager::getSingletonPtr();
	if (materialManager)
	{
		if (!mMaterial.isNull())
		{
			Ogre::ResourceHandle handle = mMaterial->getHandle();
			materialManager->remove(handle);
		}
	}
}

void VTerrain::_reset()
{
	mInfo->reset();

	mHeightmap.clear();
	mGridInfos.clear();

	delete mLightmapImage;
	mLightmapImage = VNULL;

	_destroyTiles();
	_destroyMaterials();
	_destroyTextures();

	if (mTerrainNode)
	{
		mSceneMgr->destroySceneNode(mTerrainNode);
		mTerrainNode = VNULL;
	}
}

void VTerrain::_addImageToTexture(Ogre::TexturePtr texture, VINT32 index, Ogre::Image &image, VINT32 imageSize)
{
	VINT32 imgWidth = image.getWidth();		// 图像宽
	VINT32 imgHeight = image.getHeight();	// 图像高
	VINT32 cols = texture->getWidth() / imageSize;	// 小纹理在大纹理中的列数
	VINT32 row = index / cols;	// 当前图像在大纹理中的行
	VINT32 col = index % cols;	// 当前图像在大纹理中的列
	VINT32 numOfMips = texture->getNumMipmaps();
	VINT32 i = 0;

	for (i = 0; i <= numOfMips; ++i)
	{
		image.resize((imgWidth>>i), (imgHeight>>i), Ogre::Image::FILTER_BILINEAR);
		_addMipmapToTexture(texture->getBuffer(0,i), row, col, image, (imageSize>>i));
	}
}

void VTerrain::_addMipmapToTexture(Ogre::HardwarePixelBufferSharedPtr pixels, VINT32 row, VINT32 col, Ogre::Image &image, VINT32 imageSize)
{
	assert(!pixels.isNull());

	pixels->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	const Ogre::PixelBox &pb = pixels->getCurrentLock();

	VBYTE *src = image.getData();
	VBYTE *dst = static_cast<VBYTE*>(pb.data);

	VINT32 srcBpp = Ogre::PixelUtil::getNumElemBytes(image.getFormat());
	VINT32 dstBpp = Ogre::PixelUtil::getNumElemBytes(Ogre::PF_A8R8G8B8);

	assert(srcBpp == 3 || srcBpp == 4);
	assert(dstBpp == 4);
	
	VINT32 pitch = pb.rowPitch;
	VINT32 srcPitch = image.getRowSpan();
	VINT32 dstPitch = pitch * dstBpp;

	VINT32 start_y = row * imageSize;
	VINT32 end_y = start_y + image.getHeight();

	VINT32 start_x = col * imageSize;
	VINT32 end_x = start_x + image.getWidth();

	VBYTE *srcData = src;
	dst = dst + start_y * dstPitch + start_x * dstBpp;
	VBYTE *dstData = dst;

	VINT32 x = 0, y= 0;
	for (y = start_y; y < end_y; ++y)
	{
		dstData = dst;
		srcData = src;

		for (x = start_x; x < end_x; ++x)
		{
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			if (srcBpp < dstBpp)
			{
				*dstData++ = 255;
			}
			else
			{
				*dstData++ = *srcData++;
			}
		}

		dst += dstPitch;
		src += srcPitch;
	}
	
	pixels->unlock();
}

Ogre::Vector3 VTerrain::_getNormalAt(VINT32 x, VINT32 z) const
{
	VINT32 flip = 1;
	VINT32 index = x + z * (mInfo->mXSize + 1);
	Ogre::Vector3 here((VFLOAT)x, _getHeightAt(index), (VFLOAT)z);
	Ogre::Vector3 right;
	Ogre::Vector3 back;

	if (x >= mInfo->mXSize)
	{
		flip = -1;
		right = Ogre::Vector3(VFLOAT(x-1), _getHeightAt(index-1), VFLOAT(z));
	}
	else
	{
		right = Ogre::Vector3(VFLOAT(x+1), _getHeightAt(index+1), VFLOAT(z));
	}

	if (z >= mInfo->mZSize)
	{
		flip = -1;
		back = Ogre::Vector3(VFLOAT(x), _getHeightAt(index-mInfo->mXSize-1), VFLOAT(z-1));
	}
	else
	{
		back = Ogre::Vector3(VFLOAT(x), _getHeightAt(index+mInfo->mXSize+1), VFLOAT(z+1));
	}

	// 生成矢量
	right -= here;
	back -= here;

	Ogre::Vector3 normal = Ogre::Real(flip) * back.crossProduct(right);
	normal.normalise();
	return normal;
}

