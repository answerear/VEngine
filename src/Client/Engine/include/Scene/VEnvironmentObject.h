

#ifndef __VISION_ENVIRONMENT_OBJECT_H__
#define __VISION_ENVIRONMENT_OBJECT_H__


#include "VSceneObject.h"
#include "VSceneObjectFactory.h"



class VEnvironmentObject : public VSceneObject
{
	friend class VEnvironmentObjectFactory;
	friend class VFogModeCmd;
	friend class VRenderWeatherCmd;

	template <ColorValue VEnvironmentObject::*PtrToMember> friend class VEnvColorRGBMemberCommand;
	template <class Class, typename Type, Type Class::*PtrToMember> friend class VObjectMemberPropertyCommand;

public:
	VEnvironmentObject();
	virtual ~VEnvironmentObject();

	const VString &getType() const;
	const VString &getCategory() const;

	void createRenderInstance();
	void destroyRenderInstance();

	void updateRenderInstance();

	void enableRenderWeather(VBOOL renderWeather);

	VBOOL isRenderWeather() const
	{
		return mRenderWeather;
	}

	static VSceneObjectFactory *getFactory();

protected:
	void _updateRenderInstanceImp();

protected:
	static const VString msType;
	static const VString msCategory;

	Ogre::SceneManager	*mSceneMgr;
	Ogre::Viewport		*mViewport;

	VString	mWeatherParticleSystemName;

	Ogre::ColourValue	mAmbientLight;
	Ogre::ColourValue	mBackgroundColor;
	Ogre::ColourValue	mFogColor;

	Ogre::FogMode	mFogMode;

	Ogre::Real	mFogExpDensity;
	Ogre::Real	mFogLinearStart;
	Ogre::Real	mFogLinearEnd;

	VBOOL	mRenderWeather;
};


#endif	/*__VISION_ENVIRONMENT_OBJECT_H__*/
