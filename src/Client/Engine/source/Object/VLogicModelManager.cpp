

#include "VLogicModelManager.h"
#include "VLogicModel.h"
#include "VResource.h"
#include "VGraphicsSystem.h"
#include "VEngine.h"
#include "VisionUtils.h"
#include "tinyxml.h"


using namespace Vision;


class VLogicModelControllerValue : public Ogre::ControllerValue<VFLOAT>
{
protected:
	VLogicModelManager *mLogicModelManager;

public:
	VLogicModelControllerValue(VLogicModelManager *manager)
		: mLogicModelManager(manager)
	{
	}

	VFLOAT getValue() const
	{
		return 0.0f;
	}

	void setValue(VFLOAT value)
	{
		mLogicModelManager->_updateAllLogicModels(value);
		mLogicModelManager->_updateAllModelBoundingBox();
	}
};


template<> VLogicModelManager *VSingleton<VLogicModelManager>::mInstance = VNULL;


VLogicModelManager::VLogicModelManager()
	: mAutoNameModel(0)
{
	mLogicModels.clear();
	mLogicModelTemplates.clear();

	mScriptPatterns.push_back("*.obj");
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	_createController();
}

VLogicModelManager::~VLogicModelManager()
{
	_destroyController();

	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);

	_destroyAllLogicModelTemplates();
	_destroyAllLogicModels();

	_destroyAllAnimationSkeletonLinks();

	_cleanupSkeletalEntityCache();
}


////////////////////////////////////// from Ogre::ScriptLoader ////////////////////////////////////

const Ogre::StringVector &VLogicModelManager::getScriptPatterns() const
{
	return mScriptPatterns;
}

void VLogicModelManager::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
{
	TiXmlDocument xmldoc;
	TiXmlElement *rootElement = VNULL;

	xmldoc.Parse(stream->getAsString().c_str());
	if (xmldoc.Error())
	{
		Ogre::LogManager::getSingleton().logMessage( "parse Logic Model Failed : "+Ogre::String(xmldoc.ErrorDesc()));
		return;
	}

	// Retrieve root element of the xml file.
	rootElement = xmldoc.RootElement();
	if (rootElement->ValueStr() == "AllObjects")
	{
		for (TiXmlNode *child = rootElement->FirstChild(); child != VNULL; child = child->NextSibling() )
		{
			_parseSingleObject(child, "");
		}
	}
	else
	{
		_parseSingleObject(rootElement, stream->getName());
	}
}

Ogre::Real VLogicModelManager::getLoadingOrder() const
{
	return 1000.0f;
}

VLogicModel *VLogicModelManager::createLogicModel(const VString &modelName)
{
	VString autoName = "AutoNameModel_" + Ogre::StringConverter::toString(mAutoNameModel++);

	return createLogicModel(autoName, modelName);
}

VLogicModel *VLogicModelManager::createLogicModel(const VString &name, const VString &modelName)
{
	if (mLogicModels.find(name) != mLogicModels.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"Logic Model with name '" + name + "' already exists.", 
			"LogicModelManager::createLogicModel");
	}

	const VLogicModel *modelTemplate = getTemplateByName(modelName);

	if (!modelTemplate)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot find required Logic Model template '" + modelName + "'",
			"LogicModelManager::createLogicModel");
	}

	VLogicModel *model = new VLogicModel(modelName);
	std::pair<VLogicModelMap::iterator, VBOOL> inserted =
		mLogicModels.insert(VLogicModelMap::value_type(name, model));
	assert(inserted.second);

	*model = *modelTemplate;
	model->init();
	
	return model;
}

void VLogicModelManager::destroyLogicModel(const VString &name)
{
	VLogicModelMap::iterator itr = mLogicModels.find(name);
	if (itr != mLogicModels.end())
	{
		delete itr->second;
		itr->second = VNULL;
		mLogicModels.erase(itr);
	}
}

void VLogicModelManager::destroyLogicModel(VLogicModel *model)
{
	for (VLogicModelMap::iterator itr = mLogicModels.begin(); itr != mLogicModels.end(); ++itr)
	{
		if (itr->second == model)
		{
			delete itr->second;
			itr->second = VNULL;
			mLogicModels.erase(itr);
			break;
		}
	}
}

const VLogicModel *VLogicModelManager::getTemplateByName(const VString &modelName) const
{
	VString objName = modelName;
	Ogre::StringUtil::toLowerCase(objName);

	VLogicModel *model = VNULL;
	VLogicModelMap::const_iterator itr = mLogicModelTemplates.find(objName);
	if (itr != mLogicModelTemplates.end())
	{
		model = itr->second;
	}

	return model;
}

