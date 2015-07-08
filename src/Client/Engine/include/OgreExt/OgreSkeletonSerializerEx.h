

#ifndef __OGRE_SKELETON_SERIALIZER_EX_H__
#define __OGRE_SKELETON_SERIALIZER_EX_H__


#include <OgreSkeletonSerializer.h>


namespace Ogre
{
	class SkeletonSerializerEx : public SkeletonSerializer
	{
	public:
		SkeletonSerializerEx();
		~SkeletonSerializerEx();

		void importSkeleton(DataStreamPtr& stream, Skeleton* pDest);

	protected:
		void readAnimation(DataStreamPtr& stream, Skeleton* pSkel);
		void readAnimationTrack(DataStreamPtr& stream, Animation* anim, Skeleton* pSkel);
	};
}


#endif	/*__OGRE_SKELETON_SERIALIZER_EX_H__*/
