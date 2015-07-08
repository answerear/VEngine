

#ifndef __VISION_TERRAIN_TILE_H__
#define __VISION_TERRAIN_TILE_H__


#include "VEngineConfig.h"


class VTerrain;
class VTexCoord;


class VTerrainTile 
	: public Ogre::Renderable
	, public Ogre::MovableObject
{
public:
	VTerrainTile(const Ogre::String &name, VTerrain *owner, VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize);
	virtual ~VTerrainTile();

public:
	virtual const Ogre::String &getMovableType() const;
	virtual const Ogre::AxisAlignedBox &getBoundingBox() const;
	virtual Ogre::Real getBoundingRadius() const;
	virtual void _updateRenderQueue(Ogre::RenderQueue *queue);

	virtual const Ogre::MaterialPtr &getMaterial() const;
	virtual void getRenderOperation(Ogre::RenderOperation &op);
	virtual void getWorldTransforms(Ogre::Matrix4* xform) const;
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
	virtual const Ogre::LightList &getLights() const;

#if OGRE_VERSION_MINOR > 4
	/** Shoggoth compatibility function */
	void visitRenderables(Ogre::Renderable::Visitor *visitor, bool debugRenderables);
#endif

protected:
	void _createVertexData(VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize);
	void _createIndexData(VINT32 startx, VINT32 startz, VINT32 xsize, VINT32 zsize);
	void _changeGridInfoUV(VTexCoord &leftTop, VTexCoord &rightTop, VTexCoord &leftBottom, VTexCoord &rightBottom, VWORD state, VBOOL isIndex);

	VTerrain				*mOwner;

	Ogre::SceneManager		*mSceneMgr;
// 	Ogre::SceneNode			*mParentNode;
// 	Ogre::SceneNode			*mTileNode;
	Ogre::AxisAlignedBox	mBounds;
	Ogre::Real				mBoundingRadius;
	Ogre::Vector3			mCenter;

	Ogre::VertexData		*mVertexData;	// 顶点数据
	Ogre::IndexData			*mIndexData;	// 索引数据
	
	mutable Ogre::LightList	mLightList;
	mutable VBOOL			mLightListDirty;
};


#endif	/*__VISION_TERRAIN_TILE_H__*/

