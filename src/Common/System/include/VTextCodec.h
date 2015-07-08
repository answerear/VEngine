

#ifndef __VISION_TEXT_CODEC_H__
#define __VISION_TEXT_CODEC_H__


#include "VSystemConfig.h"
#include "VSingleton.h"


class ITextCodecImp;


class VSYSTEM_DLL VTextCodec : public VSingleton<VTextCodec>
{
	friend class VSystem;

	VTextCodec();
	~VTextCodec();

	VTextCodec(const VTextCodec &);
	VTextCodec &operator =(const VTextCodec &);

public:
	VSString fromUnicode(const VWString &str, const VString &codec = "ANSI");
	VWString toUnicode(const VSString &str, const VString &codec = "ANSI");

	VBOOL convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec);

	VBOOL AnsiToUtf8(const VSString &src, VSString &dst)
	{
		return convert(src, "ANSI", dst, "UTF-8");
	}

	VBOOL AnsiToUnicode(const VSString &src, VWString &dst)
	{
		dst = toUnicode(src, "ANSI");
		return (!dst.empty());
	}

	VBOOL AnsiToGB2312(const VSString &src, VSString &dst)
	{
		return convert(src, "ANSI", dst, "GB2312");
	}

	VBOOL AnsiTBig5(const VSString &src, VSString &dst)
	{
		return convert(src, "ANSI", dst, "BIG5");
	}

	VBOOL Utf8ToAnsi(const VSString &src, VSString &dst)
	{
		return convert(src, "UTF-8", dst, "ANSI");
	}

	VBOOL Utf8ToUnicode(const VSString &src, VWString &dst)
	{
		dst = toUnicode(src, "UTF-8");
		return (!dst.empty());
	}

	VBOOL Utf8ToGB2312(const VSString &src, VSString &dst)
	{
		return convert(src, "UTF-8", dst, "GB2312");
	}

	VBOOL Utf8ToBig5(const VSString &src, VSString &dst)
	{
		return convert(src, "UTF-8", dst, "BIG5");
	}

	VBOOL UnicodeToAnsi(const VWString &src, VSString &dst)
	{
		dst = fromUnicode(src, "ANSI");
		return (!dst.empty());
	}

	VBOOL UnicodeToUtf8(const VWString &src, VSString &dst)
	{
		dst = fromUnicode(src, "UTF-8");
		return (!dst.empty());
	}

	VBOOL UnicodeToGB2312(const VWString &src, VSString &dst)
	{
		dst = fromUnicode(src, "GB2312");
		return (!dst.empty());
	}

	VBOOL UnicodeToBig5(const VWString &src, VSString &dst)
	{
		dst = fromUnicode(src, "BIG5");
		return (!dst.empty());
	}

	VBOOL GB2312ToAnsi(const VSString &src, VSString &dst)
	{
		return convert(src, "GB2312", dst, "ANSI");
	}

	VBOOL GB2312ToUtf8(const VSString &src, VSString &dst)
	{
		return convert(src, "GB2312", dst, "UTF-8");
	}

	VBOOL GB2312ToUnicode(const VSString &src, VWString &dst)
	{
		dst = toUnicode(src, "GB2312");
		return (!dst.empty());
	}

	VBOOL GB2312ToBig5(const VSString &src, VSString &dst)
	{
		return convert(src, "GB2312", dst, "BIG5");
	}

	VBOOL Big5ToAnsi(const VSString &src, VSString &dst)
	{
		return convert(src, "BIG5", dst, "ANSI");
	}

	VBOOL Big5ToUtf8(const VSString &src, VSString &dst)
	{
		return convert(src, "BIG5", dst, "UTF-8");
	}

	VBOOL Big5ToUnicode(const VSString &src, VWString &dst)
	{
		dst = toUnicode(src, "BIG5");
		return (!dst.empty());
	}

	VBOOL Big5ToGB2312(const VSString &src, VSString &dst)
	{
		return convert(src, "BIG5", dst, "GB2312");
	}

private:
	ITextCodecImp	*mCodec;
};


#define VTEXT_CODEC		(VTextCodec::instance())


#endif	/*__VISION_TEXT_CODEC_H__*/
