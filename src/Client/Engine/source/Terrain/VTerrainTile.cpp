

#include "VTerrainTile.h"
#include "VTerrain.h"
#include "VTerrainInfo.h"
#include "VTrace.h"


#define MAIN_BINDING		0


class VTexCoord
{
public:
	void	**getDataPtr()	{ return (void**)&mData;	}
	float	*getUPtr()		{ return mData;				}
	float	*getYPtr()		{ return mData+1;			}
	float	&u()			{ return *mData;			}
	float	&v()			{ return *(mData+1);		}

	void Exchange(VTexCoord &other)
	{
		float temp1 = u(), temp2 = v();
		u() = other.u();
		v() = other.v();
		other.u() = temp1;
		other.v() = temp2;
	}

private:
	float	*mData;
};



VTerrainTile::VTerrainTile(const Ogre::String &name, VTerrain *owner, VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize)
	: MovableObject(name)
	, mOwner(owner)
	, mVertexData(VNULL)
	, mIndexData(VNULL)
	, mLightListDirty(VTRUE)
{
// 	Ogre::String name = "Tile[" + Ogre::StringConverter::toString(startz) + "][" + Ogre::StringConverter::toString(startx) + "]";
// 	mTileNode = parent->createChildSceneNode(name);
// 	mTileNode->attachObject(this);

	mSceneMgr = Ogre::Root::getSingleton().getSceneManager("VisionSceneManager");

	_createVertexData(startx, startz, xsize, zsize);
	_createIndexData(startx, startz, xsize, zsize);
}

VTerrainTile::~VTerrainTile()
{
// 	mTileNode->detachObject(this);
}

const Ogre::String &VTerrainTile::getMovableType() const
{
	static Ogre::String type = "VisionTerrainTile";
	return type;
}

const Ogre::AxisAlignedBox &VTerrainTile::getBoundingBox() const
{
	return mBounds;
}

Ogre::Real VTerrainTile::getBoundingRadius() const
{
	return mBoundingRadius;
}

void VTerrainTile::_updateRenderQueue(Ogre::RenderQueue *queue)
{
	mLightListDirty = VTRUE;
	queue->addRenderable(this, mRenderQueueID);
}

const Ogre::MaterialPtr &VTerrainTile::getMaterial() const
{
	assert(mOwner);
	return mOwner->_getMaterial();
}

void VTerrainTile::getRenderOperation(Ogre::RenderOperation &op)
{
	op.useIndexes = true;
	op.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	op.vertexData = mVertexData;
	op.indexData = mIndexData; //get
}

void VTerrainTile::getWorldTransforms(Ogre::Matrix4* xform) const
{
	*xform = mParentNode->_getFullTransform();
}

Ogre::Real VTerrainTile::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	Ogre::Vector3 diff = mCenter - cam->getDerivedPosition();
	return diff.squaredLength();
}

const Ogre::LightList &VTerrainTile::getLights() const
{
	if (mLightListDirty)
	{
		mSceneMgr->_populateLightList(mCenter, getBoundingRadius(), mLightList);
		mLightListDirty = VFALSE;
	}
	return mLightList;
}

#if OGRE_VERSION_MINOR > 4
/** Shoggoth compatibility function */
void VTerrainTile::visitRenderables(Ogre::Renderable::Visitor *visitor, bool debugRenderables)
{
	visitor->visit(this, 0, false, 0);
}
#endif

