

#ifndef __VISION_ITEXT_CODEC_IMP_H__
#define __VISION_ITEXT_CODEC_IMP_H__


#include "VSystemConfig.h"


class ITextCodecImp
{
public:
	virtual VBOOL fromUnicode(const VWString &src, VSString &dst, const VString &dstCodec) = 0;
	virtual VBOOL toUnicode(const VSString &src, const VString &srcCodec, VWString &dst) = 0;
	virtual VBOOL convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec) = 0;
};


#endif	/*__VISION_ITEXT_CODEC_IMP_H__*/
