

#ifndef __OGRE_SKELETON_EX_H__
#define __OGRE_SKELETON_EX_H__


#include <OgreSkeleton.h>


namespace Ogre
{
	class SkeletonEx : public Skeleton
	{
	public:
		SkeletonEx(ResourceManager* creator, const String& name, ResourceHandle handle,
 		const String& group, bool isManual = false, ManualResourceLoader* loader = 0);

	protected:
		/** @copydoc Resource::loadImpl
		*/
		void loadImpl(void);
	};
}


#endif	/*__OGRE_SKELETON_EX_H__*/
