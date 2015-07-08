

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
	// ���ȴӿ�����Ч���л�ȡ
	VEffect *effect = VNULL;
	effect = _getEffectFromFreePool(templateName);

	// ���û�л�ȡ�����½�һ��
	if (VNULL == effect)
	{
		VEffect *tempEffect = _getEffectTemplate(templateName);
		if (VNULL == tempEffect)
		{
			// û���ҵ���Чģ�壬����
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
				// ������Ԫ��������
				effect->removeAllElements();

				if (effect->getRemoveParentNodeFlag())
				{
					effect->removeFromParentSceneNode();
				}

				// �ӻ������ɾ�������Ҽ��뵽���г���
				mActiveEffects.erase(itr);
				_addToFreeEffectPool(effect);
				break;
			}
			++itr;
		}
	}
	else
	{
		// �ر���Ч�������ӳ�ɾ�����
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

	// ���effect pool�����е���Ч����Ϊ֮ǰ������������Ч�õĶ��ǲ�ͬ�����ڵļ���ģ����Բ�������
	_clearFreeEffectPool();

	// �ִ����ЧҲ�����л���
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

/** ������Ч��������������ÿ֡������Ч */
void VEffectManager::_createController()
{
	Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
	mController = controllerManager.createFrameTimePassthroughController(
		Ogre::ControllerValueRealPtr(new VEffectControllerValue(this)));
}

/** ɾ����Ч���������� */
void VEffectManager::_destroyController()
{
	if (mController)
	{
		Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
		controllerManager.destroyController(mController);
		mController = NULL;
	}
}

/** �ɿ��������ã�ÿ֡���»����Ч���� */
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
					// ���ÿ�����Ч�أ�ֱ��ɾ��
					itr = destroyEffect(effect);
				}
				else
				{
					// �ӻ�����Ƴ������������Ч��
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

	// ������Ч�ƹ�
	if (mMainSceneLight != VNULL)
	{
		mMainSceneLight->updateLight(dt);
	}
}

/** �������ƴ�������ģ����� */
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
		// �Ѿ����ڸü���ģ�壬���쳣
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"skill template with name '" + name + "' already exists!",
			"VEffectManager::createSkillTemplate");
	}

	return skill;
}

/** �������м���ģ����� */
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

/** ����ģ�����ƻ�ȡģ�� */
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

/** �������ƴ�����Чģ����� */
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

/** ����������Чģ����� */
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

/** �ӿ�����Ч���л�ȡһ����ָ��ģ�����Ч */
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

/** ��һ����Ч���뵽��Ч���� */
void VEffectManager::_addToFreeEffectPool(VEffect *effect)
{
	assert(effect != VNULL);
	VString templateName = effect->getTemplateName();
	VEffectListMap::iterator itr = mFreeEffects.find(templateName);

	if (itr != mFreeEffects.end())
	{
		VEffectList *list = itr->second;

		// �����ģ���effect������effct�ص����ƣ��ͰѶ�ͷ��effect���
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

/** ���������Ч�������ж��� */
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

/** �����̬��Ч���� */
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

/** ע����ЧԪ�ع��� */
void VEffectManager::_registerElementFactories()
{
	_addElementFactory(new VParticleElementFactory());
	_addElementFactory(new VBillboardSetElementFactory());
	_addElementFactory(new VProjectorElementFactory());
	_addElementFactory(new VBeamElementFactory());
	_addElementFactory(new VPointLightElementFactory());
}

/** �����ЧԪ�ع��� */
void VEffectManager::_addElementFactory(VEffectElementFactory *factory)
{
	assert(factory != VNULL);
	std::pair<VElementFactoryMap::iterator, bool> inserted;
	inserted = mElementFactories.insert(VElementFactoryMap::value_type(factory->getName(), factory));
	assert(inserted.second);
}

/** ���������ЧԪ�ع��� */
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
		// ������Ч�ļ�
		_parseEffectFile(stream);
	}
	else if (Ogre::StringUtil::endsWith(mParsingFilename, ".skill", false))
	{
		// ���������ļ�
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


/** ������һ���Ҵ����� */
void VEffectManager::_skipToNextCloseBrace(Ogre::DataStreamPtr &stream)
{
	Ogre::String line = "";

	while (!stream->eof() && line != "}")
	{
		line = stream->getLine();
		++mParsingLineNumber;
	}
}

/** ������һ��������� */
void VEffectManager::_skipToNextOpenBrace(Ogre::DataStreamPtr &stream)
{
	Ogre::String line = "";

	while (!stream->eof() && line != "{")
	{
		line = stream->getLine();
		++mParsingLineNumber;
	}
}

/** ����*.effect�ļ� */
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
			// ֻҪ���ǿ��л���ע��
			if (VNULL == effect)
			{
				// һ������Ч
				vecparams = Ogre::StringUtil::split(line, "\t ");

				if (vecparams[0] != "effect" || vecparams.size() != 2)
				{
					_logErrorInfo("Bad effect system name line", line, "VEffectManager::_parseEffectFile");
					return;
				}

				// �����յ�effect
				effect = _createEffectTemplate(vecparams[1]);

				_skipToNextOpenBrace(stream);
			}
			else
			{
				if (line == "}")
				{
					// �����Ľ���һ����Ч
					effect = VNULL;
				}
				else if (line.substr(0, 7) == "element")
				{
					// һ���µ���ЧԪ��
					vecparams = Ogre::StringUtil::split(line, "\t ");
					
					if (vecparams.size() < 2)
					{
						_logErrorInfo("Bad effect system element line", line, "VEffectManager::_parseEffectFile");
						return;
					}

					_skipToNextOpenBrace(stream);

					// ������ЧԪ�صĲ���
					_parseElement(vecparams[1], stream, effect);
				}
				else
				{
					// ������Ч�Լ��Ĳ���
					_parseEffectAttrib(line, effect);
				}
			}
		}
	}
}

/** ������ЧԪ�� */
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
			// �������к�ע����
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

/** ������ЧԪ�ص����� */
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

/** ������Ч���� */
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

/** ����*.skill�ļ� */
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

				// ������һ���յ�effect
				skill = _createSkillTemplate(vecparams[1]);

				_skipToNextOpenBrace(stream);
			}
			else
			{
				if (line == "}")
				{
					// ����һ�������ļ���
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
					// ������������
					_parseSkillAttrib(line, skill);
				}
			}
		}
	}
}

/** ����skill���� */
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

/** ����animation effect info */
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

/** ����animation effect info���� */
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

/** ����animation ribbon */
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

/** ����animation ribbon���� */
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

/** ����animation scene light */
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

/** ����animation scene light���� */
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

/** ����animation sound */
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

/** ����animation sound���� */
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