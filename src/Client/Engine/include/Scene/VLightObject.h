

#ifndef __VISION_LIGHT_OBJECT_H__
#define __VISION_LIGHT_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"


class VLightObject : public VSceneObject
{
	friend class VLightObjectFactory;
	friend class VLightTypeCmd;

	template <Ogre::Vector3 VLightObject::*PtrToMember> friend class VLightVector3MemberCommand;
	template <Ogre::ColourValue VLightObject::*PtrToMember> friend class VLightColorRGBMemberCommand;
	template <class Class, typename Type, Type Class::*PtrToMember> friend class VObjectMemberPropertyCommand;

public:
	VLightObject();
	virtual ~VLightObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	void updateRenderInstance();

	static VSceneObjectFactory *getFactory();

protected:
	void _createLight();
	void _destroyLight();
	void _updateRenderInstanceImp();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneNode		*mBaseSceneNode;
	Ogre::SceneNode		*mSceneNode;
	Ogre::Light			*mLight;

	Ogre::Light::LightTypes	mLightType;

	Ogre::ColourValue	mDiffuse;
	Ogre::ColourValue	mSpecular;

	Ogre::Vector3	mPosition;
	Ogre::Vector3	mDirection;

	Ogre::Real	mRange;
	Ogre::Real	mAttenuationConstant;
	Ogre::Real	mAttenuationLinear;
	Ogre::Real	mAttenuationQuadric;
	Ogre::Real	mSpotOuterAngle;
	Ogre::Real	mSpotInnerAngle;
	Ogre::Real	mSpotFalloff;

	VBOOL	mCastShadows;
	VBOOL	mStaticLight;
};


#endif	/*__VISION_LIGHT_OBJECT_H__*/
