

#include "VTerrainInfo.h"
#include "VisionXMLParser.h"
#include "VisionXMLWriter.h"
#include "VTerrainSerializer.h"


VTerrainInfo::VTerrainInfo()
	: mXSize(0)
	, mZSize(0)
	, mTileSize(0)
	, mScale(Ogre::Vector3::UNIT_SCALE)
	, mPosition(Ogre::Vector3::ZERO)
	, mName()
	, mHeightmapFilename()
	, mHeightmapType()
	, mGridInfoFilename()
	, mGridInfoType()
	, mLightmapFilename()
	, mLightmapType()
	, mPixmaps()
	, mTextures()
	, mMaterialTemplates()
	, mFogReplacements()
{
	
}

VTerrainInfo::~VTerrainInfo()
{

}

void VTerrainInfo::reset()
{
	mXSize = 0;
	mZSize = 0;
	mTileSize = 0;
	mScale = Ogre::Vector3::UNIT_SCALE;
	mPosition = Ogre::Vector3::ZERO;
	mName.clear();
	mHeightmapFilename.clear();
	mHeightmapType.clear();
	mGridInfoFilename.clear();
	mGridInfoType.clear();
	mLightmapFilename.clear();
	mLightmapType.clear();
	mPixmaps.clear();
	mTextures.clear();
	mMaterialTemplates.clear();
	mFogReplacements.clear();
}
