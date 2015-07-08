

#ifndef __VISION_SCENE_SERIALIZER_H__
#define __VISION_SCENE_SERIALIZER_H__


#include "VisionXMLHandler.h"
#include "VSceneObject.h"


class VScene;


class VSceneXMLHandler : public XMLHandler
{
public:
	explicit VSceneXMLHandler(VScene *scene);
	~VSceneXMLHandler();

	virtual void characters(const VString &chars);
	virtual void startElement(const VString &element, const XMLAttributes &attributes);
	virtual void endElement(const VString &element);

protected:
	typedef std::map<VString, VString> PropertyMap;
	typedef std::map<VString, PropertyMap> ObjectMap;

	VScene	*mScene;
	VSceneObjectPtr	mObject;
	VString mAttributeName;
	VString mAttributeValue;
	VString	*mString;
};


#endif	/*__VISION_SCENE_SERIALIZER_H__*/

