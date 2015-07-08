

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

	// ��ȡ��������
	void load(XMLParser *parser, const VString &filename, const VString &groupName, VBOOL hasLightmap);
	// �����������
	void save(XMLWriter *writer, const VString &filename) const;

	// ��������
	void create();
	// ���ٵ���
	void destroy();

	// �Ƿ�����Ӱͼ
	VBOOL hasLightmap() const
	{
		return (mLightmapImage != VNULL);
	}

	// ��ȡ��Ӧ��ĸ߶�
	VFLOAT getHeightAt(VFLOAT x, VFLOAT z) const;

protected:
	// ��ȡ���λ�����Ϣ����
	void _loadTerrainInfo(XMLParser *parser, const VString &filename, const VString groupName);
	// ������λ�����Ϣ����
	void _saveTerrainInfo(XMLWriter *writer, const VString &filename) const;

	// ��ȡ���θ߶�ͼ����
	void _loadHeightmap(const VString &filename, const VString &type, const VString &groupName);
	// ������θ߶�ͼ����
	void _saveHeightmap(const VString &filename) const;

	// ��ȡ����������Ϣ����
	void _loadGridInfo(const VString &filename, const VString &groupName);
	// �������������Ϣ����
	void _saveGridInfo(const VString &filename) const;

	// ��ȡ��Ӱͼ����
	void _loadLightmap(const VString &filename, const VString &type, const VString &groupName);
	// ������Ӱͼ����
	void _saveLightmap(const VString &filename) const;

	// ����tile����
	void _createTiles();
	// ɾ��tile����
	void _destroyTiles();

	// ������������
	void _createTextures();
	// ɾ����������
	void _destroyTextures();

	// �������β���
	void _createMaterials();
	// ɾ�����β���
	void _destroyMaterials();

	// ��������
	void _reset();

	// ���ͼ�������в������������Ӧ��mipmap
	void _addImageToTexture(Ogre::TexturePtr texture, VINT32 index, Ogre::Image &image, VINT32 imageSize);

	// ����ͼ���Ӧ������mipmap
	void _addMipmapToTexture(Ogre::HardwarePixelBufferSharedPtr pixels, VINT32 row, VINT32 col, Ogre::Image &image, VINT32 imageSize);

	// ��ȡ��������
	const Ogre::MaterialPtr &_getMaterial() const { return mMaterial; }

	// ��ȡ��Ӧ��ĸ߶�ͼ�ĸ߶�
	VFLOAT &_getHeightAt(VINT32 x, VINT32 z) { return mHeightmap[x+z*(mInfo->getXSize()+1)]; }
	const VFLOAT &_getHeightAt(VINT32 x, VINT32 z) const { return mHeightmap[x+z*(mInfo->getXSize()+1)]; }
	VFLOAT &_getHeightAt(VINT32 index) { return mHeightmap[index]; }
	const VFLOAT &_getHeightAt(VINT32 index) const { return mHeightmap[index]; }

	// ��ȡ��Ӧ��ķ���
	Ogre::Vector3 _getNormalAt(VINT32 x, VINT32 z) const;

protected:
	VString				mGroupName;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::SceneNode		*mTerrainNode;

	Ogre::Vector3		mOffset;	// ����ƫ��

	VTerrainInfo	*mInfo;			// ���λ�����Ϣ
	RealArray		mHeightmap;		// �߶�ͼ����
	GridInfoArray	mGridInfos;		// ������Ϣ
	Ogre::Image		*mLightmapImage;// ��Ӱͼ����

	Ogre::TexturePtr	mLayer;		// ͼ����������
	Ogre::TexturePtr	mLightmap;	// ��Ӱͼ��������
	Ogre::MaterialPtr	mMaterial;	// ��������
	TileArray			mTiles;		// Tile���飬������Ⱦ�õ�tile����
};


#endif	/*__VISION_TERRAIN_H__*/