VLogicModel *VLogicModelManager::getModelByName(const VString &modelName)
{
	VLogicModel *model = VNULL;
	VLogicModelMap::iterator itr = mLogicModels.find(modelName);
	if (itr != mLogicModels.end())
	{
		model = itr->second;
	}

	return model;
}

const VString &VLogicModelManager::getSkeletonFromAnimationName(const VString &modelName, const VString &animName) const
{
	VAnimationSkeletonLinksMap::const_iterator itr = mAnimationSkeletonLinks.find(modelName);

	if (itr == mAnimationSkeletonLinks.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"can't find the skeleton link map of the model '" + modelName + 
			"', get animation '" + animName + "' failed!",
			"VLogicModelManager::getSkeletonFromAnimationName");
	}

	VAnimationSkeletonLinks *links = itr->second;

	// 在找出这个动作所对应的骨骼名称
	VAnimationSkeletonLinks::const_iterator it = links->find(animName);
	if (it == links->end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"can't find the skeleton link to the animation '" + 
			animName + ", get animation '" + animName + "' failed!",
			"VLogicModelManager::getSkeletonFromAnimationName"); 
	}

	return it->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VLogicModelManager::_parseSingleObject(TiXmlNode *rootElement, const VString &objectName)
{
	const char *tempValue = VNULL;

	const TiXmlElement *objectElement = rootElement->ToElement();

	if (objectElement == VNULL)
	{
		Ogre::LogManager::getSingleton().logMessage( "Object Element is NULL : _parseSingleObject " + objectName);
		return;
	}

	const TiXmlElement *attribsElement = VNULL;
	const TiXmlElement *entityListElement = VNULL;
	const TiXmlElement *materialListElement = VNULL;
	const TiXmlElement *effectsElement = VNULL;
	const TiXmlElement *shadowUncastableElement = VNULL;
	const TiXmlElement *locatorsElement = VNULL;
	const TiXmlElement *animationSkeletonElement = VNULL;
	const TiXmlElement *fabricColliderElement = VNULL;

	VString realObjectName;

	if (objectName.empty())
	{
		realObjectName = objectElement->Attribute("name");
	}
	else
	{
		realObjectName = objectName;
	}

	VLogicModel *model = new VLogicModel(realObjectName);

	/////////////////// EntityList ///////////////////////
	entityListElement = objectElement->FirstChildElement("EntityList");

	if (entityListElement != VNULL)
	{
		const TiXmlElement *entityElement = entityListElement->FirstChildElement("Entity");

		while (entityElement != VNULL)
		{
			VString entName, meshName, matName;

			// name of entity
			tempValue = entityElement->Attribute("name");
			if (tempValue != VNULL)
			{
				entName = tempValue;
			}
			else
			{
				entName = "";
			}

			// name of mesh
			tempValue = entityElement->Attribute("file");
			if (tempValue != VNULL)
			{
				meshName = tempValue;
			}
			else
			{
				meshName = "";
			}

			// name of material
			tempValue = entityElement->Attribute("material");
			if (tempValue != VNULL)
			{
				matName = tempValue;
			}
			else
			{
				matName = "";
			}

			model->addEntity(entName, meshName, matName);
			if (model->mAttributes.find(entName) == model->mAttributes.end())
			{
				model->addAttribute(entName, "", "Mesh");
			}

			entityElement = entityElement->NextSiblingElement("Entity");
		}
	}

	/////////////////// MaterialList ////////////////////
	materialListElement = objectElement->FirstChildElement("MaterialList");

	if (materialListElement != VNULL)
	{
		const TiXmlElement *materialElement = materialListElement->FirstChildElement("Material");

		while (materialElement != VNULL)
		{
			VString name, entName, matName;

			tempValue = materialElement->Attribute("name");
			if (tempValue != VNULL)
			{
				name = tempValue;
			}
			else
			{
				name = "";
			}

			tempValue = materialElement->Attribute("entity");
			if (tempValue != VNULL)
			{
				entName = tempValue;
			}
			else
			{
				entName = tempValue;
			}

			tempValue = materialElement->Attribute("material");
			if (tempValue != VNULL)
			{
				matName = tempValue;
			}
			else
			{
				matName = "";
			}

			model->addMaterial(name, entName, matName);
			if (model->mAttributes.find(name) == model->mAttributes.end())
			{
				model->addAttribute(name, "", "Material");
			}

			materialElement = materialElement->NextSiblingElement("Material");
		}
	}

	/////////////////// Locators /////////////////////
	locatorsElement = objectElement->FirstChildElement("Locators");

	if (locatorsElement != VNULL)
	{
		const TiXmlElement *locatorElement = locatorsElement->FirstChildElement("Locator");

		while (locatorElement != VNULL)
		{
			VString locatorName, boneName;
			Vector3 pos = Vector3::ZERO;
			Quaternion orientation = Quaternion::IDENTITY;
			Real qx = 0.0f, qy = 0.0f, qz = 0.0f, qw = 0.0f;
			VBOOL transferable = VFALSE;
			VBOOL translateFirst = VFALSE;

			tempValue = locatorElement->Attribute("name");
			if (tempValue != VNULL)
			{
				locatorName = tempValue;
			}
			else
			{
				locatorElement = locatorsElement->NextSiblingElement("Locator");
				continue;
			}

			tempValue = locatorElement->Attribute("bonename");
			if (tempValue != VNULL)
			{
				boneName = tempValue;
			}
			else
			{
				boneName = "";
			}

			pos.x = Ogre::StringConverter::parseReal(locatorElement->Attribute("x"));
			pos.y = Ogre::StringConverter::parseReal(locatorElement->Attribute("y"));
			pos.z = Ogre::StringConverter::parseReal(locatorElement->Attribute("z"));

			qx = Ogre::StringConverter::parseReal(locatorElement->Attribute("qx"));
			qy = Ogre::StringConverter::parseReal(locatorElement->Attribute("qy"));
			qz = Ogre::StringConverter::parseReal(locatorElement->Attribute("qz"));
			qw = Ogre::StringConverter::parseReal(locatorElement->Attribute("qw"));
			orientation.FromAngleAxis(Ogre::Radian(qw), Ogre::Vector3(qx, qy, qz));

			tempValue = locatorElement->Attribute("transferable");
			if (tempValue != VNULL)
			{
				transferable = Ogre::StringConverter::parseBool(tempValue);
			}

			tempValue = locatorElement->Attribute("translateFirst");
			if (tempValue != VNULL)
			{
				translateFirst = Ogre::StringConverter::parseBool(tempValue);
			}

			if (!model->addLocator(locatorName, boneName, pos, orientation, transferable, translateFirst))
			{
				locatorElement = locatorElement->NextSiblingElement("Locator");
				continue;
			}

			/////////////////////// Slot //////////////////////
			const TiXmlElement *slotElement = locatorElement->FirstChildElement("Slot");

			while (slotElement != VNULL)
			{
				VString slotName, slotObj, slotAttrib;

				tempValue = slotElement->Attribute("name");
				if (tempValue != VNULL)
				{
					slotName = tempValue;
				}
				else
				{
					slotElement = slotElement->NextSiblingElement("Slot");
					continue;
				}

				tempValue = slotElement->Attribute("object");
				if (tempValue != VNULL)
				{
					slotObj = tempValue;
				}
				else
				{
					slotObj = "";
				}

				tempValue = slotElement->Attribute("attribs");
				if (tempValue != VNULL)
				{
					slotAttrib = tempValue;
				}
				else
				{
					slotAttrib = "";
				}

				model->addSlot(locatorName, slotName, slotObj, slotAttrib);
				if (model->mAttributes.find(slotName) == model->mAttributes.end())
				{
					model->addAttribute(slotName, "", "SlotCreate");
				}

				slotElement = slotElement->NextSiblingElement("Slot");
			}

			locatorElement = locatorElement->NextSiblingElement("Locator");
		}
	}

	/////////////////// Attribute //////////////////////
	attribsElement = objectElement->FirstChildElement("Attributes");

	if (attribsElement != VNULL)
	{
		const TiXmlElement *attribElement = attribsElement->FirstChildElement("Attribute");

		while (attribElement != VNULL)
		{
			VString name, value, hints;

			tempValue = attribElement->Attribute("name");
			if (tempValue != VNULL)
			{
				name = tempValue;
			}
			else
			{
				attribElement = attribElement->NextSiblingElement("Attribute");
				continue;
			}

			tempValue = attribElement->Attribute("value");
			if (tempValue != VNULL)
			{
				value = tempValue;
			}
			else
			{
				value = "";
			}

			tempValue = attribElement->Attribute("hints");
			if (tempValue != VNULL)
			{
				hints = tempValue;
			}
			else
			{
				hints = "";
			}

			model->addAttribute(name, value, hints);

			attribElement = attribElement->NextSiblingElement("Attribute");
		}
	}

	////////////////////// Effects /////////////////////
	effectsElement = objectElement->FirstChildElement("Effects");

	if (effectsElement != VNULL)
	{
		const TiXmlElement *effectElement = effectsElement->FirstChildElement("Effect");

		while (effectElement != VNULL)
		{
			VString name, effectName, locatorName;
			VBOOL translateAll = VFALSE;
			Ogre::ColourValue color = Ogre::ColourValue::White;

			tempValue = effectElement->Attribute("name");
			if (tempValue != VNULL)
			{
				name = tempValue;
			}
			else
			{
				name = "";
			}

			tempValue = effectElement->Attribute("effect");
			if (tempValue != VNULL)
			{
				effectName = tempValue;
			}
			else
			{
				effectName = "";
			}

			tempValue = effectElement->Attribute("locator");
			if (tempValue != VNULL)
			{
				locatorName = tempValue;
			}
			else
			{
				locatorName = "";
			}

			tempValue = effectElement->Attribute("translateAll");
			if (tempValue != VNULL)
			{
				translateAll = Ogre::StringConverter::parseBool(tempValue);
			}

			tempValue = effectElement->Attribute("colour");
			if (tempValue != VNULL)
			{
				color = Ogre::StringConverter::parseColourValue(tempValue);
			}

			model->addEffect(name, effectName, locatorName, translateAll, color);

			effectElement = effectElement->NextSiblingElement("Effect");
		}
	}

	//////////////////////// ShadowUncastable //////////////////
	shadowUncastableElement = objectElement->FirstChildElement("ShadowUncasts");
	if (shadowUncastableElement != VNULL)
	{
		const TiXmlElement *meshElement = shadowUncastableElement->FirstChildElement("Mesh");

		while (meshElement != VNULL)
		{
			VString meshName;

			tempValue = meshElement->Attribute("meshname");

			if (tempValue != VNULL) 
			{
				meshName = tempValue;
			}
			else
			{
				meshName = "";                
			}

			model->addShadowUncastable(meshName);

			meshElement = meshElement->NextSiblingElement("Mesh");
		}
	}

	/////////////////////// Skeletons //////////////////////////
	animationSkeletonElement = objectElement->FirstChildElement("Skeletons");
	VAnimationSkeletonLinks *links = _createAnimationSkeletonLinksForModel(realObjectName);

	if (animationSkeletonElement != VNULL)
	{
		const TiXmlElement *skeletonElement = VNULL;
		skeletonElement = animationSkeletonElement->FirstChildElement("Skeleton");

		while (skeletonElement != VNULL)
		{
			VString skeletonName;
			tempValue = skeletonElement->Attribute("Skeleton");

			if (tempValue != VNULL)
			{
				skeletonName = tempValue;
			}
			else
			{
				skeletonName = "";
			}

			VString animations;
			tempValue = skeletonElement->Attribute("animations");

			if (tempValue != VNULL)
			{
				animations = tempValue;
			}
			else
			{
				animations = "";
			}

			if (skeletonName.empty() || animations.empty())
			{
				Ogre::LogManager::getSingleton().logMessage( "skeleton name is empty or animations is empty "
					"when parse animation skeleton link!"
					"LogicModelManager::parseScript " + realObjectName );

				skeletonElement = skeletonElement->NextSiblingElement("Skeleton");
				continue;
			}

			if (links != VNULL)
			{
				Ogre::StringVector animArray = Ogre::StringUtil::split(animations);
				assert(animArray.size());

				for (size_t i = 0; i < animArray.size(); ++i)
				{
					// 往link map中插入动作与骨骼名称的对应关系
					VString temp = animArray[i];
					links->insert(VAnimationSkeletonLinks::value_type(temp, skeletonName));
				}
			}

			skeletonElement = skeletonElement->NextSiblingElement("Skeleton");
		}

		if (links != VNULL)
		{
			model->mLinks = links;
		}
	}

	Ogre::StringUtil::toLowerCase(realObjectName);
	std::pair<VLogicModelMap::iterator, VBOOL> inserted = 
		mLogicModelTemplates.insert(VLogicModelMap::value_type(realObjectName, model));

	if (!inserted.second)
	{
		delete model;
		model = VNULL;

		Ogre::LogManager::getSingleton().logMessage( "Logic Model with name '" + realObjectName + "' inserted failed! " +
			"LogicModelManager::parseScript " + realObjectName );
	}
}

