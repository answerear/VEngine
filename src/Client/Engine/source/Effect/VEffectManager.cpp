

#include "VEngine.h"
#include "VGraphicsSystem.h"
#include "VEffectManager.h"

#include "VMainSceneLight.h"

#include "VEffect.h"
#include "VEffectElement.h"
#include "VEffectElementFactory.h"

#include "VSkill.h"
#include "VAnimationEffectInfo.h"
#include "VAnimationRibbon.h"
#include "VAnimationSceneLight.h"
#include "VAnimationSound.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// class VEffectControllerValue
////////////////////////////////////////////////////////////////////////////////////////////////////
class VEffectControllerValue : public Ogre::ControllerValue<VFLOAT>
{
protected:
	VEffectManager *mEffectMgr;

public:
	VEffectControllerValue(VEffectManager *effectMgr)
		: mEffectMgr(effectMgr)
	{
	}

	VFLOAT getValue() const
	{
		return 0.0f;
	}

	void setValue(VFLOAT value)
	{
		mEffectMgr->_updateActiveEffects(value);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// class VEffectManager
////////////////////////////////////////////////////////////////////////////////////////////////////

template<> VEffectManager *VSingleton<VEffectManager>::mInstance = VNULL;

VEffectManager::VEffectManager()
	: mController(VNULL)
	, mMainSceneLight(VNULL)
	, mEffectLevel(VEL_HIGH)
	, mMaxNumFreeEffectPerList(10)
	, mParsingLineNumber(0)
	, mParsingFilename("")
{
	mScriptPatterns.push_back("*.effect");
	mScriptPatterns.push_back("*.skill");

	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	mElementFactories.clear();
	mEffectTemplates.clear();
	mActiveEffects.clear();
	mFreeEffects.clear();
	mSkills.clear();

	_registerElementFactories();
	_createController();

	mMainSceneLight = new VMainSceneLight();
}

VEffectManager::~VEffectManager()
{
	delete mMainSceneLight;
	mMainSceneLight = VNULL;

	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);

	_clearActiveEffectList();
	_clearFreeEffectPool();
	_destroyEffectTemplates();
	_destroySkillTemplates();
	_clearElementFactories();
	_destroyController();
}

VEffect *VEffectManager::createEffect(const VString &templateName)
{
	// 首先从空闲特效池中获取
	VEffect *effect = VNULL;
	effect = _getEffectFromFreePool(templateName);

	// 如果没有获取到就新建一个
	if (VNULL == effect)
	{
		VEffect *tempEffect = _getEffectTemplate(templateName);
		if (VNULL == tempEffect)
		{
			// 没有找到特效模板，报错
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot find required template '"
				"Effect_Error'",
				"VEffectManager::createEffect");
		}

		effect = new VEffect(templateName);
		*effect = *tempEffect;
	}

	assert(effect != VNULL);
	mActiveEffects.push_back(effect);

	return effect;
}

VEffectManager::VEffectIterator VEffectManager::destroyEffect(VEffect *effect)
{
	assert(effect != VNULL);

	VEffectIterator itr = mActiveEffects.begin();

	while (itr != mActiveEffects.end())
	{
		if (*itr == effect)
		{
			delete *itr;
			*itr = VNULL;

			itr = mActiveEffects.erase(itr);
			break;
		}

		++itr;
	}

	return itr;
}

void VEffectManager::removeEffect(VEffect *effect, VBOOL removeParentNode /* = VTRUE */, VBOOL removeNow /* = VTRUE */)
{
	assert(effect != VNULL);

	effect->setRemoveParentNodeFlag(removeParentNode);

	if (removeNow)
	{
		VEffectIterator itr = mActiveEffects.begin();
		while (itr != mActiveEffects.end())
		{
			if (*itr == effect)
			{
				// 让所有元素先休眠
				effect->removeAllElements();

				if (effect->getRemoveParentNodeFlag())
				{
					effect->removeFromParentSceneNode();
				}

				// 从活动链表中删除，并且加入到空闲池中
				mActiveEffects.erase(itr);
				_addToFreeEffectPool(effect);
				break;
			}
			++itr;
		}
	}
	else
	{
		// 关闭特效，设置延迟删除标记
		effect->shutdown();
		effect->setDeleteLaterFlag(VTRUE);
	}
}

VEffectElement *VEffectManager::createElement(const VString &type)
{
	VElementFactoryMap::const_iterator itr = mElementFactories.find(type);

	if (itr == mElementFactories.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
			"the element type '" + type + "' didn't exist!",
			"VEffectManager::createElement");
	}

	VEffectElementFactory *factory = itr->second;
	assert(factory != VNULL);

	VEffectElement *element = factory->createElement();
	assert(element != VNULL);
	return element;
}

