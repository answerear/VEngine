

#ifndef __VISION_TEXT_CODEC_IMP_H__
#define __VISION_TEXT_CODEC_IMP_H__


#include "ITextCodecImp.h"


class VTextCodecImp : public ITextCodecImp
{
public:
	virtual VBOOL fromUnicode(const VWString &src, VSString &dst, const VString &dstCodec);
	virtual VBOOL toUnicode(const VSString &src, const VString &srcCodec, VWString &dst);
	virtual VBOOL convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec);
	
	VUINT32 getCodePage(const VLocale &loc) const;
};



#endif	/*__VISION_TEXT_CODEC_IMP_H__*/