void VLogicModelManager::_createController()
{
	static Ogre::ControllerFunctionRealPtr gIdentityControllerFunction(new Ogre::ScaleControllerFunction(1.0f, false));
	Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
	mController = controllerManager.createController(
		controllerManager.getFrameTimeSource(),
		Ogre::ControllerValueRealPtr(new VLogicModelControllerValue(this)),
		gIdentityControllerFunction);
}

void VLogicModelManager::_destroyController()
{
	if (mController)
	{
		Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
		controllerManager.destroyController(mController);
		mController = VNULL;
	}
}

void VLogicModelManager::_updateAllLogicModels(const Real &dt)
{
	for (VLogicModelMap::iterator itr = mLogicModels.begin(); itr != mLogicModels.end(); ++itr)
	{
		VLogicModel *model = itr->second;
		assert(model);
		if (model->isActive() && !model->isChildModel()/* && !model->isAttached()*/)
		{
			model->execute(dt);
		}
	}
}

void VLogicModelManager::_updateAllModelBoundingBox()
{

}

void VLogicModelManager::_destroyAllLogicModels()
{
	for (VLogicModelMap::iterator itr = mLogicModels.begin(); itr != mLogicModels.end(); ++itr)
	{
		if (itr->second)
		{
			delete itr->second;
			itr->second = VNULL;
		}
	}

	mLogicModels.clear();
}

