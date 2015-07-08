

#ifndef __VISION_TERRAIN_INFO_H__
#define __VISION_TERRAIN_INFO_H__


#include "VEngineConfig.h"


class XMLParser;
class XMLWriter;

enum OrientationFlags
{
	FlipHorizontal		= 1,	/**< ͼƬˮƽ��ת�������ҷ�ת������ */
	FlipVertical		= 2,	/**< ͼƬ��ֱ��ת�������·�ת���ߵ� */
	RotateRightAngle	= 4,	/**< ��ʱ����ת90�� */
	MirrorDiagonal		= 8,	/**< �Խ��߾��� */
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
	VINT32	mXSize;		// ��ͼ��С
	VINT32	mZSize;		// ��ͼ��С
	VINT32	mTileSize;	// Tile��С
	Ogre::Vector3 mScale;		// ��ͼ����
	Ogre::Vector3 mPosition;	// ��ͼ���ĵ�λ��

	VString mName;	// ��ͼ����

	VString mHeightmapFilename;	// �߶�ͼ�ļ���
	VString mHeightmapType;		// �߶�ͼ����

	VString mGridInfoFilename;	// ������Ϣ�ļ���
	VString mGridInfoType;		// ������Ϣ����

	VString mLightmapFilename;	// ��Ӱͼ�ļ���
	VString mLightmapType;		// ��Ӱͼ����

	PixmapArray mPixmaps;		// ����ͼ����
	TextureArray mTextures;		// ��������

	MaterialTemplates mMaterialTemplates;	// ����ģ������
	FogReplacements mFogReplacements;		// ��Ч������
};


#endif	/*__VISION_TERRAIN_INFO_H__*/
