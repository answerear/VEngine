/**************************************************************************************************
 * Copyright (C) 2012. Vision Studio.
 * All rights reserved.
 *
 * @file    VGraphicsSystem.h
 * @brief   Brief description.
 * @author  aaronwang
 * @date    2012/2/18 20:47:34
 * @version v1.00
 **************************************************************************************************/

#ifndef __VISION_GRAPHICS_SYSTEM_H__
#define __VISION_GRAPHICS_SYSTEM_H__


#include "VEngineConfig.h"

#include <SdkTrays.h>
#include <SdkCameraMan.h>


class VSceneObjectFactoryManager;
class IRenderEventListener;
class IWindowEventListener;
class VInputSystem;
class VEffectManager;
class VLogicModelManager;


/**
 * @class VGraphicsSystem
 * 渲染系统类，负责启动渲染接口、更新渲染和关闭渲染接口
 */
class VENGINE_DLL VGraphicsSystem 
	: public Ogre::FrameListener
	, public Ogre::WindowEventListener
	, public OgreBites::SdkTrayListener
{
	friend class VCamera;

public:
	/**
	 * @brief Constructor for VGraphicsSystem.
	 */
	VGraphicsSystem();
	
	/**
	 * @brief Destructor for VGraphicsSystem.
	 */
	virtual ~VGraphicsSystem();

private:
	VGraphicsSystem(const VGraphicsSystem &);
	VGraphicsSystem &operator =(const VGraphicsSystem &);

public:
	void startup(const VString &title, VINT32 width, VINT32 height, const VString &cfgFile);
	void shutdown();
	VBOOL run();

	void setupDebugPanel(VInputSystem *system);
	
	VINT32 getRenderWindowID();

	void setRenderEventListener(IRenderEventListener *listener);
	void setWindowEventListener(IWindowEventListener *listener);

	Ogre::SceneManager *getSceneManager()	{ return mSceneMgr; }

	Ogre::Viewport *getViewport()	{ return mViewport; }

	Ogre::SceneNode *getBaseSceneNode()	{ return mBaseSceneNode; }

	Ogre::SceneNode *getStaticSceneNode()	{ return mStaticSceneNode; }

	void setAnimationQuality(Ogre::Real animationQuality);
	Ogre::Real getAnimationQuality() const	{ return mAnimationQuality; }

	VAnimationType determineAnimationType(Ogre::Real level);

protected:
	void _setupResources(const VString &resFile);
	VBOOL _setupRenderConfig(const VString &systemFile);
	void _createRenderWindow(const VString &title, VINT32 width, VINT32 height, VBOOL fullscreen);
	void _createSceneManager();
	void _createCamera();
	void _createViewport();
	void _loadResources();
	void _initScene();
	void _createFrameListener();

	virtual void _registerObjectFactories();
	virtual void _registerMovableObjectFactories();
	
	// From Ogre::FrameListener
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	// From Ogre::WindowEventListener
	virtual void windowMoved(Ogre::RenderWindow* rw);
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual bool windowClosing(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual void windowFocusChange(Ogre::RenderWindow* rw);
	
protected:
	IRenderEventListener	*mRenderEventListener;
	IWindowEventListener	*mWindowEventListener;
	
	Ogre::Root			*mRoot;
	Ogre::RenderWindow	*mRenderWindow;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::Camera		*mCamera;
	Ogre::Viewport		*mViewport;
	
	OgreBites::SdkTrayManager	*mTrayMgr;
    OgreBites::SdkCameraMan		*mCameraMan;		// basic camera controller
    OgreBites::ParamsPanel		*mDetailsPanel;		// sample details panel

	Ogre::SceneNode		*mBaseSceneNode;
	Ogre::SceneNode		*mStaticSceneNode;

	VSceneObjectFactoryManager	*mObjFactoryMgr;

	typedef std::list<Ogre::MovableObjectFactory*> MovableObjectFactoryList;
	MovableObjectFactoryList mMovableObjectFactoryList;

	VEffectManager		*mEffectManager;
	VLogicModelManager	*mLogicModelManager;

	Ogre::Real		mAnimationQuality;
	Ogre::Real		mStartTime;

	VBOOL	mRunning;
};


#endif	/*__VISION_GRAPHICS_SYSTEM_H__*/
