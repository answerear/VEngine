

#ifndef __VISION_LOGIC_MODEL_MANAGER_H__
#define __VISION_LOGIC_MODEL_MANAGER_H__


#include "VEngineConfig.h"
#include "VSingleton.h"


class VLogicModel;
class TiXmlNode;

// �����붯��֮��Ĺ���map��keyΪ�������ƣ�valueΪ������������Ĺ����ļ�����
typedef std::map<VString, VString>	VAnimationSkeletonLinks;

class VENGINE_DLL VLogicModelManager 
	: public VSingleton<VLogicModelManager>
	, public Ogre::ScriptLoader
{
	friend class VLogicModelXMLHandler;
	friend class VLogicModel;
	friend class VLogicModelControllerValue;

public:
	// �߼�ģ��map��keyΪ�߼�ģ�����ƣ�valueΪ��Ӧ�߼�ģ�Ͷ���
	typedef std::map<VString, VLogicModel*> VLogicModelMap;
	// �������Ӧʵ��֮������Ļ���
	typedef std::multimap<Ogre::Skeleton*, Ogre::Entity*> VSkeletalEntityCache;
	// ģ���������������֮���map��keyΪ�߼�ģ�����ƣ�valueΪ������������map
	typedef std::map<VString, VAnimationSkeletonLinks*> VAnimationSkeletonLinksMap;

public:
	VLogicModelManager();
	virtual ~VLogicModelManager();

public:	
	/** 
	 * @brief �����߼�ģ��
	 */
	VLogicModel *createLogicModel(const VString &modelName);
	VLogicModel *createLogicModel(const VString &name, const VString &modelName);

	/**
	 * @brief �����߼�ģ��
	 */
	void destroyLogicModel(const VString &name);
	void destroyLogicModel(VLogicModel *model);

	/** 
	 * @brief �������ƻ�ȡ�߼�ģ��ģ��
	 */
	const VLogicModel *getTemplateByName(const VString &modelName) const;
	VLogicModel *getModelByName(const VString &modelName);

	/** 
	 * @brief ��ȡָ���������ڵĹ�������
	 */
	const VString &getSkeletonFromAnimationName(const VString &modelName, const VString &animName) const;

	/**
	 * @brief ���ö�����ֵģʽ
	 */
	void setAnimationInterpolationMode(Ogre::Animation::InterpolationMode mode)
	{
		mInterpolationMode = mode;
	}
	/** 
	 * @brief ��ȡ������ֵģʽ
	 */
	Ogre::Animation::InterpolationMode getAnimationInterpolationMode() const
	{
		return mInterpolationMode;
	}

protected:
	// from Ogre::ScriptLoader
	const Ogre::StringVector &getScriptPatterns() const;
	void parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName);
	Ogre::Real getLoadingOrder() const;

	/** 
	 * @brief ������������
	 */
	void _parseSingleObject(TiXmlNode *rootElement, const VString &objectName);

	/**
	 * @brief �����߼�ģ�Ϳ�����
	 */
	void _createController();
	/** 
	 * @brief �����߼�ģ�Ϳ�����
	 */
	void _destroyController();

	/**
	 * @brief ���������߼�ģ��
	 */
	void _updateAllLogicModels(const Real &dt);
	/**
	 * @brief ��������ģ����ײ����
	 */
	void _updateAllModelBoundingBox();	

	/** 
	 * @brief ���������߼�ģ��
	 */
	void _destroyAllLogicModels();
	/**
	 * @brief ���������߼�ģ��ģ��
	 */
	void _destroyAllLogicModelTemplates();

	/**
	 * @brief ����������Ӧ��ʵ��
	 */
	Ogre::Entity* _createSkeletalEntity(const Ogre::SkeletonPtr& skeleton);
	/** 
	 * @brief ���ٹ�����Ӧ��ʵ��
	 */
	void _destroySkeletalEntity(Ogre::Entity* entity);

	/**
	 * @brief �������ʵ�建��
	 */
	void _cleanupSkeletalEntityCache(void);

	/**
	 * @brief ���������Ͷ�����Ӧ��ϵ
	 */
	VAnimationSkeletonLinks *_createAnimationSkeletonLinksForModel(const VString &modelName);

	/** 
	 * @brief ������еĹ���������������
	 */
	void _destroyAllAnimationSkeletonLinks();

protected:
	VUINT32	mAutoNameModel;	/**< ����ģ��ʱ�Զ����ɵ����� */

	Ogre::Controller<Ogre::Real> *mController;	/**< ���ڿ���ÿ֡ˢ�µĿ����� */

	Ogre::StringVector	mScriptPatterns;	/**< for Ogre::ScriptLoader */

	VLogicModelMap mLogicModelTemplates;	/**< �߼�ģ��ģ�� */
	VLogicModelMap mLogicModels;			/**< ʵ��ģ��ģ�� */

	VSkeletalEntityCache		mSkeletalEntityCache;		/**< ����ʵ�建�� */
	VAnimationSkeletonLinksMap	mAnimationSkeletonLinks;	/**< ���������붯��֮���map */

	Ogre::Animation::InterpolationMode	mInterpolationMode;	/** ������ֵģʽ */
};


#define VLOGIC_MODEL_MANAGER	VLogicModelManager::instance()

#endif	/*__VISION_LOGIC_MODEL_MANAGER_H__*/
