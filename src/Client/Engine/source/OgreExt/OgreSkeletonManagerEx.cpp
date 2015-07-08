
#include "OgreSkeletonManagerEx.h"
#include "OgreSkeletonEx.h"


namespace Ogre
{
	SkeletonManagerEx::SkeletonManagerEx()
	{

	}
	//-----------------------------------------------------------------------
	Resource* SkeletonManagerEx::createImpl(const String& name, ResourceHandle handle, 
		const String& group, bool isManual, ManualResourceLoader* loader, 
		const NameValuePairList* createParams)
	{
		return OGRE_NEW SkeletonEx(this, name, handle, group, isManual, loader);
	}
}
