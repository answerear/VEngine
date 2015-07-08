

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
		// 使所有发射器有效
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
		// 需要更新粒子颜色
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
	// 设置所有发射器均无效
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
		// 先把粒子系统从渲染父节点脱离
		if (mParticleSystem->getParentNode() != VNULL 
			&& mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
		{
			mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
		}

		// 再把所有发射器无效
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
	// 把粒子系统挂在到渲染树上
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
			// 要显示，先挂载到渲染树上
			if (mParticleSystem->getParentNode() == VNULL && mBaseNode != VNULL)
			{
				mBaseNode->attachObject(mParticleSystem);
			}
		}
		else
		{
			// 要隐藏，从父节点渲染树中卸载
			if (mParticleSystem->getParentNode() != VNULL 
				&& mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
			{
				mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
			}

			// 对于mesh粒子，要隐藏它生成的mesh particle
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
			
			// 获取特效级别，不同级别对应不同的粒子数量
			VEffectLevel effectLevel = VEFFECT_MANAGER.getEffectLevel();

			if (VEL_MID == effectLevel)
			{
				// 中级特效
				VINT32 quota = (VINT32)(psTemplate->getParticleQuota() * 0.5f);
				if (0 == quota)
					quota = 1;

				mParticleSystem->setParticleQuota(quota);
				mParticleSystem->setIterationInterval(0.0f);
			}
			else if (VEL_LOW == effectLevel)
			{
				// 低级特效
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
	// 改变colour fader影响器中的颜色
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

					// 如果之前已经保存过原来的颜色，就直接获取
					if (i != mColorMap.end())
					{
						originColor = i->second;
					}
					else
					{
						originColor = 
							Ogre::StringConverter::parseColourValue(affector->getParameter(paraName));

						/// 保存原来的颜色
						mColorMap.insert(VColorMap::value_type(paraName, originColor));
					}

					/// 用原来的颜色与要设定的颜色进行相乘
					affector->setParameter(paraName, Ogre::StringConverter::toString(originColor * mCurrentColor));
				}

			}
		}
	}
}
