

#include "VEngine.h"
#include "VGraphicsSystem.h"
#include "VEffectManager.h"
#include "VParticleElement.h"

#include <OgreParticleEmitter.h>
#include <OgreParticleAffector.h>
#include <OgreParticleSystemRenderer.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Command
////////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::String VParticleElement::CmdParticleSystem::doGet(const void *target) const
{
	return static_cast<const VParticleElement*>(target)->getParticleSystemName();
}

void VParticleElement::CmdParticleSystem::doSet(void *target, const Ogre::String &val)
{
	static_cast<VParticleElement*>(target)->setParticleSystemName(val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// class VParticleElement
////////////////////////////////////////////////////////////////////////////////////////////////////

VParticleElement::CmdParticleSystem VParticleElement::msParticleSystemCmd;

VParticleElement::VParticleElement(const VString &type)
	: VEffectElement(type)
	, mCurrentColor(Ogre::ColourValue::White)
	, mParticleSystemName("")
	, mParticleSystem(VNULL)
	, mNeedUpdateColor(VFALSE)
	, mIsEmmiterEnabled(VFALSE)
{
	if (_initDefaults(type))
	{
		Ogre::ParamDictionary *dict = getParamDictionary();

		// Custom params      
		dict->addParameter(Ogre::ParameterDef("ParticleSystem", 
			"the name of particle system.",
			Ogre::PT_STRING),
			&msParticleSystemCmd);      
	}
}

VParticleElement::~VParticleElement()
{
	destroyRenderInstance();
}

void VParticleElement::setParticleSystemName(const VString &name)
{
	mParticleSystemName = name;
	_destroyParticleSystem();
	mIsRenderInstanceCreated = VFALSE;
}

VBOOL VParticleElement::createRenderInstance()
{
	return _createParticleSystem();
}

void VParticleElement::updateRenderInstance(const Real &dt, const VTransform &xform)
{
	if (!mIsEmmiterEnabled && VNULL != mParticleSystem)
	{
		// ʹ���з�������Ч
		for (VWORD i = 0; i < mParticleSystem->getNumEmitters(); ++i)
		{
			Ogre::ParticleEmitter *emitter = mParticleSystem->getEmitter(i);
			if (VNULL != emitter && !emitter->getEnabled())
			{
				emitter->setEnabled(VTRUE);
			}
		}

		mIsEmmiterEnabled = VTRUE;
	}

	if (mNeedUpdateColor)
	{
		// ��Ҫ����������ɫ
		_setAffectorColor();
		mNeedUpdateColor = VFALSE;
	}
}

void VParticleElement::destroyRenderInstance()
{
	_destroyParticleSystem();
	VEffectElement::destroyRenderInstance();
}

void VParticleElement::destroy()
{
	VEffectElement::destroy();
}

void VParticleElement::shutdown()
{
	// �������з���������Ч
	if (mParticleSystem != VNULL)
	{
		for (VWORD i = 0; i < mParticleSystem->getNumEmitters(); ++i)
		{
			Ogre::ParticleEmitter *emitter = mParticleSystem->getEmitter(i);

			if (emitter != VNULL && emitter->getEnabled())
			{
				emitter->setEnabled(VFALSE);
			}
		}
	}
}

void VParticleElement::sleep()
{
	if (mParticleSystem != VNULL)
	{
		// �Ȱ�����ϵͳ����Ⱦ���ڵ�����
		if (mParticleSystem->getParentNode() != VNULL 
			&& mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
		{
			mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
		}

		// �ٰ����з�������Ч
		for (VWORD i = 0; i < mParticleSystem->getNumEmitters(); ++i)
		{
			Ogre::ParticleEmitter *emitter = mParticleSystem->getEmitter(i);
			if (emitter != VNULL && emitter->getEnabled())
			{
				emitter->setEnabled(VFALSE);
			}
		}

		mIsEmmiterEnabled = VFALSE;

		mParticleSystem->clear();

		Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();

		assert(renderer);

		if (renderer->getType() == "mesh")
		{

		}
	}

	mCurrentColor = Ogre::ColourValue::White;
	mNeedUpdateColor = VFALSE;

	VEffectElement::sleep();
}

void VParticleElement::active()
{
	// ������ϵͳ���ڵ���Ⱦ����
	if (mParticleSystem != VNULL && mParticleSystem->getParentNode() != VNULL && mBaseNode != VNULL)
	{
		mBaseNode->attachObject(mParticleSystem);
	}

	VEffectElement::active();
}

VBOOL VParticleElement::existVisibleElements() const
{
	if (mParticleSystem != VNULL)
	{
		return (mParticleSystem->getNumParticles() > 0);
	}
	return VFALSE;
}

void VParticleElement::setColor(const Ogre::ColourValue &color)
{
	mCurrentColor = color;
	mNeedUpdateColor = VTRUE;
}

void VParticleElement::setVisibilityFlags(VUINT32 flags)
{
	if (mParticleSystem != VNULL)
	{
		mParticleSystem->setVisibilityFlags(flags);
	}
}

void VParticleElement::setVisible(VBOOL visible)
{
	if (mParticleSystem != VNULL)
	{
		if (visible)
		{
			// Ҫ��ʾ���ȹ��ص���Ⱦ����
			if (mParticleSystem->getParentNode() == VNULL && mBaseNode != VNULL)
			{
				mBaseNode->attachObject(mParticleSystem);
			}
		}
		else
		{
			// Ҫ���أ��Ӹ��ڵ���Ⱦ����ж��
			if (mParticleSystem->getParentNode() != VNULL 
				&& mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
			{
				mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
			}

			// ����mesh���ӣ�Ҫ���������ɵ�mesh particle
			Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();
			assert (renderer);

			if (renderer->getType() == "mesh")
			{
				
			}
		}
	}

	VEffectElement::setVisible(visible);
}


VBOOL VParticleElement::_createParticleSystem()
{
	assert(mParticleSystem == VNULL);
	
	VBOOL result = VFALSE;

	if (!mParticleSystemName.empty())
	{
		Ogre::SceneNode *parent = mBaseNode;

		if (parent != VNULL)
		{
			mParticleSystem = parent->getCreator()->createParticleSystem(parent->getName(), mParticleSystemName);
			assert(mParticleSystem != VNULL);
			Ogre::ParticleSystem *psTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate(mParticleSystemName);
			
			// ��ȡ��Ч���𣬲�ͬ�����Ӧ��ͬ����������
			VEffectLevel effectLevel = VEFFECT_MANAGER.getEffectLevel();

			if (VEL_MID == effectLevel)
			{
				// �м���Ч
				VINT32 quota = (VINT32)(psTemplate->getParticleQuota() * 0.5f);
				if (0 == quota)
					quota = 1;

				mParticleSystem->setParticleQuota(quota);
				mParticleSystem->setIterationInterval(0.0f);
			}
			else if (VEL_LOW == effectLevel)
			{
				// �ͼ���Ч
				VINT32 quota = (VINT32)(psTemplate->getParticleQuota() * 0.1f);
				if (0 == quota)
					quota = 1;

				mParticleSystem->setParticleQuota(quota);
				mParticleSystem->setIterationInterval(0.0f);
			}

			mParticleSystem->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
			mParticleSystem->setCastShadows(VFALSE);
			mParticleSystem->setVisible(mVisible);
			parent->attachObject(mParticleSystem);
			result = VTRUE;
		}
	}

	return result;
}

void VParticleElement::_destroyParticleSystem()
{
	if (mParticleSystem != VNULL)
	{
		Ogre::SceneManager *sceneMgr = VENGINE.getGfxSystem()->getSceneManager();
		sceneMgr->destroyParticleSystem(mParticleSystem);
		mParticleSystem = VNULL;
	}
}

void VParticleElement::_setAffectorColor()
{
	// �ı�colour faderӰ�����е���ɫ
	if (mParticleSystem != VNULL)
	{
		for ( unsigned short i = 0; i < mParticleSystem->getNumAffectors(); ++i )
		{
			Ogre::ParticleAffector *affector = mParticleSystem->getAffector(i);

			assert (affector);
			if (affector->getType() == "ColourFading")
			{
				for ( unsigned short num = 0; num < 6; ++num )
				{
					Ogre::String paraName = "colour" + Ogre::StringConverter::toString(num);
					VColorMap::iterator i = mColorMap.find(paraName);

					Ogre::ColourValue originColor;

					// ���֮ǰ�Ѿ������ԭ������ɫ����ֱ�ӻ�ȡ
					if (i != mColorMap.end())
					{
						originColor = i->second;
					}
					else
					{
						originColor = 
							Ogre::StringConverter::parseColourValue(affector->getParameter(paraName));

						/// ����ԭ������ɫ
						mColorMap.insert(VColorMap::value_type(paraName, originColor));
					}

					/// ��ԭ������ɫ��Ҫ�趨����ɫ�������
					affector->setParameter(paraName, Ogre::StringConverter::toString(originColor * mCurrentColor));
				}

			}
		}
	}
}
