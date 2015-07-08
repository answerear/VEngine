

#include "VTextCodec.h"
#include "ITextCodecImp.h"
#include "VSystem.h"
#include "VSystemFactory.h"
#include "VAssert.h"


template<> VTextCodec *VSingleton<VTextCodec>::mInstance = VNULL;


VTextCodec::VTextCodec()
	: mCodec(VNULL)
{
	mCodec = VSYSTEM_FACTORY.createTextCodec();
}

VTextCodec::~VTextCodec()
{
	delete mCodec;
	mCodec = VNULL;
}

VSString VTextCodec::fromUnicode(const VWString &str, const VString &codec /* = "ANSI" */)
{
	V_ASSERT(mCodec);
	VSString dst;
	mCodec->fromUnicode(str, dst, codec);
	return dst;
}

VWString VTextCodec::toUnicode(const VSString &str, const VString &codec /* = "ANSI" */)
{
	V_ASSERT(mCodec);
	VWString dst;
	mCodec->toUnicode(str, codec, dst);
	return dst;
}

VBOOL VTextCodec::convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec)
{
	V_ASSERT(mCodec);
	return mCodec->convert(src, srcCodec, dst, dstCodec);
}
