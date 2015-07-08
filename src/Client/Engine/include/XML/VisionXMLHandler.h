

#ifndef __VISION_XML_HANDLER_H__
#define __VISION_XML_HANDLER_H__


#include "VEngineConfig.h"


class XMLAttributes;


class XMLHandler
{
public:
	XMLHandler();
	virtual ~XMLHandler();

	virtual void characters(const VString &chars);
	virtual void startElement(const VString &element, const XMLAttributes &attributes);
	virtual void endElement(const VString &element);
};


#endif	/*__VISION_XML_HANDLER_H__*/
