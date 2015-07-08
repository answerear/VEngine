

#ifndef __VISION_TEXT_CODEC_IMP_H__
#define __VISION_TEXT_CODEC_IMP_H__


#include "ITextCodecImp.h"


class VTextCodecImp : public ITextCodecImp
{
public:
	VBOOL fromUnicode(const VWString &src, VSString &dst, const VString &dstCodec);
	VBOOL toUnicode(const VSString &src, const VString &srcCodec, VWString &dst);
	VBOOL convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec);

protected:
	VUINT32 getCodePage(const VString &codec) const;
};


#endif	/*__VISION_TEXT_CODEC_IMP_H__*/
