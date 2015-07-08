

#include "VTextCodecImp.h"


VBOOL VTextCodecImp::fromUnicode(const VWString &src, VSString &dst, const VString &dstCodec)
{
	return VFALSE;
}

VBOOL VTextCodecImp::toUnicode(const VSString &src, const VString &srcCodec, VWString &dst)
{
	return VFALSE;
}

VBOOL VTextCodecImp::convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec)
{
	return VFALSE;
}

VUINT32 VTextCodecImp::getCodePage(const VLocale &loc) const
{
	return 0;
}
