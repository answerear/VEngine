

#include "VSceneInfo.h"


VSceneInfo::VSceneInfo()
{

}

VSceneInfo::~VSceneInfo()
{

}

void VSceneInfo::reset()
{
	mName.clear();
	mAuthor.clear();
	mCopyright.clear();
	mDescription.clear();
	mTerrainFilename.clear();
	mTerrainMinHeight = 0;
	mTerrainMaxHeight = 0;
}