void VLogicModelManager::_destroyAllLogicModelTemplates()
{
	for (VLogicModelMap::iterator itr = mLogicModelTemplates.begin(); itr != mLogicModelTemplates.end(); ++itr)
	{
		if (itr->second)
		{
			delete itr->second;
			itr->second = VNULL;
		}
	}

	mLogicModelTemplates.clear();
}

Ogre::Entity *VLogicModelManager::_createSkeletalEntity(const Ogre::SkeletonPtr& skeleton)
{
	Ogre::Entity *entity;

	VSkeletalEntityCache::iterator itr = mSkeletalEntityCache.find(skeleton.get());
	if (itr != mSkeletalEntityCache.end())
	{
		entity = itr->second;
		mSkeletalEntityCache.erase(itr);

		Ogre::AnimationStateSet *ass = entity->getAllAnimationStates();
		while (ass->hasEnabledAnimationState())
		{
			Ogre::AnimationState *as = ass->getEnabledAnimationStateIterator().getNext();
			as->setEnabled(false);
		}
	}
	else
	{
		Ogre::MeshPtr mesh = createNullMeshForSkeleton(skeleton);
		static VUINT32 gsCount = 0;
		VGraphicsSystem *system = VENGINE.getGfxSystem();
		entity = system->getSceneManager()->createEntity(
			mesh->getName()+Ogre::StringConverter::toString(gsCount++,4,'0'),
			mesh->getName());
	}

	return entity;
}

