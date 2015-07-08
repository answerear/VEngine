

#ifndef __VISION_XML_WRITER_H__
#define __VISION_XML_WRITER_H__


#include "VEngineConfig.h"


class XMLNode;


class XMLWriter
{
public:
	XMLWriter();
	virtual ~XMLWriter();

	virtual void writeXMLFile(const XMLNode *root, const VString &filename) = 0;
};


#endif	/*__VISION_XML_WRITER_H__*/
