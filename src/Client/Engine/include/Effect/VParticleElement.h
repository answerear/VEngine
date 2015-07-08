

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

	VColorMap mColorMap;	 /// ��������ϵͳ��ԭ������ɫ����������ɫӰ�����е�ԭ��ɫ��Ҳ����������ϵͳ����ɫ
	Ogre::ColourValue mCurrentColor;			/**< ��ǰ���õ�������ɫ */

	Ogre::String			mParticleSystemName;/**< ����ϵͳ���� */
	Ogre::ParticleSystem	*mParticleSystem;	/**< ����ϵͳ���� */

	VBOOL	mNeedUpdateColor;	/**< �Ƿ���Ҫ������ɫ */
	VBOOL	mIsEmmiterEnabled;	/**< ���ӷ������Ƿ���Ч */
};


#endif	/*__VISION_PARTICLE_ELEMENT_H__*/
