
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

	/** ������Чģ�����ƴ�����Ч���� */
	VEffect *createEffect(const VString &templateName);
	/** ������Ч���� */
	VEffectIterator destroyEffect(VEffect *effect);
	/** �Ƴ���Ч���󣬰���ӻ��Ч�����Ƴ����ŵ����������� */
	void removeEffect(VEffect *effect, VBOOL removeParentNode = VTRUE, VBOOL removeNow = VTRUE);

	/** ������ЧԪ�ص�����������һ����ЧԪ�� */
	VEffectElement *createElement(const VString &type);
	
	/** �������ƻ�ȡ���ܶ��� */
	VSkill *getSkill(const VString &skillName);
	/** ���ݼ���ģ�����ƴ������ܶ��� */
	VSkill *createSkill(const VString &skillName);
	/** ���ټ��ܶ��� */
	void destroySkill(VSkill *skill);

	/** ������Ч���� */
	void setEffectLevel(VEffectLevel level);
	/** ��ȡ��Ч���� */
	VEffectLevel getEffectLevel() const
	{
		return mEffectLevel;
	}

protected:
	/** ������Ч��������������ÿ֡������Ч */
	void _createController();
	/** ɾ����Ч���������� */
	void _destroyController();
	/** �ɿ��������ã�ÿ֡���»����Ч���� */
	void _updateActiveEffects(const Real &dt);

	/** �������ƴ�������ģ����� */
	VSkill *_createSkillTemplate(const VString &name);
	/** �������м���ģ����� */
	void _destroySkillTemplates();

	/** ����ģ�����ƻ�ȡģ�� */
	VEffect *_getEffectTemplate(const VString &name);
	/** �������ƴ�����Чģ����� */
	VEffect *_createEffectTemplate(const VString &name);
	/** ����������Чģ����� */
	void _destroyEffectTemplates();

	/** �ӿ�����Ч���л�ȡһ����ָ��ģ�����Ч */
	VEffect *_getEffectFromFreePool(const VString &templateName);
	/** ��һ����Ч���뵽��Ч���� */
	void _addToFreeEffectPool(VEffect *effect);
	/** ���������Ч�������ж��� */
	void _clearFreeEffectPool();

	/** �����̬��Ч���� */
	void _clearActiveEffectList();

	/** ע����ЧԪ�ع��� */
	void _registerElementFactories();
	/** �����ЧԪ�ع��� */
	void _addElementFactory(VEffectElementFactory *factory);
	/** ���������ЧԪ�ع��� */
	void _clearElementFactories();

	// from Ogre::ScriptLoader
	const Ogre::StringVector& getScriptPatterns(void) const;
	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
	Real getLoadingOrder(void) const;

	/** ������һ���Ҵ����� */
	void _skipToNextCloseBrace(Ogre::DataStreamPtr &stream);
	/** ������һ��������� */
	void _skipToNextOpenBrace(Ogre::DataStreamPtr &stream);

	/** ����*.effect�ļ� */
	void _parseEffectFile(Ogre::DataStreamPtr &stream);
	/** ������ЧԪ�� */
	void _parseElement(const VString &type, Ogre::DataStreamPtr &stream, VEffect *effect);
	/** ������ЧԪ�ص����� */
	void _parseElementAttrib(const VString &line, VEffectElement *element);
	/** ������Ч���� */
	void _parseEffectAttrib(const VString &line, VEffect *effect);

	/** ����*.skill�ļ� */
	void _parseSkillFile(Ogre::DataStreamPtr &stream);
	/** ����skill���� */
	void _parseSkillAttrib(const VString &line, VSkill *skill);
	/** ����animation effect info */
	void _parseAnimEffectInfo(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** ����animation effect info���� */
	void _parseAnimEffectInfoAttrib(const VString &line, VAnimationEffectInfo *effectInfo);
	/** ����animation ribbon */
	void _parseAnimRibbon(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** ����animation ribbon���� */
	void _parseAnimRibbonAttrib(const VString &line, VAnimationRibbon *ribbon);
	/** ����animation scene light */
	void _parseAnimSceneLight(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** ����animation scene light���� */
	void _parseAnimSceneLightAttrib(const VString &line, VAnimationSceneLight *sceneLight);
	/** ����animation sound */
	void _parseAnimSound(Ogre::DataStreamPtr &stream, VSkill *skill);
	/** ����animation sound���� */
	void _parseAnimSoundAttrib(const VString &line, VAnimationSound *sound);

	/** ���������־ */
	void _logErrorInfo(const Ogre::String &errorInfo, const Ogre::String &lineContent, const Ogre::String &functionName);

protected:
	VElementFactoryMap	mElementFactories;	/**< ��ЧԪ�ع������� */
	VEffectMap			mEffectTemplates;	/**< ��Чģ������ */
	VEffectList			mActiveEffects;		/**< ���Ч���� */
	VEffectListMap		mFreeEffects;		/**< ������Ч���� */
	VSkillMap			mSkills;			/**< ������Ч���� */

	Ogre::StringVector	mScriptPatterns;	/**< �ű��ļ����� */

	Ogre::Controller<Ogre::Real>	*mController;	/**< ���ڿ���ÿ֡���µĿ����� */

	VMainSceneLight		*mMainSceneLight;	/**< ��ǰ�����ϵƹ�仯��Ϣ */
	VEffectLevel		mEffectLevel;		/**< ��Ч���� */

	VINT32				mMaxNumFreeEffectPerList;	/**< ������Ч�б�������Ч�� */

	VINT32	mParsingLineNumber;
	VString	mParsingFilename;
};


#define VEFFECT_MANAGER	(VEffectManager::instance())


#endif	/*__VISION_EFFECT_MANAGER_H__*/