VSkill *VEffectManager::getSkill(const VString &skillName)
{
	VSkillMap::const_iterator itr = mSkills.find(skillName);
	VSkill *skill = VNULL;

	if (itr != mSkills.end())
	{
		skill = itr->second;
	}
	
	return skill;
}

VSkill *VEffectManager::createSkill(const VString &skillName)
{
	VSkill *tempSkill = getSkill(skillName);

	VSkill *skill = VNULL;
	if (tempSkill != VNULL)
	{
		skill = new VSkill(skillName);
		*skill = *tempSkill;
	}

	return skill;
}

void VEffectManager::destroySkill(VSkill *skill)
{
	assert(skill != VNULL);
	skill->shutdown();
	delete skill;
}

void VEffectManager::setEffectLevel(VEffectLevel level)
{
	mEffectLevel = level;

	// 清除effect pool中所有的特效，因为之前创建出来的特效用的都是不同于现在的级别的，所以不能重用
	_clearFreeEffectPool();

	// 现存的特效也不进行回收
	VEffectList::iterator itr = mActiveEffects.begin();
	while (itr != mActiveEffects.end())
	{
		if (*itr)
		{
			(*itr)->setUseFreePool(VFALSE);
		}

		++itr;
	}
}

/** 创建特效控制器对象，用于每帧更新特效 */
void VEffectManager::_createController()
{
	Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
	mController = controllerManager.createFrameTimePassthroughController(
		Ogre::ControllerValueRealPtr(new VEffectControllerValue(this)));
}

/** 删除特效控制器对象 */
void VEffectManager::_destroyController()
{
	if (mController)
	{
		Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
		controllerManager.destroyController(mController);
		mController = NULL;
	}
}

/** 由控制器调用，每帧更新活动的特效对象 */
void VEffectManager::_updateActiveEffects(const Real &dt)
{
	VEffectIterator itr = mActiveEffects.begin();
	while (itr != mActiveEffects.end())
	{
		VEffect *effect = *itr;

		assert(effect != VNULL);

		if (effect->getDeleteLaterFlag())
		{
			if (!effect->existVisibleElements())
			{
				effect->removeAllElements();

				if (effect->getRemoveParentNodeFlag())
				{
					effect->removeFromParentSceneNode();
				}

				if (!effect->getUseFreePool())
				{
					// 不用空闲特效池，直接删除
					itr = destroyEffect(effect);
				}
				else
				{
					// 从活动链表移除并放入空闲特效池
					itr = mActiveEffects.erase(itr);
					_addToFreeEffectPool(effect);					
				}
			}
			else
			{
				++itr;
			}
		}
		else
		{
			effect->update(dt);
			++itr;
		}
	}

	// 更新特效灯光
	if (mMainSceneLight != VNULL)
	{
		mMainSceneLight->updateLight(dt);
	}
}

/** 根据名称创建技能模板对象 */
VSkill *VEffectManager::_createSkillTemplate(const VString &name)
{
	VSkill *skill = VNULL;
	VSkillMap::const_iterator itr = mSkills.find(name);
	if (itr == mSkills.end())
	{
		skill = new VSkill(name);
		assert(skill != VNULL);
		mSkills[name] = skill;
	}
	else
	{
		// 已经存在该技能模板，抛异常
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"skill template with name '" + name + "' already exists!",
			"VEffectManager::createSkillTemplate");
	}

	return skill;
}

/** 销毁所有技能模板对象 */
void VEffectManager::_destroySkillTemplates()
{
	VSkillMap::iterator itr = mSkills.begin();

	while (itr != mSkills.end())
	{
		delete itr->second;
		itr->second = VNULL;
		++itr;
	}

	mSkills.clear();
}

/** 根据模板名称获取模板 */
VEffect *VEffectManager::_getEffectTemplate(const VString &name)
{
	VEffectMap::const_iterator itr = mEffectTemplates.find(name);
	VEffect *effect = VNULL;
	if (itr != mEffectTemplates.end())
	{
		effect = itr->second;
	}
	return effect;
}

/** 根据名称创建特效模板对象 */
VEffect *VEffectManager::_createEffectTemplate(const VString &name)
{
	VEffect *effect = VNULL;
	VEffectMap::const_iterator itr = mEffectTemplates.find(name);

	if (itr == mEffectTemplates.end())
	{
		effect = new VEffect(name);
		assert(effect != VNULL);
		mEffectTemplates[name] = effect;
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"Effect template with name '" + name + "' already exists!",
			"VEffectManager::createEffectTemplate");
	}

	return effect;
}

