

#ifndef __VISION_LOGIC_MODEL_MANAGER_H__
#define __VISION_LOGIC_MODEL_MANAGER_H__


#include "VEngineConfig.h"
#include "VSingleton.h"


class VLogicModel;
class TiXmlNode;

// 骨骼与动作之间的关联map，key为动作名称，value为包含这个动作的骨骼文件名称
typedef std::map<VString, VString>	VAnimationSkeletonLinks;

class VENGINE_DLL VLogicModelManager 
	: public VSingleton<VLogicModelManager>
	, public Ogre::ScriptLoader
{
	friend class VLogicModelXMLHandler;
	friend class VLogicModel;
	friend class VLogicModelControllerValue;

public:
	// 逻辑模型map，key为逻辑模型名称，value为对应逻辑模型对象
	typedef std::map<VString, VLogicModel*> VLogicModelMap;
	// 骨骼与对应实体之间关联的缓存
	typedef std::multimap<Ogre::Skeleton*, Ogre::Entity*> VSkeletalEntityCache;
	// 模型与骨骼动画关联之间的map，key为逻辑模型名称，value为骨骼动画关联map
	typedef std::map<VString, VAnimationSkeletonLinks*> VAnimationSkeletonLinksMap;

public:
	VLogicModelManager();
	virtual ~VLogicModelManager();

public:	
	/** 
	 * @brief 创建逻辑模型
	 */
	VLogicModel *createLogicModel(const VString &modelName);
	VLogicModel *createLogicModel(const VString &name, const VString &modelName);

	/**
	 * @brief 销毁逻辑模型
	 */
	void destroyLogicModel(const VString &name);
	void destroyLogicModel(VLogicModel *model);

	/** 
	 * @brief 根据名称获取逻辑模型模板
	 */
	const VLogicModel *getTemplateByName(const VString &modelName) const;
	VLogicModel *getModelByName(const VString &modelName);

	/** 
	 * @brief 获取指定动画所在的骨骼名称
	 */
	const VString &getSkeletonFromAnimationName(const VString &modelName, const VString &animName) const;

	/**
	 * @brief 设置动画插值模式
	 */
	void setAnimationInterpolationMode(Ogre::Animation::InterpolationMode mode)
	{
		mInterpolationMode = mode;
	}
	/** 
	 * @brief 获取动画插值模式
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
	 * @brief 解析单个物体
	 */
	void _parseSingleObject(TiXmlNode *rootElement, const VString &objectName);

	/**
	 * @brief 创建逻辑模型控制器
	 */
	void _createController();
	/** 
	 * @brief 销毁逻辑模型控制器
	 */
	void _destroyController();

	/**
	 * @brief 更新所有逻辑模型
	 */
	void _updateAllLogicModels(const Real &dt);
	/**
	 * @brief 更新所有模型碰撞区域
	 */
	void _updateAllModelBoundingBox();	

	/** 
	 * @brief 销毁所有逻辑模型
	 */
	void _destroyAllLogicModels();
	/**
	 * @brief 销毁所有逻辑模型模板
	 */
	void _destroyAllLogicModelTemplates();

	/**
	 * @brief 创建骨骼对应的实体
	 */
	Ogre::Entity* _createSkeletalEntity(const Ogre::SkeletonPtr& skeleton);
	/** 
	 * @brief 销毁骨骼对应的实体
	 */
	void _destroySkeletalEntity(Ogre::Entity* entity);

	/**
	 * @brief 清楚骨骼实体缓存
	 */
	void _cleanupSkeletalEntityCache(void);

	/**
	 * @brief 创建骨骼和动画对应关系
	 */
	VAnimationSkeletonLinks *_createAnimationSkeletonLinksForModel(const VString &modelName);

	/** 
	 * @brief 清除所有的骨骼动画关联对象
	 */
	void _destroyAllAnimationSkeletonLinks();

protected:
	VUINT32	mAutoNameModel;	/**< 创建模型时自动生成的名称 */

	Ogre::Controller<Ogre::Real> *mController;	/**< 用于控制每帧刷新的控制器 */

	Ogre::StringVector	mScriptPatterns;	/**< for Ogre::ScriptLoader */

	VLogicModelMap mLogicModelTemplates;	/**< 逻辑模型模板 */
	VLogicModelMap mLogicModels;			/**< 实体模型模板 */

	VSkeletalEntityCache		mSkeletalEntityCache;		/**< 骨骼实体缓存 */
	VAnimationSkeletonLinksMap	mAnimationSkeletonLinks;	/**< 骨骼名称与动作之间的map */

	Ogre::Animation::InterpolationMode	mInterpolationMode;	/** 动画插值模式 */
};


#define VLOGIC_MODEL_MANAGER	VLogicModelManager::instance()

#endif	/*__VISION_LOGIC_MODEL_MANAGER_H__*/
