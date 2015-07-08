

#ifndef __VISION_LOGIC_MODEL_SERIALIZER_H__
#define __VISION_LOGIC_MODEL_SERIALIZER_H__


#include "VisionXMLHandler.h"


class VLogicModelManager;
class VLogicModel;


class VLogicModelXMLHandler : public XMLHandler
{
public:
	explicit VLogicModelXMLHandler(VLogicModelManager *manager);
	virtual ~VLogicModelXMLHandler();

	virtual void characters(const VString &chars);
	virtual void startElement(const VString &element, const XMLAttributes &attributes);
	virtual void endElement(const VString &element);

protected:
	VLogicModelManager	*mManager;
	VLogicModel			*mModel;

	VString mModelName;
	VString	mLocatorName;
};


#endif	/*__VISION_LOGIC_MODEL_SERIALIZER_H__*/