/** 销毁所有特效模板对象 */
void VEffectManager::_destroyEffectTemplates()
{
	VEffectMap::iterator itr = mEffectTemplates.begin();

	while (itr != mEffectTemplates.end())
	{
		delete itr->second;
		itr->second = VNULL;
		++itr;
	}

	mEffectTemplates.clear();
}

/** 从空闲特效池中获取一个所指定模板的特效 */
VEffect *VEffectManager::_getEffectFromFreePool(const VString &templateName)
{
	VEffect *freeEffect = VNULL;
	VEffectListMap::iterator itr = mFreeEffects.find(templateName);

	if (itr != mFreeEffects.end())
	{
		VEffectList *list = itr->second;
		if (list->size() > 0)
		{
			VEffectIterator i = list->begin();
			freeEffect = *i;
			list->erase(i);
			freeEffect->reborn();
		}
		else
		{

		}
	}
	else
	{

	}

	return freeEffect;
}

/** 把一个特效加入到特效池中 */
void VEffectManager::_addToFreeEffectPool(VEffect *effect)
{
	assert(effect != VNULL);
	VString templateName = effect->getTemplateName();
	VEffectListMap::iterator itr = mFreeEffects.find(templateName);

	if (itr != mFreeEffects.end())
	{
		VEffectList *list = itr->second;

		// 如果该模板的effect超出了effct池的限制，就把对头的effect清除
		if (list->size() >= (size_t)mMaxNumFreeEffectPerList)
		{
			VEffectIterator i = list->begin();
			VEffect *effect = *i;
			assert(effect != VNULL);
			delete effect;
			effect = VNULL;
			list->erase(i);
		}

		list->push_back(effect);
	}
	else
	{
		VEffectList *list = new VEffectList();
		list->push_back(effect);
		std::pair<VEffectListMap::iterator, bool> inserted = 
			mFreeEffects.insert(VEffectListMap::value_type(templateName, list));
		assert(inserted.second);
	}
}

/** 清除空闲特效池中所有对象 */
void VEffectManager::_clearFreeEffectPool()
{
	VEffectListMap::iterator itr = mFreeEffects.begin();

	while (itr != mFreeEffects.end())
	{
		VEffectList *list = itr->second;

		if (list != VNULL)
		{
			
			VEffectList::iterator i = list->begin();

			while (i != list->end())
			{
				delete (*i);
				(*i) = VNULL;
				++i;
			}

			list->clear();
		}

		delete list;
		itr->second = VNULL;
		++itr;
	}

	mFreeEffects.clear();
}

/** 清除动态特效链表 */
void VEffectManager::_clearActiveEffectList()
{
	VEffectList::iterator itr = mActiveEffects.begin();

	while (itr != mActiveEffects.end())
	{
		delete *itr;
		*itr = VNULL;
		++itr;
	}

	mActiveEffects.clear();
}

/** 注册特效元素工厂 */
void VEffectManager::_registerElementFactories()
{
	_addElementFactory(new VParticleElementFactory());
	_addElementFactory(new VBillboardSetElementFactory());
	_addElementFactory(new VProjectorElementFactory());
	_addElementFactory(new VBeamElementFactory());
	_addElementFactory(new VPointLightElementFactory());
}

/** 添加特效元素工厂 */
void VEffectManager::_addElementFactory(VEffectElementFactory *factory)
{
	assert(factory != VNULL);
	std::pair<VElementFactoryMap::iterator, bool> inserted;
	inserted = mElementFactories.insert(VElementFactoryMap::value_type(factory->getName(), factory));
	assert(inserted.second);
}

/** 清除所有特效元素工厂 */
void VEffectManager::_clearElementFactories()
{
	VElementFactoryMap::iterator itr = mElementFactories.begin();

	while (itr != mElementFactories.end())
	{
		delete itr->second;
		itr->second = VNULL;
		++itr;
	}

	mElementFactories.clear();
}

const Ogre::StringVector &VEffectManager::getScriptPatterns(void) const
{
	return mScriptPatterns;
}

void VEffectManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	mParsingFilename = stream->getName();
	mParsingLineNumber = 0;

	if (Ogre::StringUtil::endsWith(mParsingFilename, ".effect", false))
	{
		// 解析特效文件
		_parseEffectFile(stream);
	}
	else if (Ogre::StringUtil::endsWith(mParsingFilename, ".skill", false))
	{
		// 解析技能文件
		_parseSkillFile(stream);
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
			"wrong file name " + mParsingFilename, "VEffectManager::parseScript");
	}
}

