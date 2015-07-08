

#include "VGraphicsSystem.h"
#include "VInputSystem.h"
#include "IRenderEventListener.h"
#include "IWindowEventListener.h"

#include "VSceneObjectFactoryManager.h"
#include "VEffectManager.h"
#include "VLogicModelManager.h"

#include "VActorObject.h"
#include "VEffectObject.h"
#include "VEnvironmentObject.h"
#include "VLightObject.h"
#include "VModelObject.h"
#include "VParticleSystemObject.h"
#include "VSkyBoxObject.h"
#include "VSkyDomeObject.h"
#include "VSkyPlaneObject.h"
#include "VStaticEntityObject.h"
#include "VTerrainLiquidObject.h"

#include "OgreAutoAnimationEntity.h"
#include "OgreSkeletonManagerEx.h"

#include "VDateTime.h"


static VString BOOTSTRAP_RESOURCE_GROUP_NAME = "Bootstrap";


VGraphicsSystem::VGraphicsSystem()
	: mRenderEventListener(VNULL)
	, mWindowEventListener(VNULL)
	, mRoot(VNULL)
	, mRenderWindow(VNULL)
	, mSceneMgr(VNULL)
	, mCamera(VNULL)
	, mViewport(VNULL)
	, mTrayMgr(VNULL)
	, mCameraMan(VNULL)
	, mDetailsPanel(VNULL)
	, mEffectManager(VNULL)
	, mLogicModelManager(VNULL)
	, mAnimationQuality(50)
{
	
}

VGraphicsSystem::~VGraphicsSystem()
{
	shutdown();
}


void VGraphicsSystem::startup(const VString &title, VINT32 width, VINT32 height, const VString &cfgFile)
{
	Ogre::ConfigFile cf;
	cf.load(cfgFile);

	// 读取各种其他配置文件
	Ogre::String pluginFile = cf.getSetting("PluginsConfig", "ConfigFiles", "plugins.ini");
	Ogre::String resourcesFile = cf.getSetting("ResourcesConfig", "ConfigFiles", "resources.ini");
	Ogre::String systemFile = cf.getSetting("SettingsConfig", "ConfigFiles", "settings.ini");

	// 创建OGRE Root对象，启动OGRE
	mRoot = new Ogre::Root(pluginFile);

	// 为了正确加载天龙八部的骨骼文件（指针操作有疑惑）
	Ogre::SkeletonManager* skeletonManager = Ogre::SkeletonManager::getSingletonPtr();
	OGRE_DELETE skeletonManager;
	OGRE_NEW Ogre::SkeletonManagerEx();

	_registerMovableObjectFactories();

	mObjFactoryMgr = new VSceneObjectFactoryManager();
	_registerObjectFactories();

	_setupResources(resourcesFile);
	VBOOL fullscreen = _setupRenderConfig(systemFile);
	_createRenderWindow(title, width, height, fullscreen);
	_createSceneManager();
	_createCamera();
	_createViewport();

	mEffectManager = new VEffectManager();
	mLogicModelManager = new VLogicModelManager();

	_loadResources();
	_initScene();
	_createFrameListener();

	mStartTime = Real(VDateTime::currentMSecsSinceEpoch() * 0.001);
}

void VGraphicsSystem::shutdown()
{
	delete mLogicModelManager;
	mLogicModelManager = VNULL;

	delete mEffectManager;
	mEffectManager = VNULL;

	delete mObjFactoryMgr;
	mObjFactoryMgr = VNULL;

	delete mRoot;
	mRoot = VNULL;
}

VBOOL VGraphicsSystem::run()
{
	if (!mRoot || !mRoot->getRenderSystem())
		return VFALSE;

	mRunning = VTRUE;
	mRoot->startRendering();
	return VTRUE;
}

void VGraphicsSystem::setupDebugPanel(VInputSystem *system)
{
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mRenderWindow, system->mMouse, this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();

	// create a params panel for displaying sample details
	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();
}

VINT32 VGraphicsSystem::getRenderWindowID()
{
	VINT32 wndID = 0;

	if (mRenderWindow != VNULL)
	{
		mRenderWindow->getCustomAttribute("WINDOW", &wndID);
	}

	return wndID;
}

void VGraphicsSystem::setRenderEventListener(IRenderEventListener *listener)
{
	mRenderEventListener = listener;
}

