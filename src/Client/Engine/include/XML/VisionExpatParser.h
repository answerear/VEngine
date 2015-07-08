

#ifndef __VISION_EXPAT_PARSER_H__
#define __VISION_EXPAT_PARSER_H__


#include <memory>
#include "VisionXMLParser.h"


struct ConvertBuffer
{
	Ogre::UTFString	utfString;
	std::vector<char> multiByteBuffer;
};


class VENGINE_DLL ExpatParser : public XMLParser
{
public:
	ExpatParser(const VLocale *loc);
	virtual ~ExpatParser();

	void parseXMLFile(XMLHandler &handler, const VString &xmlName, const VString &xmlResourceGroup);
	void parseXMLFile(XMLHandler &handler, Ogre::DataStreamPtr &dataStream);

protected:
	std::auto_ptr<VLocale>	mLocale;
	ConvertBuffer			mBuffer;
};


#endif	/*__VISION_EXPAT_PARSER_H__*/
