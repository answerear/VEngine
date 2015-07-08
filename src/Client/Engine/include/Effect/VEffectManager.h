
#ifndef __VISION_EFFECT_MANAGER_H__
#define __VISION_EFFECT_MANAGER_H__


#include "VEngineConfig.h"
#include "VSingleton.h"


class VSkill;
class VAnimationEffectInfo;
class VAnimationRibbon;
class VAnimationSceneLight;
class VAnimationSound;

class VEffect;
class VEffectElement;
class VEffectElementFactory;

class VMainSceneLight;


enum VEffectLevel
{
	VEL_LOW,
	VEL_MID,
	VEL_HIGH
};

class VEffectManager 
	: public VSingleton<VEffectManager>
	, public Ogre::ScriptLoader
{
	friend class VEffectControllerValue;

public:
	typedef std::map<VString, VEffect*> VEffectMap;
	typedef std::list<VEffect*> VEffectList;
	typedef std::map<VString, VEffectList*> VEffectListMap;
	typedef std::map<VString, VEffectElementFactory*> VElementFactoryMap;
	typedef std::map<VString, VSkill*> VSkillMap;
// 	typedef std::vector<VString> VLoadedScripts;
	typedef VEffectList::iterator VEffectIterator;

public:
	VEffectManager();
	~VEffectManager();

	/** 根据特效模板名称创建特效对象 */
	VEffect *createEffect(const VString &templateName);
	/** 销毁特效对象 */
	VEffectIterator destroyEffect(VEffect *effect);
	/** 移除特效对象，把其从活动特效链表移除，放到空闲链表中 */
	void removeEffect(VEffect *effect, VBOOL removeParentNode = VTRUE, VBOOL removeNow = VTRUE);

	/** 根据特效元素的类型来创建一个特效元素 */
	VEffectElement *createElement(const VString &type);
	
	/** 根据名称获取技能对象 */
	VSkill *getSkill(const VString &skillName);
	/** 根据技能模板名称创建技能对象 */
	VSkill *createSkill(const VString &skillName);
	/** 销毁技能对象 */
	void destroySkill(VSkill *skill);

	/** 设置特效级别 */
	void setEffectLevel(VEffectLevel level);
	/** 获取特效级别 */
	VEffectLevel getEffectLevel() const
	{
		return mEffectLevel;
	}

protected:
	/** 创建特效控制器对象，用于每帧更新特效 */
	void _createController();
	/** 删除特效控制器对象 */
	void _destroyController();
	/** 由控制器调用，每帧更新活动的特效对象 */
	void _updateActiveEffects(const Real &dt);

	/** 根据名称创建技能模板对象 */
	VSkill *_createSkillTemplate(const VString &name);
	/** 销毁所有技能模板对象 */
	void _destroySkillTemplates();

	/** 根据模板名称获取模板 */
	VEffect *_getEffectTemplate(const VString &name);
	/** 根据名称创建特效模板对象 */
	VEffect *_createEffectTemplate(const VString &name);
	/** 销毁所有特效模板对象 */
	void _destroyEffectTemplates();

	/** 从空闲特效池中获取一个所指定模板的特效 */
	VEffect *_getEffectFromFreePool(const VString &templateName);
	/** 把一个特效加入到特效池中 */
	void _addToFreeEffectPool(VEffect *effect);
	/** 清除空闲特效池中所有对象 */
	void _clearFreeEffectPool();

	/** 清除动态特效链表 */
	void _clearActiveEffectList();

	/** 注册特效元素工厂 */
	void _registerElementFactories();
	/** 添加特效元素工厂 */
	void _addElementFactory(VEffectElementFactory *factory);
	/** 清除所有特效元素工厂 */
	void _clearElementFactories();

	// from Ogre::ScriptLoader
	const Ogre::StringVector& getScriptPatterns(void) const;
	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
	Real getLoadingOrder(void) const;

	/** 跳到下一个右大括号 */
	void _skipToNextCloseBrace(Ogre::DataStreamPtr &stream);
	/** 跳到下一个左大括号 */
	void _skipToNextOpenBrace(Ogre::DataStreamPtr &stream);

	/** 解析*.effect文件 */
	void _parseEffectFile(Ogre::DataStreamPtr &stream);
	/** 解析特效元素 */
	void _parseElement(const VString &type, Ogre::DataStreamPtr &stream, VEffect *effect);
	/** 解析特效元素的属性 */
	void _parseElementAttrib(const VString &line, VEffectElement *element);
	/** 解析特效属性 */
	void _parseEffectAttrib(const VString &line, VEffect *effect);

	/** 解析*.skill文件 */
	void _parseSkillFile(Ogre::DataStreamPtr &stream);
	/** 解析skill属性 */
	void _parseSkillAttrib(const VString &line, VSkill *skill);
	/** 解析animation effect info */
	void _parseAnimEffectInfo(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** 解析animation effect info属性 */
	void _parseAnimEffectInfoAttrib(const VString &line, VAnimationEffectInfo *effectInfo);
	/** 解析animation ribbon */
	void _parseAnimRibbon(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** 解析animation ribbon属性 */
	void _parseAnimRibbonAttrib(const VString &line, VAnimationRibbon *ribbon);
	/** 解析animation scene light */
	void _parseAnimSceneLight(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** 解析animation scene light属性 */
	void _parseAnimSceneLightAttrib(const VString &line, VAnimationSceneLight *sceneLight);
	/** 解析animation sound */
	void _parseAnimSound(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** 解析animation sound属性 */
	void _parseAnimSoundAttrib(const VString &line, VAnimationSound *sound);

	/** 输出错误日志 */
	void _logErrorInfo(const Ogre::String &errorInfo, const Ogre::String &lineContent, const Ogre::String &functionName);

protected:
	VElementFactoryMap	mElementFactories;	/**< 特效元素工厂容器 */
	VEffectMap			mEffectTemplates;	/**< 特效模版容器 */
	VEffectList			mActiveEffects;		/**< 活动特效容器 */
	VEffectListMap		mFreeEffects;		/**< 空闲特效容器 */
	VSkillMap			mSkills;			/**< 技能特效容器 */

	Ogre::StringVector	mScriptPatterns;	/**< 脚本文件类型 */

	Ogre::Controller<Ogre::Real>	*mController;	/**< 用于控制每帧更新的控制器 */

	VMainSceneLight		*mMainSceneLight;	/**< 当前场景上灯光变化信息 */
	VEffectLevel		mEffectLevel;		/**< 特效级别 */

	VINT32				mMaxNumFreeEffectPerList;	/**< 空闲特效列表最大的特效数 */

	VINT32	mParsingLineNumber;
	VString	mParsingFilename;
};


#define VEFFECT_MANAGER	(VEffectManager::instance())


#endif	/*__VISION_EFFECT_MANAGER_H__*/