void VLogicModelManager::_destroySkeletalEntity(Ogre::Entity* entity)
{
	mSkeletalEntityCache.insert(VSkeletalEntityCache::value_type(entity->getMesh()->getSkeleton().get(), entity));
}

void VLogicModelManager::_cleanupSkeletalEntityCache()
{
	VGraphicsSystem *system = VENGINE.getGfxSystem();
	VSkeletalEntityCache::iterator itr;
	for (itr = mSkeletalEntityCache.begin(); itr != mSkeletalEntityCache.end(); ++itr)
	{
		Ogre::Entity *entity = itr->second;
		system->getSceneManager()->destroyEntity(entity);
	}
	mSkeletalEntityCache.clear();
}

VAnimationSkeletonLinks *VLogicModelManager::_createAnimationSkeletonLinksForModel(const VString &modelName)
{
	VAnimationSkeletonLinksMap::iterator itr = mAnimationSkeletonLinks.find(modelName);
	if (itr != mAnimationSkeletonLinks.end())
	{
		Ogre::LogManager::getSingleton().logMessage( "VAnimationSkeletonLinks with name '"
			+ modelName + "' already existed! Maybe something is wrong!" + "VLogicModelManager::parseScript" );

		return itr->second;
	}
	
	VAnimationSkeletonLinks *links = new VAnimationSkeletonLinks();
	mAnimationSkeletonLinks.insert(VAnimationSkeletonLinksMap::value_type(modelName, links));
	return links;
}

void VLogicModelManager::_destroyAllAnimationSkeletonLinks()
{
	VAnimationSkeletonLinksMap::iterator itr = mAnimationSkeletonLinks.begin();

	while (itr != mAnimationSkeletonLinks.end())
	{
		delete itr->second;

		++itr;
	}

	mAnimationSkeletonLinks.clear();
}

