

#ifndef __VISION_XML_PARSER_H__
#define __VISION_XML_PARSER_H__


#include "VEngineConfig.h"


class XMLHandler;


class XMLParser
{
public:
	XMLParser();
	virtual ~XMLParser();

	virtual void parseXMLFile(XMLHandler &handler, const VString &xmlName, const VString &xmlResourceGroup) = 0;
};


#endif	/*__VISION_XML_PARSER_H__*/
