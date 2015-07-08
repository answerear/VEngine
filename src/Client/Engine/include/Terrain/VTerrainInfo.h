

#ifndef __VISION_TERRAIN_INFO_H__
#define __VISION_TERRAIN_INFO_H__


#include "VEngineConfig.h"


class XMLParser;
class XMLWriter;

enum OrientationFlags
{
	FlipHorizontal		= 1,	/**< 图片水平翻转，即左右翻转、镜像 */
	FlipVertical		= 2,	/**< 图片垂直翻转，即上下翻转、颠倒 */
	RotateRightAngle	= 4,	/**< 逆时针旋转90度 */
	MirrorDiagonal		= 8,	/**< 对角线镜像 */
};

struct Pixmap
{
	VINT32 textureID;
	Ogre::Real	left;
	Ogre::Real	top;
	Ogre::Real	right;
	Ogre::Real	bottom;

	Pixmap()	{}

	explicit Pixmap(VINT32 textureID)
		: textureID(textureID)
		, left(0)
		, top(0)
		, right(1)
		, bottom(1)
	{
	}

	Pixmap(VINT32 textureID, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom)
		: textureID(textureID)
		, left(left)
		, top(top)
		, right(right)
		, bottom(bottom)
	{
	}

	VBOOL operator ==(const Pixmap &other) const
	{
		return (textureID == other.textureID && left == other.left && right == other.right && top == other.top && bottom == other.bottom);
	}

	VBOOL operator !=(const Pixmap &other) const
	{
		return !(*this == other);
	}

	VBOOL operator <(const Pixmap &other) const
	{
		if (textureID < other.textureID)
			return VTRUE;
		if (textureID > other.textureID)
			return VFALSE;
		if (left < other.left)
			return VTRUE;
		if (left > other.left)
			return VFALSE;
		if (top < other.top)
			return VTRUE;
		if (top > other.top)
			return VFALSE;
		if (right < other.right)
			return VTRUE;
		if (right > other.right)
			return VFALSE;
		if (bottom < other.bottom)
			return VTRUE;
		if (bottom > other.bottom)
			return VFALSE;
		return VFALSE;
	}
};

struct FogReplacement
{
	VString programNames[4];
};


typedef std::vector<Pixmap> PixmapArray;
typedef std::vector<VString> TextureArray;
typedef std::map<VString,VString> MaterialTemplates;
typedef std::vector<FogReplacement> FogReplacements;


class VTerrainInfo
{
	friend class VTerrain;
	friend class VTerrainXMLHandler;

public:
	VTerrainInfo();
	virtual ~VTerrainInfo();

	void reset();

	VINT32 getXSize() const
	{
		return mXSize;
	}
	void setXSize(VINT32 xsize)
	{
		mXSize = xsize;
	}

	VINT32 getZSize() const
	{
		return mZSize;
	}
	void setZSize(VINT32 zsize)
	{
		mZSize = zsize;
	}

	VINT32 getTileSize() const
	{
		return mTileSize;
	}
	void setTileSize(VINT32 tileSize)
	{
		mTileSize = tileSize;
	}

	const Ogre::Vector3 &getScale() const
	{
		return mScale;
	}
	void setScale(const Ogre::Vector3 &scale)
	{
		mScale = scale;
	}

	const Ogre::Vector3 &getCenter() const
	{
		return mPosition;
	}
	void setCenter(const Ogre::Vector3 &pos)
	{
		mPosition = pos;
	}

	VINT32 getTextureCount() const
	{
		return mTextures.size();
	}

	const Pixmap &getPixmap(VINT32 index) const
	{
		return mPixmaps[index];
	}

	VINT32 getPixmapCount() const
	{
		return mPixmaps.size();
	}

protected:
	VINT32	mXSize;		// 地图大小
	VINT32	mZSize;		// 地图大小
	VINT32	mTileSize;	// Tile大小
	Ogre::Vector3 mScale;		// 地图缩放
	Ogre::Vector3 mPosition;	// 地图中心点位置

	VString mName;	// 地图名称

	VString mHeightmapFilename;	// 高度图文件名
	VString mHeightmapType;		// 高度图类型

	VString mGridInfoFilename;	// 网格信息文件名
	VString mGridInfoType;		// 网格信息类型

	VString mLightmapFilename;	// 阴影图文件名
	VString mLightmapType;		// 阴影图类型

	PixmapArray mPixmaps;		// 像素图数组
	TextureArray mTextures;		// 纹理数组

	MaterialTemplates mMaterialTemplates;	// 材质模板数组
	FogReplacements mFogReplacements;		// 雾效果数组
};


#endif	/*__VISION_TERRAIN_INFO_H__*/
