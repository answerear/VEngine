

#include "OgreSkeletonSerializerEx.h"

#include <OgreAnimation.h>
#include <OgreBone.h>
#include <OgreSkeletonFileFormat.h>


namespace Ogre
{
#define SKELETON_ANIMATION_TRACK_KEYFRAME_LIST		0x4120

	const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

	SkeletonSerializerEx::SkeletonSerializerEx(void)
	{
	}

	SkeletonSerializerEx::~SkeletonSerializerEx(void)
	{
	}
	//---------------------------------------------------------------------
	void SkeletonSerializerEx::importSkeleton(DataStreamPtr& stream, Skeleton* pSkel)
	{
		// Determine endianness (must be the first thing we do!)
		determineEndianness(stream);

		// Check header
		readFileHeader(stream);

		unsigned short streamID;
		while(!stream->eof())
		{
			streamID = readChunk(stream);
			switch (streamID)
			{
			case SKELETON_BONE:
				readBone(stream, pSkel);
				break;
			case SKELETON_BONE_PARENT:
				readBoneParent(stream, pSkel);
				break;
			case SKELETON_ANIMATION:
				readAnimation(stream, pSkel);
				break;
			case SKELETON_ANIMATION_LINK:
				readSkeletonAnimationLink(stream, pSkel);
				break;
			}
		}

		// Assume bones are stored in the binding pose
		pSkel->setBindingPose();


	}

	//---------------------------------------------------------------------
	void SkeletonSerializerEx::readAnimation(DataStreamPtr& stream, Skeleton* pSkel)
	{
		// char* name                       : Name of the animation
		String name;
		name = readString(stream);
		// float length                      : Length of the animation in seconds
		float len;
		readFloats(stream, &len, 1);

		Animation *pAnim = pSkel->createAnimation(name, len);

		// Read all tracks
		if (!stream->eof())
		{
			unsigned short streamID = readChunk(stream);
			while(streamID == SKELETON_ANIMATION_TRACK && !stream->eof())
			{
				readAnimationTrack(stream, pAnim, pSkel);

				if (!stream->eof())
				{
					// Get next stream
					streamID = readChunk(stream);
				}
			}
			if (!stream->eof())
			{
				// Backpedal back to start of this stream if we've found a non-track
				stream->skip(-STREAM_OVERHEAD_SIZE);
			}
		}
	}
	//---------------------------------------------------------------------
	void SkeletonSerializerEx::readAnimationTrack(DataStreamPtr& stream, Animation* anim, 
		Skeleton* pSkel)
	{
		// unsigned short boneIndex     : Index of bone to apply to
		unsigned short boneHandle;
		readShorts(stream, &boneHandle, 1);

		// Find bone
		Bone *targetBone = pSkel->getBone(boneHandle);

		// Create track
		NodeAnimationTrack* pTrack = anim->createNodeTrack(boneHandle, targetBone);

		// Keep looking for nested keyframes
		if (!stream->eof())
		{
			unsigned short streamID = readChunk(stream);
			while((streamID == SKELETON_ANIMATION_TRACK_KEYFRAME 
				|| streamID == SKELETON_ANIMATION_TRACK_KEYFRAME_LIST)
				&& !stream->eof())
			{
				if (streamID == SKELETON_ANIMATION_TRACK_KEYFRAME_LIST)	// 新增代码
				{			
					unsigned short len;
					unsigned short flags;
					readShorts(stream, &len, 1);
					readShorts(stream, &flags, 1);

					float time;
					for (int i = 0; i < len; i += 1)
					{
						readFloats(stream, &time, 1);
						TransformKeyFrame *kf = pTrack->createNodeKeyFrame(time);

						Quaternion rot = Quaternion::IDENTITY;
						if (flags & 1)
						{
							readObject(stream, rot);
						}
						kf->setRotation(rot);

						Vector3 trans = Vector3::ZERO;
						if (flags & 2)
						{
							readObject(stream, trans);
						}
						kf->setTranslate(trans);

						//网上流行的解析代码，少了这部分，导致有些动画解析不正确，比如柳枝的摆动
						//很多场景中的StaticEntity都包含柳树的骨骼动画                        
						Vector3 scale = Vector3::UNIT_SCALE;
						if (flags & 4)
						{
							readObject(stream, scale);
						}
						kf->setScale(scale);
					}
				}
				else
					readKeyFrame(stream, pTrack, pSkel);

				if (!stream->eof())
				{
					// Get next stream
					streamID = readChunk(stream);
				}
			}
			if (!stream->eof())
			{
				// Backpedal back to start of this stream if we've found a non-keyframe
				stream->skip(-STREAM_OVERHEAD_SIZE);
			}
		}
	}
}
