

#ifndef __VISION_MODEL_SERIALIZER_H__
#define __VISION_MODEL_SERIALIZER_H__


#include "VEngineConfig.h"
#include "VisionXMLHandler.h"


class VModel;


class VModelXMLHandler : public XMLHandler
{
public:
	VModelXMLHandler(VModel *model, const VString &resourceName, const VString &groupName);
	virtual ~VModelXMLHandler();

	void startElement(const VString &element, const XMLAttributes &attributes);
	void endElement(const VString &element);

protected:
	VModel	*mModel;

	Ogre::Entity		*mMasterEntity;
	Ogre::MovableObject	*mLastMovable;
	Ogre::Node			*mLastAttachNode;

	VString mModelResourceName;
	VString mModelGroupName;
};


#endif	/*__VISION_MODEL_SERIALIZER_H__*/