void VGraphicsSystem::setWindowEventListener(IWindowEventListener *listener)
{
	mWindowEventListener = listener;
}

void VGraphicsSystem::setAnimationQuality(Ogre::Real animationQuality)
{
	if (mAnimationQuality != animationQuality)
	{
		mAnimationQuality = animationQuality;
	}
}

VAnimationType VGraphicsSystem::determineAnimationType(Ogre::Real level)
{
	level += mAnimationQuality;
	if (level >= 100)
		return AT_FULL;
	else if (level >= 50)
		return AT_FROZE;
	else
		return AT_DISABLE;
}


// From Ogre::FrameListener
bool VGraphicsSystem::frameStarted(const Ogre::FrameEvent& evt)
{
	Real dt = evt.timeSinceLastFrame;
	if (mRenderEventListener != VNULL)
	{
		Real time = Real(VDateTime::currentMSecsSinceEpoch() * 0.001) - mStartTime;
		mRenderEventListener->update(time, dt);
		mRenderEventListener->preRender(time, dt);
	}

	return mRunning;
}

bool VGraphicsSystem::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mTrayMgr->frameRenderingQueued(evt);
	
    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }
	
	return mRunning;
}

bool VGraphicsSystem::frameEnded(const Ogre::FrameEvent& evt)
{
	Real dt = evt.timeSinceLastFrame;
	if (mRenderEventListener != VNULL)
	{
		Real time = Real(VDateTime::currentMSecsSinceEpoch() * 0.001) - mStartTime;
		mRenderEventListener->postRender(time, dt);
	}

	return mRunning;
}

// From Ogre::WindowEventListener
void VGraphicsSystem::windowMoved(Ogre::RenderWindow* rw)
{
	if (mWindowEventListener != VNULL)
	{
		VINT32 x, y;
		VUINT32 w, h;
		VUINT32 d;
		rw->getMetrics(w, h, d, x, y);
 		mWindowEventListener->windowMoved(x, y);
	}
}

void VGraphicsSystem::windowResized(Ogre::RenderWindow* rw)
{
	if (mWindowEventListener != VNULL)
	{
		VUINT32 w = rw->getWidth();
		VUINT32 h = rw->getHeight();
		mWindowEventListener->windowResized(w, h);
	}
}

bool VGraphicsSystem::windowClosing(Ogre::RenderWindow* rw)
{
	bool result = Ogre::WindowEventListener::windowClosing(rw);
	if (mWindowEventListener != VNULL)
	{
		result = mWindowEventListener->windowClosing();
	}
	return result;
}

void VGraphicsSystem::windowClosed(Ogre::RenderWindow* rw)
{
	if (mWindowEventListener != VNULL)
	{
		mRunning = VFALSE;
		mWindowEventListener->windowClosed();
	}
}

void VGraphicsSystem::windowFocusChange(Ogre::RenderWindow* rw)
{
	if (mWindowEventListener != VNULL)
	{
		mWindowEventListener->windowFocusChange();
	}
}


void VGraphicsSystem::_setupResources(const VString &resFile)
{
	// 读取资源配置文件，添加资源路径
	Ogre::ConfigFile cf;
	cf.load(resFile);

	Ogre::ConfigFile::SectionIterator secItr = cf.getSectionIterator();
	Ogre::String sectionName, typeName, archName;

	while (secItr.hasMoreElements())
	{
		sectionName = secItr.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = secItr.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator itr;
		for (itr = settings->begin(); itr != settings->end(); ++itr)
		{
			typeName = itr->first;
			archName = itr->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, sectionName);
		}
	}
}