Real VEffectManager::getLoadingOrder(void) const
{
	return 1000.0f;
}


/** 跳到下一个右大括号 */
void VEffectManager::_skipToNextCloseBrace(Ogre::DataStreamPtr &stream)
{
	Ogre::String line = "";

	while (!stream->eof() && line != "}")
	{
		line = stream->getLine();
		++mParsingLineNumber;
	}
}

/** 跳到下一个左大括号 */
void VEffectManager::_skipToNextOpenBrace(Ogre::DataStreamPtr &stream)
{
	Ogre::String line = "";

	while (!stream->eof() && line != "{")
	{
		line = stream->getLine();
		++mParsingLineNumber;
	}
}

/** 解析*.effect文件 */
void VEffectManager::_parseEffectFile(Ogre::DataStreamPtr &stream)
{
	Ogre::String line;
	VEffect *effect = VNULL;
	Ogre::StringVector vecparams;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			// 只要不是空行或者注释
			if (VNULL == effect)
			{
				// 一个新特效
				vecparams = Ogre::StringUtil::split(line, "\t ");

				if (vecparams[0] != "effect" || vecparams.size() != 2)
				{
					_logErrorInfo("Bad effect system name line", line, "VEffectManager::_parseEffectFile");
					return;
				}

				// 创建空的effect
				effect = _createEffectTemplate(vecparams[1]);

				_skipToNextOpenBrace(stream);
			}
			else
			{
				if (line == "}")
				{
					// 完整的解析一个特效
					effect = VNULL;
				}
				else if (line.substr(0, 7) == "element")
				{
					// 一个新的特效元素
					vecparams = Ogre::StringUtil::split(line, "\t ");
					
					if (vecparams.size() < 2)
					{
						_logErrorInfo("Bad effect system element line", line, "VEffectManager::_parseEffectFile");
						return;
					}

					_skipToNextOpenBrace(stream);

					// 解析特效元素的参数
					_parseElement(vecparams[1], stream, effect);
				}
				else
				{
					// 解析特效自己的参数
					_parseEffectAttrib(line, effect);
				}
			}
		}
	}
}

/** 解析特效元素 */
void VEffectManager::_parseElement(const VString &type, Ogre::DataStreamPtr &stream, VEffect *effect)
{
	VEffectElement *element = createElement(type);
	assert(element != VNULL);

	effect->addElement(element);

	Ogre::String line;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			// 跳过空行和注释行
			if (line == "}")
			{
				break;
			}
			else
			{
				_parseElementAttrib(line, element);
			}
		}
	}
}

/** 解析特效元素的属性 */
void VEffectManager::_parseElementAttrib(const VString &line, VEffectElement *element)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseElementAttrib");
		return;
	}

	if (!element->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad effect element attribute line", line, "VEffectManager::_parseElementAttrib");
	}
}

/** 解析特效属性 */
void VEffectManager::_parseEffectAttrib(const VString &line, VEffect *effect)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseEffectAttrib");
		return;
	}

	if (!effect->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad effect attribute line", line, "VEffectManager::_parseEffectAttrib");
	}
}

/** 解析*.skill文件 */
void VEffectManager::_parseSkillFile(Ogre::DataStreamPtr &stream)
{
	Ogre::String line;
	VSkill *skill = VNULL;

	Ogre::StringVector vecparams;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			if (VNULL == skill)
			{
				vecparams = Ogre::StringUtil::split(line, "\t ");

				if (vecparams[0] != "skill" || vecparams.size() != 2)
				{
					_logErrorInfo("Wrong skill name line", line, "EffectManager::parseSkillFile");

					continue;
				}

				// 创建了一个空的effect
				skill = _createSkillTemplate(vecparams[1]);

				_skipToNextOpenBrace(stream);
			}
			else
			{
				if (line == "}")
				{
					// 解析一个完整的技能
					skill = VNULL;
				}
				else if (line == "AnimEffect")
				{
					_skipToNextOpenBrace(stream);
					_parseAnimEffectInfo(stream, skill);
				}
				else if (line == "Ribbon")
				{
					_skipToNextOpenBrace(stream);
					_parseAnimRibbon(stream, skill);
				}
				else if (line == "SceneLight")
				{
					_skipToNextOpenBrace(stream);
					_parseAnimSceneLight(stream, skill);
				}
				else if (line == "Sound")
				{
					_skipToNextOpenBrace(stream);
					_parseAnimSound(stream, skill);
				}
				else
				{
					// 解析技能属性
					_parseSkillAttrib(line, skill);
				}
			}
		}
	}
}