void VTerrainTile::_createVertexData(VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize)
{
	assert(mOwner && mOwner->mInfo);
	assert(!mVertexData);

	mVertexData = new Ogre::VertexData();
	mVertexData->vertexStart = 0;
	mVertexData->vertexCount = xsize * zsize * 4;

	Ogre::VertexDeclaration *decl = mVertexData->vertexDeclaration;
	Ogre::VertexBufferBinding *bind = mVertexData->vertexBufferBinding;

	size_t offset = 0;
	// 顶点位置
	decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	// 顶点法线
	decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	// 纹理坐标
	// layer0
	decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	// layer1
	decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	if (mOwner->hasLightmap())
	{
		// lightmap
		decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 2);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	}
	
	// 
	VINT32 ss = decl->getVertexSize(MAIN_BINDING);
	Ogre::HardwareVertexBufferSharedPtr vertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		decl->getVertexSize(MAIN_BINDING), mVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	bind->setBinding(MAIN_BINDING, vertexBuffer);

	Ogre::Real minHeight = mOwner->mOffset.y + mOwner->mInfo->getScale().y;
	Ogre::Real maxHeight = mOwner->mOffset.y;

	Pixmap emptyPixmap;
	emptyPixmap.textureID = mOwner->mInfo->getTextureCount();
	emptyPixmap.left = 0.1f;
	emptyPixmap.top = 0.1f;
	emptyPixmap.right = 0.9f;
	emptyPixmap.bottom = 0.9f;

	VINT32 endx = startx + xsize;
	VINT32 endz = startz + zsize;

	VFLOAT *vertices = static_cast<VFLOAT*>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	size_t numOfVertices = vertexBuffer->getNumVertices();
	size_t sizeOfVertex = vertexBuffer->getVertexSize();

	VINT32 width = mOwner->mInfo->getXSize();
	VINT32 depth = mOwner->mInfo->getZSize();

	VINT32 x = 0, z = 0;
	for (z = startz; z < endz; ++z)
	{
		for (x = startx; x < endx; ++x)
		{
			const GridInfo &gridInfo = mOwner->mGridInfos[x+z*width];

			if (gridInfo.layers[0].pixmapID == 0)
				continue;

			Pixmap pixmap[2];
			pixmap[0] = mOwner->mInfo->getPixmap(gridInfo.layers[0].pixmapID-1);
			if (gridInfo.layers[1].pixmapID != 0)
				pixmap[1] = mOwner->mInfo->getPixmap(gridInfo.layers[1].pixmapID-1);
			else
				pixmap[1] = emptyPixmap;

			VINT32 row[2] = {pixmap[0].textureID/TERRAIN_ATLASE_COL, pixmap[1].textureID/TERRAIN_ATLASE_COL};
			VINT32 col[2] = {pixmap[0].textureID%TERRAIN_ATLASE_COL, pixmap[1].textureID%TERRAIN_ATLASE_COL};

			VFLOAT basex[2] = {(VFLOAT)col[0]/TERRAIN_ATLASE_COL, (VFLOAT)col[1]/TERRAIN_ATLASE_COL};
			VFLOAT basez[2] = {(VFLOAT)row[0]/TERRAIN_ATLASE_ROW, (VFLOAT)row[1]/TERRAIN_ATLASE_ROW};

			VTexCoord texCoord[2][4];

			// TopLeft Vertex
			// position
			VFLOAT height = mOwner->mOffset.y + mOwner->_getHeightAt(x, z) * mOwner->mInfo->getScale().y;
			*vertices++ = mOwner->mOffset.x + mOwner->mInfo->getScale().x * x;
			*vertices++ = height;
			*vertices++ = mOwner->mOffset.z + mOwner->mInfo->getScale().z * z;
			// normal
			Ogre::Vector3 normal = mOwner->_getNormalAt(x, z);
			*vertices++ = normal.x;
			*vertices++ = normal.y;
			*vertices++ = normal.z;
			// layer0 & layer1
			*texCoord[0][0].getDataPtr() = vertices;
			vertices += 2;
			*texCoord[1][0].getDataPtr() = vertices;
			vertices += 2;
			VINT32 i = 0;
			for (i = 0; i < 2; ++i)
			{
				texCoord[i][0].u() = basex[i]+(VFLOAT)pixmap[i].left/TERRAIN_ATLASE_COL;
				texCoord[i][0].v() = basez[i]+(VFLOAT)pixmap[i].top/TERRAIN_ATLASE_ROW;
			}
			// light map
			if (mOwner->hasLightmap())
			{
				*vertices++ = VFLOAT(x) / (mOwner->mInfo->getXSize());
				*vertices++ = VFLOAT(z) / (mOwner->mInfo->getZSize());
			}

			// vertex TopRight
			// position
			height = mOwner->mOffset.y + mOwner->_getHeightAt(x+1, z) * mOwner->mInfo->getScale().y;
			*vertices++ = mOwner->mOffset.x + mOwner->mInfo->getScale().x * (x + 1);
			*vertices++ = height;
			*vertices++ = mOwner->mOffset.z + mOwner->mInfo->getScale().z * z;
			// normal
			normal = mOwner->_getNormalAt(x+1, z);
			*vertices++ = normal.x;
			*vertices++ = normal.y;
			*vertices++ = normal.z;
			// layer0 & layer1
			*texCoord[0][1].getDataPtr() = vertices;
			vertices += 2;
			*texCoord[1][1].getDataPtr() = vertices;
			vertices += 2;
			for (i = 0; i < 2; ++i)
			{
				texCoord[i][1].u() = basex[i]+(VFLOAT)pixmap[i].right/TERRAIN_ATLASE_COL;
				texCoord[i][1].v() = basez[i]+(VFLOAT)pixmap[i].top/TERRAIN_ATLASE_ROW;
			}
			// light map
			if (mOwner->hasLightmap())
			{
				*vertices++ = VFLOAT(x+1) / (mOwner->mInfo->getXSize());
				*vertices++ = VFLOAT(z) / (mOwner->mInfo->getZSize());
			}

			// vertex BottomLeft
			// position
			height = mOwner->mOffset.y + mOwner->_getHeightAt(x, z+1) * mOwner->mInfo->getScale().y;
			*vertices++ = mOwner->mOffset.x + mOwner->mInfo->getScale().x * x;
			*vertices++ = height;
			*vertices++ = mOwner->mOffset.z + mOwner->mInfo->getScale().z * (z + 1);
			// normal
			normal = mOwner->_getNormalAt(x, z+1);
			*vertices++ = normal.x;
			*vertices++ = normal.y;
			*vertices++ = normal.z;
			// layer0 & layer1
			*texCoord[0][2].getDataPtr() = vertices;
			vertices += 2;
			*texCoord[1][2].getDataPtr() = vertices;
			vertices += 2;
			for (i = 0; i < 2; ++i)
			{
				texCoord[i][2].u() = basex[i]+(VFLOAT)pixmap[i].left/TERRAIN_ATLASE_COL;
				texCoord[i][2].v() = basez[i]+(VFLOAT)pixmap[i].bottom/TERRAIN_ATLASE_ROW;
			}
			// light map
			if (mOwner->hasLightmap())
			{
				*vertices++ = VFLOAT(x) / (mOwner->mInfo->getXSize());
				*vertices++ = VFLOAT(z+1) / (mOwner->mInfo->getZSize());
			}

			// vertex BottomRight
			// position
			height = mOwner->mOffset.y + mOwner->_getHeightAt(x+1, z+1) * mOwner->mInfo->getScale().y;
			*vertices++ = mOwner->mOffset.x + mOwner->mInfo->getScale().x * (x + 1);
			*vertices++ = height;
			*vertices++ = mOwner->mOffset.z + mOwner->mInfo->getScale().z * (z + 1);
			// normal
			normal = mOwner->_getNormalAt(x+1, z+1);
			*vertices++ = normal.x;
			*vertices++ = normal.y;
			*vertices++ = normal.z;
			// layer0 & layer1
			*texCoord[0][3].getDataPtr() = vertices;
			vertices += 2;
			*texCoord[1][3].getDataPtr() = vertices;
			vertices += 2;
			for (i = 0; i < 2; ++i)
			{
				texCoord[i][3].u() = basex[i]+(VFLOAT)pixmap[i].right/TERRAIN_ATLASE_COL;
				texCoord[i][3].v() = basez[i]+(VFLOAT)pixmap[i].bottom/TERRAIN_ATLASE_ROW;
			}
			// light map
			if (mOwner->hasLightmap())
			{
				*vertices++ = VFLOAT(x+1) / (mOwner->mInfo->getXSize());
				*vertices++ = VFLOAT(z+1) / (mOwner->mInfo->getZSize());
			}

			VBOOL isIndex = gridInfo.flags == GridInfo::FlipDiagonal;
			_changeGridInfoUV(texCoord[0][0], texCoord[0][1], texCoord[0][2], texCoord[0][3], gridInfo.layers[0].orientation, isIndex);

			if (gridInfo.layers[1].pixmapID != 0)
				_changeGridInfoUV(texCoord[1][0], texCoord[1][1], texCoord[1][2], texCoord[1][3], gridInfo.layers[1].orientation, isIndex);

			if (height < minHeight)
				minHeight = height;
			if (height > maxHeight)
				maxHeight = height;
		}
	}

	vertexBuffer->unlock();

	mBounds.setExtents(mOwner->mOffset.x + mOwner->mInfo->getScale().x * startx, minHeight,
		mOwner->mOffset.z + mOwner->mInfo->getScale().z * startz,
		mOwner->mOffset.x + mOwner->mInfo->getScale().x * endx, maxHeight,
		mOwner->mOffset.z + mOwner->mInfo->getScale().z * (endz));

	mCenter = mBounds.getCenter();

	mBoundingRadius = (mBounds.getMaximum() - mCenter).length();
}

