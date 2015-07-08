

#include "VLogicModelSerializer.h"
#include "VisionXMLAttributes.h"
#include "VLogicModel.h"
#include "VLogicModelManager.h"


VLogicModelXMLHandler::VLogicModelXMLHandler(VLogicModelManager *manager)
	: mManager(manager)
	, mModel(VNULL)
{

}

VLogicModelXMLHandler::~VLogicModelXMLHandler()
{

}


void VLogicModelXMLHandler::characters(const VString &chars)
{

}

void VLogicModelXMLHandler::startElement(const VString &element, const XMLAttributes &attributes)
{
	if (element == "AllObjects")
	{

	}
	else if (element == "Object")
	{
		VString name = attributes.getValue("name");
		mModel = new VLogicModel(name);
	}
	else if (element == "EntityList")
	{

	}
	else if (element == "Entity")
	{
		assert(mModel != VNULL);

		VString entName = attributes.getValue("name");
		VString meshName = attributes.getValue("file");
		VString matName = attributes.getValue("material");

		mModel->addEntity(entName, meshName, matName);
	}
	else if (element == "MaterialList")
	{
	}
	else if (element == "Material")
	{
		assert(mModel != VNULL);

		VString name = attributes.getValue("name");
		VString entName = attributes.getValue("entity");
		VString matName = attributes.getValue("material");

		mModel->addMaterial(name, entName, matName);
		if (mModel->hasAttribute(name))
		{
			mModel->addAttribute(name, "", "Material");
		}
	}
	else if (element == "Locators")
	{
		VINT32 count = attributes.getValueAs<VINT32>("count");
	}
	else if (element == "Locator")
	{
		assert(mModel != VNULL);

		VString name = attributes.getValue("name");
		VString boneName = attributes.getValue("bonename");
		VFLOAT x = attributes.getValueAs<VFLOAT>("x");
		VFLOAT y = attributes.getValueAs<VFLOAT>("y");
		VFLOAT z = attributes.getValueAs<VFLOAT>("z");
		VFLOAT qx = attributes.getValueAs<VFLOAT>("qx");
		VFLOAT qy = attributes.getValueAs<VFLOAT>("qy");
		VFLOAT qz = attributes.getValueAs<VFLOAT>("qz");
		VFLOAT qw = attributes.getValueAs<VFLOAT>("qw");

		VBOOL transferable = attributes.getValueAs<VBOOL>("transferable");
		VBOOL translateFirst = attributes.getValueAs<VBOOL>("translateFirst");

		Vector3 pos(x, y, z);
		Quaternion ori = Quaternion::IDENTITY;
		ori.FromAngleAxis(Ogre::Radian(qw), Ogre::Vector3(qx, qy, qz));

		if (mModel->addLocator(name, boneName, pos, ori, transferable, translateFirst))
		{
			mLocatorName = name;
		}
		else
		{
			mLocatorName.clear();
		}
	}
	else if (element == "Slot")
	{
		assert(mModel != VNULL);
		assert(!mLocatorName.empty());

		VString name = attributes.getValue("name");
		VString object = attributes.getValue("object");
		VString attribs = attributes.getValue("attribs");

		mModel->addSlot(mLocatorName, name, object, attribs);
	}
	else if (element == "Attributes")
	{

	}
	else if (element == "Attribute")
	{
		assert(mModel != VNULL);

		VString name = attributes.getValue("name");
		VString value = attributes.getValue("value");
		VString hints = attributes.getValue("hints");

		mModel->addAttribute(name, value, hints);
	}
	else if (element == "Effects")
	{

	}
	else if (element == "Effect")
	{
		VString name = attributes.getValue("name");
		VString eff = attributes.getValue("effect");
		VString locator = attributes.getValue("locator");
		VBOOL translateAll = attributes.getValueAs<VBOOL>("translateAll");
		VString val = attributes.getValue("colour");
		Ogre::ColourValue colour = Ogre::StringConverter::parseColourValue(val);
	}
	else if (element == "ShadowUncasts")
	{

	}
	else if (element == "Mesh")
	{
		assert(mModel != VNULL);

		// Shadow uncastable mesh
		VString meshName = attributes.getValue("meshname");
		mModel->addShadowUncastable(meshName);
	}
	else if (element == "Skeletons")
	{

	}
	else if (element == "Skeleton")
	{
		VString skel = attributes.getValue("Skeleton");
		VString animations = attributes.getValue("animations");

	}
	else if (element == "FabricColliders")
	{

	}
	else if (element == "FabricCollider")
	{
		assert(mModel != VNULL);

		VString name = attributes.getValue("name");
		VString locator = attributes.getValue("locator");
		VString info = attributes.getValue("info");

	}
}

void VLogicModelXMLHandler::endElement(const VString &element)
{
	if (element == "AllObjects")
	{
		
	}
	else if (element == "Object")
	{
		VLogicModelManager &mgr = VLOGIC_MODEL_MANAGER;
		std::pair<VLogicModelManager::VLogicModelMap::iterator, VBOOL> inserted = 
			mgr.mLogicModelTemplates.insert(VLogicModelManager::VLogicModelMap::value_type(mModelName, mModel));

		Ogre::StringUtil::toLowerCase(mModelName);

		if (!inserted.second)
		{
			delete mModel;
			mModel = VNULL;

			Ogre::LogManager::getSingleton().logMessage( "Logic Model with name '" + mModelName + "' inserted failed! " +
				"LogicModelManager::parseScript " + mModelName );
		}

		mModel = VNULL;
		mModelName.clear();
	}
	else if (element == "EntityList")
	{
		
	}
	else if (element == "Entity")
	{
		
	}
	else if (element == "MaterialList")
	{
	}
	else if (element == "Material")
	{
	}
	else if (element == "Locators")
	{
		
	}
	else if (element == "Locator")
	{
		mLocatorName.clear();
	}
	else if (element == "Slot")
	{
		
	}
	else if (element == "Attributes")
	{
		
	}
	else if (element == "Attribute")
	{
		
	}
	else if (element == "Effects")
	{
		
	}
	else if (element == "Effect")
	{
		
	}
	else if (element == "ShadowUncasts")
	{
		
	}
	else if (element == "Mesh")
	{
		
	}
	else if (element == "Skeletons")
	{
		
	}
	else if (element == "Skeleton")
	{
		
	}
	else if (element == "FabricColliders")
	{
		
	}
	else if (element == "FabricCollider")
	{
		
	}
}

