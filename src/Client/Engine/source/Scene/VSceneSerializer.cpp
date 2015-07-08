
#include "VSceneSerializer.h"
#include "VisionXMLAttributes.h"
#include "VScene.h"
#include "VSceneObjectFactoryManager.h"


static const VString CURRENT_FORMAT_VERSION = "0.1.1";


VSceneXMLHandler::VSceneXMLHandler(VScene *scene)
	: mScene(scene)
	, mObject()
	, mAttributeName()
	, mAttributeValue()
	, mString()
{

}

VSceneXMLHandler::~VSceneXMLHandler()
{

}

void VSceneXMLHandler::characters(const VString &chars)
{
	if (mString)
	{
		mString->append(chars);
	}
}

void VSceneXMLHandler::startElement(const VString &element, const XMLAttributes &attributes)
{
	if (element == "Property")
	{
		assert(mObject);

		const VString &name = attributes.getValue("name");
		const VString &value = attributes.getValue("value");
		mObject->setPropertyAsString(name, value);
	}
	else if (element == "Object")
	{
		assert(!mObject);

		const VString &type = attributes.getValue("type");
		mObject = VSceneObjectFactoryManager::instance().createInstance(type);
		if (attributes.exists("name"))
		{
			mObject->setName(attributes.getValue("name"));
		}
		else
		{
			mObject->setName(mScene->generateAutoName(mObject));
		}
		mScene->addObject(mObject);
	}
	else if (element == "Attribute")
	{
		assert(mString);
		mAttributeName = attributes.getValue("name");
		mAttributeValue.clear();
		mString = &mAttributeValue;
	}
	else if (element == "Terrain")
	{
		mScene->mTerrainFilename = attributes.getValue("filename");
		if(attributes.exists("minheight"))
			mScene->mTerrainMinHeight = Ogre::StringConverter::parseReal(attributes.getValue("minheight"));
		if(attributes.exists("maxheight"))
			mScene->mTerrainMaxHeight = Ogre::StringConverter::parseReal(attributes.getValue("maxheight"));
	}
	else if (element == "Scene")
	{
		Ogre::String formatVersion = attributes.getValue("formatVersion");
		if (formatVersion != CURRENT_FORMAT_VERSION &&
			formatVersion != "0.1.0")
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Unexpected format version was found while parsing the scene file."
				"\nformat version require: " + CURRENT_FORMAT_VERSION +
				"\nformat version in scene file: " + formatVersion,
				"VSceneXMLHandler::startElement");
		}

		mScene->mName = attributes.getValueAs<VString>("name", Ogre::StringUtil::BLANK);
	}
	else if (element == "Author")
	{
		assert(!mString);
		mString = &mScene->mAuthor;
	}
	else if (element == "Copyright")
	{
		assert(!mString);
		mString = &mScene->mCopyright;
	}
	else if (element == "Description")
	{
		assert(!mString);
		mString = &mScene->mDescription;
	}
	else
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Unexpected data was found while parsing the scene file: '" + element + "' is unknown.",
			"VSceneXMLHandler::startElement");
	}
}

void VSceneXMLHandler::endElement(const VString &element)
{
	if (element == "Object")
	{
		assert(mObject);
		mObject.reset();
	}
	else if (element == "Attribute")
	{
		assert(mString);
		mString = NULL;
		mScene->setAttribute(mAttributeName, mAttributeValue);
	}
	else if (element == "Author")
	{
		assert(mString);
		mString = NULL;
	}
	else if (element == "Copyright")
	{
		assert(mString);
		mString = NULL;
	}
	else if (element == "Description")
	{
		assert(mString);
		mString = NULL;
	}
}

