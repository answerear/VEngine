

#ifndef __OGRE_SKELETON_MANAGER_EX_H__
#define __OGRE_SKELETON_MANAGER_EX_H__

#include <OgreSkeletonManager.h>


namespace Ogre
{
	class SkeletonManagerEx : public SkeletonManager
	{
	public:
		SkeletonManagerEx();

	protected:
		/// @copydoc ResourceManager::createImpl
		Resource* createImpl(const String& name, ResourceHandle handle, 
			const String& group, bool isManual, ManualResourceLoader* loader, 
			const NameValuePairList* createParams);
	};
}


#endif	/*__OGRE_SKELETON_MANAGER_EX_H__*/
