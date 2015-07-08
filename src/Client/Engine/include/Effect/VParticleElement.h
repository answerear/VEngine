

#ifndef __VISION_PARTICLE_ELEMENT_H__
#define __VISION_PARTICLE_ELEMENT_H__


#include "VEffectElement.h"


class VParticleElement : public VEffectElement
{
protected:
	class CmdParticleSystem : public Ogre::ParamCommand
	{
	protected:
		Ogre::String doGet(const void *target) const;
		void doSet(void *target, const Ogre::String &val);
	};

public:
	VParticleElement(const VString &type);
	~VParticleElement();

	void setParticleSystemName(const VString &name);
	const VString &getParticleSystemName() const
	{
		return mParticleSystemName;
	}

	virtual VBOOL createRenderInstance();
	virtual void updateRenderInstance(const Real &dt, const VTransform &xform);
	virtual void destroyRenderInstance();

	virtual void destroy();

	virtual void shutdown();

	virtual void sleep();
	virtual void active();
	virtual VBOOL existVisibleElements() const;

	virtual void setColor(const Ogre::ColourValue &color);

	virtual void setVisibilityFlags(VUINT32 flags);
	virtual void setVisible(VBOOL visible);

protected:
	VBOOL _createParticleSystem();
	void _destroyParticleSystem();

	void _setAffectorColor();

protected:
	typedef std::map<VString, Ogre::ColourValue> VColorMap;

	static CmdParticleSystem	msParticleSystemCmd;

	VColorMap mColorMap;	 /// 保存粒子系统中原来的颜色，可能是颜色影响器中的原颜色，也可能是粒子系统的颜色
	Ogre::ColourValue mCurrentColor;			/**< 当前设置的粒子颜色 */

	Ogre::String			mParticleSystemName;/**< 粒子系统名称 */
	Ogre::ParticleSystem	*mParticleSystem;	/**< 粒子系统对象 */

	VBOOL	mNeedUpdateColor;	/**< 是否需要更新颜色 */
	VBOOL	mIsEmmiterEnabled;	/**< 粒子发射器是否有效 */
};


#endif	/*__VISION_PARTICLE_ELEMENT_H__*/
