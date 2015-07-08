

#include "OgreSkeletonEx.h"
#include "OgreSkeletonSerializerEx.h"

#include <OgreLogManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreSkeletonManager.h>


namespace Ogre
{
	SkeletonEx::SkeletonEx(ResourceManager* creator, const String& name, ResourceHandle handle,
		const String& group, bool isManual, ManualResourceLoader* loader)
		:Skeleton(creator, name, handle, group, isManual, loader)
	{
	}
	//---------------------------------------------------------------------
	void SkeletonEx::loadImpl(void)
	{
		SkeletonSerializerEx serializer;	// 这里改了
		LogManager::getSingleton().stream()
			<< "Skeleton: Loading " << mName;

		DataStreamPtr stream = 
			ResourceGroupManager::getSingleton().openResource(
			mName, mGroup, true, this);

		serializer.importSkeleton(stream, this);

		// Load any linked skeletons
		LinkedSkeletonAnimSourceList::iterator i;
		for (i = mLinkedSkeletonAnimSourceList.begin(); 
			i != mLinkedSkeletonAnimSourceList.end(); ++i)
		{
			i->pSkeleton = SkeletonManager::getSingleton().load(
				i->skeletonName, mGroup);
		}
	}
}
