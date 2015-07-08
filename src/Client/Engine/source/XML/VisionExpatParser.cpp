

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <expat.h>
#include "VTextCodec.h"

#include "VisionExpatParser.h"
#include "VisionXMLHandler.h"
#include "VisionXMLAttributes.h"


typedef boost::function2<const VString &, VString &, const VString &> UTFConverter;


static const VString &expat_to_local(VString &d, const VString &s, ConvertBuffer &buffers)
{
	VTEXT_CODEC.Utf8ToGB2312(s, d);
	return d;
}


struct UserData
{
	XMLHandler		*handler;
	UTFConverter	converter;
	VString			s0, s1;
};


static void XMLCALL expatStartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
{
	UserData *data = static_cast<UserData*>(userData);
	XMLHandler *handler = data->handler;
	const UTFConverter &converter = data->converter;
	assert(handler);
	assert(converter);
	VString &s0 = data->s0;
	VString &s1 = data->s1;

	XMLAttributes attributes;
	for (const XML_Char **p = atts; *p; p += 2)
	{
		assert(p[0] && p[1]);
		s0 = p[0];
		s1 = p[1];

		attributes.setValue(converter(s0,s0), converter(s1, s1));
	}

	s0 = name;
	handler->startElement(converter(s0, s0), attributes);
}

static void XMLCALL expatEndElementHandler(void *userData, const XML_Char *name)
{
	UserData *data = static_cast<UserData*>(userData);
	assert(data);
	XMLHandler *handler = data->handler;
	const UTFConverter &converter = data->converter;
	assert(handler);
	assert(converter);
	VString &s0 = data->s0;
	s0 = name;
	handler->endElement(converter(s0, s0));
}

static void XMLCALL expatCharacterDataHandler(void *userData, const XML_Char *s, int len)
{
	UserData *data = static_cast<UserData*>(userData);
	assert(data);
	XMLHandler *handler = data->handler;
	const UTFConverter &converter = data->converter;
	assert(handler);
	assert(converter);
	VString &s0 = data->s0;
	s0 = VString(s, len);
	handler->characters(converter(s0, s0));
}


ExpatParser::ExpatParser(const VLocale *loc)
	: mLocale()
{
	if (loc)
	{
		mLocale.reset(new VLocale(*loc));
	}
}

ExpatParser::~ExpatParser()
{

}

void ExpatParser::parseXMLFile(XMLHandler &handler, const VString &xmlName, const VString &xmlResourceGroup)
{
	XML_Parser parser = XML_ParserCreate(VNULL);

	try
	{
		UserData userData;
		userData.handler = &handler;
		userData.converter = boost::bind(&expat_to_local, _1, _2, boost::ref(mBuffer));

		XML_SetUserData(parser, &userData);
		XML_SetElementHandler(parser, expatStartElementHandler, expatEndElementHandler);
		XML_SetCharacterDataHandler(parser, expatCharacterDataHandler);

		const size_t MaxBufferSize = 64 * 1024;
		Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource(xmlName, xmlResourceGroup);
		VBOOL done;
		do 
		{
			void *buffer = XML_GetBuffer(parser, MaxBufferSize);
			if (!buffer)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
					"Can't allocate buffer while parse XML file '" + xmlName + "'.",
					"ExpatParser::parseXMLFile");
			}

			size_t count = dataStream->read(buffer, MaxBufferSize);
			done = dataStream->eof();

			if (XML_ParseBuffer(parser, count, done) != XML_STATUS_OK)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
					"An error occurred while parsing XML file '" + xmlName +
					"' at line " + Ogre::StringConverter::toString(XML_GetCurrentLineNumber(parser)) +
					". Additional information: " + XML_ErrorString(XML_GetErrorCode(parser)),
					"ExpatParser::parseXMLFile");
			}
		} while (!done);
	}
	catch (Ogre::Exception &e)
	{
		XML_ParserFree(parser);
		throw;
	}

	XML_ParserFree(parser);
}

void ExpatParser::parseXMLFile(XMLHandler &handler, Ogre::DataStreamPtr &dataStream)
{
	XML_Parser parser = XML_ParserCreate(VNULL);
	
	try
	{
		UserData userData;
		userData.handler = &handler;
		userData.converter = boost::bind(&expat_to_local, _1, _2, boost::ref(mBuffer));
		
		XML_SetUserData(parser, &userData);
		XML_SetElementHandler(parser, expatStartElementHandler, expatEndElementHandler);
		XML_SetCharacterDataHandler(parser, expatCharacterDataHandler);
		
		const size_t MaxBufferSize = 64 * 1024;
		
		VBOOL done;
		do 
		{
			void *buffer = XML_GetBuffer(parser, MaxBufferSize);
			if (!buffer)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"Can't allocate buffer while parse XML file.",
							"ExpatParser::parseXMLFile");
			}
			
			size_t count = dataStream->read(buffer, MaxBufferSize);
			done = dataStream->eof();
			
			if (XML_ParseBuffer(parser, count, done) != XML_STATUS_OK)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"An error occurred while parsing XML file at line " 
							+ Ogre::StringConverter::toString(XML_GetCurrentLineNumber(parser)) +
							". Additional information: " + XML_ErrorString(XML_GetErrorCode(parser)),
							"ExpatParser::parseXMLFile");
			}
		} while (!done);
	}
	catch (Ogre::Exception &e)
	{
		XML_ParserFree(parser);
		throw;
	}
	
	XML_ParserFree(parser);
}
