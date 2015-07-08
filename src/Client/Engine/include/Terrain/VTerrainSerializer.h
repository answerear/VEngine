

#ifndef __VISION_TERRAIN_SERIALIZER_H__
#define __VISION_TERRAIN_SERIALIZER_H__


#include "VisionXMLHandler.h"


class VTerrainInfo;


class VTerrainXMLHandler : public XMLHandler
{
public:
	VTerrainXMLHandler(VTerrainInfo *info);
	~VTerrainXMLHandler();

private:
	VTerrainXMLHandler(const VTerrainXMLHandler &);
	VTerrainXMLHandler &operator =(const VTerrainXMLHandler &);

public:
	void startElement(const VString &element, const XMLAttributes &attributes);

protected:
	VTerrainInfo *mInfo;
};


#endif	/*__VISION_TERRAIN_SERIALIZER_H__*/