void VTerrainTile::_createIndexData(VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize)
{
	assert(mOwner && mOwner->mInfo);
	assert(!mIndexData);

	mIndexData = new Ogre::IndexData();
	mIndexData->indexStart = 0;
	mIndexData->indexCount = xsize * zsize * 6;
	mIndexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
		Ogre::HardwareIndexBuffer::IT_16BIT, 
		mIndexData->indexCount, 
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	VWORD *indices = static_cast<VWORD*>(mIndexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	VINT32 x = 0, z = 0;
	for (z = 0; z < zsize; ++z)
	{
		for (x = 0; x < xsize; ++x)
		{
			VINT32 index = (z + startz) * mOwner->mInfo->getXSize() + startx + x;
			const GridInfo &gridInfo = mOwner->mGridInfos[index];
			VWORD iIndexTopLeft = (x + z * xsize) * 4;
			VWORD iIndexTopRight = iIndexTopLeft + 1;
			VWORD iIndexBottomLeft = iIndexTopLeft + 2;
			VWORD iIndexBottomRight = iIndexTopLeft + 3;

			if (0 == gridInfo.flags) 
			{
				*indices++ = iIndexTopLeft;
				*indices++ = iIndexBottomLeft;
				*indices++ = iIndexTopRight;
				*indices++ = iIndexBottomLeft;
				*indices++ = iIndexBottomRight;
				*indices++ = iIndexTopRight;
			}
			else {
				*indices++ = iIndexBottomLeft;
				*indices++ = iIndexBottomRight;
				*indices++ = iIndexTopLeft;
				*indices++ = iIndexBottomRight;
				*indices++ = iIndexTopRight;
				*indices++ = iIndexTopLeft;
			}
		}
	}
	mIndexData->indexBuffer->unlock();
}

void VTerrainTile::_changeGridInfoUV(VTexCoord& leftTop, VTexCoord& rightTop, 
									VTexCoord& leftBottom, VTexCoord& rightBottom, 
									VWORD state, VBOOL isIndex)
{
	//0 不变
	//1 图片水平翻转
	//2 图片垂直翻转
	//4 顺时针旋转90度
	//8 对角线上方顶点纹理坐标复制到对角线下方顶点。（与对角线垂直的两个顶点）
	VWORD res1 = state&1;
	VWORD res2 = state&2;
	VWORD res3 = state&4;
	VWORD res4 = state&8;

	if (res1 != 0)
	{
		leftTop.Exchange(rightTop);
		leftBottom.Exchange(rightBottom);
	}

	if (res2 != 0)
	{
		leftTop.Exchange(leftBottom);
		rightTop.Exchange(rightBottom);
	}

	if (res3 != 0)
	{
		leftTop.Exchange(rightTop);
		leftBottom.Exchange(rightTop);
		rightBottom.Exchange(rightTop);
	}

	if (res4 != 0)
	{
		// 非正常索引
		if (isIndex) 
		{
			leftBottom.u() = rightTop.u();
			leftBottom.v() = rightTop.v();
		}
		// 正常索引
		else
		{
			rightBottom.u() = leftTop.u();
			rightBottom.v() = leftTop.v();
		}
	}
}