/** 解析skill属性 */
void VEffectManager::_parseSkillAttrib(const VString &line, VSkill *skill)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseSkillAttrib");
		return;
	}

	if (!skill->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad skill attribute line", line, "VEffectManager::_parseSkillAttrib");
	}
}

/** 解析animation effect info */
void VEffectManager::_parseAnimEffectInfo(Ogre::DataStreamPtr &stream, VSkill *skill)
{
	assert(skill != VNULL);

	VAnimationEffectInfo *effectInfo = skill->addAnimationEffectInfo();
	assert(effectInfo != VNULL);

	Ogre::String line;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			if (line == "}")
			{
				break;
			}
			else
			{
				_parseAnimEffectInfoAttrib(line, effectInfo);
			}
		}
	}
}

/** 解析animation effect info属性 */
void VEffectManager::_parseAnimEffectInfoAttrib(const VString &line, VAnimationEffectInfo *effectInfo)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ");

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseAnimEffectInfoAttrib");
		return;
	}

	if (!effectInfo->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad Anim Effect Info attribute line", line, "VEffectManager::_parseAnimEffectInfoAttrib");
	}
}

/** 解析animation ribbon */
void VEffectManager::_parseAnimRibbon(Ogre::DataStreamPtr &stream, VSkill *skill)
{
	assert(skill != VNULL);

	VAnimationRibbon *ribbon = skill->addAnimationRibbon();
	assert(ribbon != VNULL);

	Ogre::String line;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			if (line == "}")
			{
				break;
			}
			else
			{
				_parseAnimRibbonAttrib(line, ribbon);
			}
		}
	}
}

/** 解析animation ribbon属性 */
void VEffectManager::_parseAnimRibbonAttrib(const VString &line, VAnimationRibbon *ribbon)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ");

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseAnimRibbonAttrib");
		return;
	}

	if (!ribbon->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad Anim Ribbon attribute line", line, "VEffectManager::_parseAnimRibbonAttrib");
	}
}

/** 解析animation scene light */
void VEffectManager::_parseAnimSceneLight(Ogre::DataStreamPtr &stream, VSkill *skill)
{
	assert(skill != VNULL);

	VAnimationSceneLight *sceneLight = skill->addAnimationSceneLight();
	assert(sceneLight != VNULL);

	Ogre::String line;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			if (line == "}")
			{
				break;
			}
			else
			{
				_parseAnimSceneLightAttrib(line, sceneLight);
			}
		}
	}
}

/** 解析animation scene light属性 */
void VEffectManager::_parseAnimSceneLightAttrib(const VString &line, VAnimationSceneLight *sceneLight)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ");

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseAnimSceneLightAttrib");
		return;
	}

	if (!sceneLight->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad Anim Scene Light attribute line", line, "VEffectManager::_parseAnimSceneLightAttrib");
	}
}

/** 解析animation sound */
void VEffectManager::_parseAnimSound(Ogre::DataStreamPtr &stream, VSkill *skill)
{
	assert(skill != VNULL);

	VAnimationSound *sound = skill->addAnimationSound();
	assert(sound != VNULL);

	Ogre::String line;

	while (!stream->eof())
	{
		line = stream->getLine();
		++mParsingLineNumber;

		if (!(line.empty() || line.substr(0, 2) == "//"))
		{
			if (line == "}")
			{
				break;
			}
			else
			{
				_parseAnimSoundAttrib(line, sound);
			}
		}
	}
}

/** 解析animation sound属性 */
void VEffectManager::_parseAnimSoundAttrib(const VString &line, VAnimationSound *sound)
{
	Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ");

	if (vecparams.size() != 2)
	{
		_logErrorInfo("the number of parameters must be 2!", line, "VEffectManager::_parseAnimSoundAttrib");
		return;
	}

	if (!sound->setParameter(vecparams[0], vecparams[1]))
	{
		_logErrorInfo("Bad Anim Sound attribute line", line, "VEffectManager::_parseAnimSoundAttrib");
	}
}

void VEffectManager::_logErrorInfo(const Ogre::String &errorInfo, const Ogre::String &lineContent, const Ogre::String &functionName)
{
	Ogre::String errorDesc;

	errorDesc += "\n------------EffectManager Error Description------------------------";
	errorDesc += "\nfile : " + mParsingFilename;
	errorDesc += "\nline : " + Ogre::StringConverter::toString(mParsingLineNumber);
	errorDesc += "\nline content : " + lineContent;
	errorDesc += "\nfunction : " + functionName;
	errorDesc += "\ndescription : " + errorInfo;
	errorDesc += "\n-------------------------------------------------------------------";

	Ogre::LogManager::getSingleton().logMessage(errorDesc);
}