VBOOL VGraphicsSystem::_setupRenderConfig(const VString &systemFile)
{
	Ogre::ConfigFile cf;
	cf.load(systemFile);

	Ogre::ConfigFile::SectionIterator secItr = cf.getSectionIterator();
	Ogre::String value;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	value = cf.getSetting("RenderSystem", "Render", "Direct3D9 Rendering Subsystem");
#else
	value = cf.getSetting("RenderSystem", "Render", "OpenGL Rendering Subsystem");
#endif

	Ogre::RenderSystemList::const_iterator itrRender = mRoot->getAvailableRenderers().begin();
	while (itrRender != mRoot->getAvailableRenderers().end())
	{
		const Ogre::String &renderName = (*itrRender)->getName();
		if (renderName == value)
			break;
		++itrRender;
	}

	Ogre::RenderSystem *renderSystem = *itrRender;

	// 全屏抗锯齿度
	value = cf.getSetting("Antialiasing", "Render", "None");
	renderSystem->setConfigOption("FSAA", value);

	// 是否垂直同步
	value = cf.getSetting("VSync", "Render", "No");
	renderSystem->setConfigOption("VSync", value);

	// 是否全屏
	value = cf.getSetting("FullScreen", "Render", "No");
	renderSystem->setConfigOption("Full Screen", value);

	bool fullscreen;
	if ("No" == value)
	{
		fullscreen = false;
	}
	else
	{
		fullscreen = true;
	}

	mRoot->setRenderSystem(renderSystem);

	// 初始化OGRE
	mRoot->initialise(false);

	return fullscreen;
}

void VGraphicsSystem::_createRenderWindow(const VString &title, VINT32 width, VINT32 height, VBOOL fullscreen)
{
	// 关联渲染窗口
	Ogre::RenderWindow *renderWindow = NULL;

	Ogre::NameValuePairList params;

//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
//	params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)wndID);
//#endif

//	renderWindow = mRoot->createRenderWindow("Render Window", width, height, fullscreen, &params);
	renderWindow = mRoot->createRenderWindow(title, width, height, fullscreen, &params);
	
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	renderWindow->windowMovedOrResized();
	//resizeEvent(0);
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	renderWindow->resize(width(), height());
#endif

	mRenderWindow = renderWindow;
}

void VGraphicsSystem::_createSceneManager()
{
	// 创建场景管理器
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "VisionSceneManager");
}

void VGraphicsSystem::_createCamera()
{
	// 创建相机
	mCamera = mSceneMgr->createCamera("Main");
	mCamera->setPosition(Ogre::Vector3(750, 800, 800));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(100.0f);//LM修改
	mCamera->setFarClipDistance(10000.0f);
	// 45 degree fov
	mCamera->setFOVy(Ogre::Degree(45));
	// Auto aspect ratio
	mCamera->setAutoAspectRatio(true);
// 	mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
}

void VGraphicsSystem::_createViewport()
{
	// 创建视口
	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue::Black);
	if (!mCamera->getAutoAspectRatio())
	{
		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	}
}

void VGraphicsSystem::_loadResources()
{
	// 加载资源
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(BOOTSTRAP_RESOURCE_GROUP_NAME);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void VGraphicsSystem::_initScene()
{
	mBaseSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("#MainSceneNode");
	mStaticSceneNode = mBaseSceneNode->createChildSceneNode("#StaticSceneNode");
}

void VGraphicsSystem::_createFrameListener()
{
	windowResized(mRenderWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);

	mRoot->addFrameListener(this);
}

void VGraphicsSystem::_registerObjectFactories()
{
	mObjFactoryMgr->registerFactory(VActorObject::getFactory());
	mObjFactoryMgr->registerFactory(VEffectObject::getFactory());
	mObjFactoryMgr->registerFactory(VEnvironmentObject::getFactory());
	mObjFactoryMgr->registerFactory(VLightObject::getFactory());
	mObjFactoryMgr->registerFactory(VModelObject::getFactory());
	mObjFactoryMgr->registerFactory(VParticleSystemObject::getFactory());
	mObjFactoryMgr->registerFactory(VSkyBoxObject::getFactory());
	mObjFactoryMgr->registerFactory(VSkyDomeObject::getFactory());
	mObjFactoryMgr->registerFactory(VSkyPlaneObject::getFactory());
	mObjFactoryMgr->registerFactory(VStaticEntityObject::getFactory());
	mObjFactoryMgr->registerFactory(VTerrainLiquidObject::getFactory());
}

void VGraphicsSystem::_registerMovableObjectFactories()
{
	mMovableObjectFactoryList.push_back(new Ogre::AutoAnimationEntityFactory);
	
	// TODO: new movable object factory register here

	// put into ogre core
	for (MovableObjectFactoryList::const_iterator it = mMovableObjectFactoryList.begin(); it != mMovableObjectFactoryList.end(); ++it)
	{
		Ogre::MovableObjectFactory* factory = *it;
		mRoot->addMovableObjectFactory(factory);
	}
}
