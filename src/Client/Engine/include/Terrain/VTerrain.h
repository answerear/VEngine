

#ifndef __VISION_TERRAIN_H__
#define __VISION_TERRAIN_H__


#include "VEngineConfig.h"
#include "VTerrainInfo.h"


class XMLParser;
class XMLWriter;
class VTerrainTile;


const VUINT32 TERRAIN_ATLASE_IMAGE_SIZE = 256;
const VUINT32 TERRAIN_ATLASE_COL = 8;
const VUINT32 TERRAIN_ATLASE_ROW = 4;


enum Corner
{
	LEFT_TOP		= 0,
	RIGHT_TOP		= 1,
	LEFT_BOTTOM		= 2,
	RIGHT_BOTTOM	= 3,
};

enum
{
	NumLayers = 2		/**< Number of texture layers */
};

struct LayerInfo
{
	VWORD	pixmapID;
	VWORD	orientation;

	VBOOL operator ==(const LayerInfo &other) const
	{
		return (pixmapID == other.pixmapID && orientation == other.orientation);
	}

	VBOOL operator !=(const LayerInfo &other) const
	{
		return !(*this == other);
	}
};

struct GridInfo
{
	LayerInfo layers[NumLayers];
	VUINT32	flags;

	enum Flag
	{
		FlipDiagonal = 1 << 0,
		DisableDecal = 1 << 1,
	};

	// 		const Corner *getCorners() const
	// 		{
	// 			return msConnerMap[(flags & FlipDiagonal) ? 1 : 0];
	// 		}

	VBOOL operator ==(const GridInfo &other) const
	{
		return layers[0] == other.layers[0] && layers[1] == other.layers[1] && flags == other.flags;
	}

	VBOOL operator !=(const GridInfo &other) const
	{
		return !(*this == other);
	}
};

struct MaterialID
{
	VBYTE textureID0;
	VBYTE textureID1;

	MaterialID()
		: textureID0(0)
		, textureID1(0)
	{
	}

	MaterialID(VBYTE textureID0, VBYTE textureID1)
		: textureID0(textureID0)
		, textureID1(textureID1)
	{
	}

	VBOOL operator ==(const MaterialID &other) const
	{
		return (textureID0 == other.textureID0 && textureID1 == other.textureID1);
	}

	VBOOL operator !=(const MaterialID &other) const
	{
		return !(*this == other);
	}

	VBOOL operator <(const MaterialID &other) const
	{
		if (textureID0 < other.textureID0)
			return VTRUE;
		if (other.textureID0 < textureID0)
			return VFALSE;
		if (textureID1 < other.textureID1)
			return VTRUE;
		if (other.textureID1 < textureID1)
			return VFALSE;
		return VFALSE;
	}
};

typedef std::vector<GridInfo> GridInfoArray;
typedef std::vector<Ogre::Real> RealArray;
typedef std::vector<VTerrainTile*> TileArray;
typedef std::vector<Ogre::TexturePtr> TextureList;
typedef std::map<MaterialID, Ogre::MaterialPtr> MaterialMap;
typedef std::vector<Ogre::MaterialPtr> MaterialArray;


class VTerrain
{
	friend class VTerrainTile;
	friend class VScene;

public:
	VTerrain();
	virtual ~VTerrain();

	// 读取地形数据
	void load(XMLParser *parser, const VString &filename, const VString &groupName, VBOOL hasLightmap);
	// 保存地形数据
	void save(XMLWriter *writer, const VString &filename) const;

	// 创建地形
	void create();
	// 销毁地形
	void destroy();

	// 是否有阴影图
	VBOOL hasLightmap() const
	{
		return (mLightmapImage != VNULL);
	}

	// 获取对应点的高度
	VFLOAT getHeightAt(VFLOAT x, VFLOAT z) const;

protected:
	// 读取地形基本信息数据
	void _loadTerrainInfo(XMLParser *parser, const VString &filename, const VString groupName);
	// 保存地形基本信息数据
	void _saveTerrainInfo(XMLWriter *writer, const VString &filename) const;

	// 读取地形高度图数据
	void _loadHeightmap(const VString &filename, const VString &type, const VString &groupName);
	// 保存地形高度图数据
	void _saveHeightmap(const VString &filename) const;

	// 读取地形网格信息数据
	void _loadGridInfo(const VString &filename, const VString &groupName);
	// 保存地形网格信息数据
	void _saveGridInfo(const VString &filename) const;

	// 读取阴影图数据
	void _loadLightmap(const VString &filename, const VString &type, const VString &groupName);
	// 保存阴影图数据
	void _saveLightmap(const VString &filename) const;

	// 创建tile对象
	void _createTiles();
	// 删除tile对象
	void _destroyTiles();

	// 创建地形纹理
	void _createTextures();
	// 删除地形纹理
	void _destroyTextures();

	// 创建地形材质
	void _createMaterials();
	// 删除地形材质
	void _destroyMaterials();

	// 重置数据
	void _reset();

	// 添加图像到纹理中并且生成纹理对应的mipmap
	void _addImageToTexture(Ogre::TexturePtr texture, VINT32 index, Ogre::Image &image, VINT32 imageSize);

	// 生成图像对应的纹理mipmap
	void _addMipmapToTexture(Ogre::HardwarePixelBufferSharedPtr pixels, VINT32 row, VINT32 col, Ogre::Image &image, VINT32 imageSize);

	// 获取地形纹理
	const Ogre::MaterialPtr &_getMaterial() const { return mMaterial; }

	// 获取对应点的高度图的高度
	VFLOAT &_getHeightAt(VINT32 x, VINT32 z) { return mHeightmap[x+z*(mInfo->getXSize()+1)]; }
	const VFLOAT &_getHeightAt(VINT32 x, VINT32 z) const { return mHeightmap[x+z*(mInfo->getXSize()+1)]; }
	VFLOAT &_getHeightAt(VINT32 index) { return mHeightmap[index]; }
	const VFLOAT &_getHeightAt(VINT32 index) const { return mHeightmap[index]; }

	// 获取对应点的法线
	Ogre::Vector3 _getNormalAt(VINT32 x, VINT32 z) const;

protected:
	VString				mGroupName;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::SceneNode		*mTerrainNode;

	Ogre::Vector3		mOffset;	// 地形偏移

	VTerrainInfo	*mInfo;			// 地形基本信息
	RealArray		mHeightmap;		// 高度图数组
	GridInfoArray	mGridInfos;		// 网格信息
	Ogre::Image		*mLightmapImage;// 阴影图对象

	Ogre::TexturePtr	mLayer;		// 图层纹理数据
	Ogre::TexturePtr	mLightmap;	// 阴影图纹理数据
	Ogre::MaterialPtr	mMaterial;	// 材质数据
	TileArray			mTiles;		// Tile数组，保存渲染用的tile对象
};


#endif	/*__VISION_TERRAIN_H__*/
