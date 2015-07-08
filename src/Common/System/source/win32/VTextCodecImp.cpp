

#include <windows.h>
#include <algorithm>
#include <vector>
#include "VTextCodecImp.h"


VBOOL VTextCodecImp::fromUnicode(const VWString &src, VSString &dst, const VString &dstCodec)
{
	if (src.empty())
	{
		return VFALSE;
	}

	VUINT32 nCodePage = getCodePage(dstCodec);
	const WCHAR *lpWideCharStr = src.c_str();
	VINT32 cchWideChar = src.length() + 1;

	// 获取多字节字符串长度
	VINT32 cbMultiByte = ::WideCharToMultiByte(nCodePage, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL);

	// 转换多字节字符串到宽字节字符串
	VCHAR *str = new VCHAR[cbMultiByte+1];
	VINT32 cchConvertedMultiByte = ::WideCharToMultiByte(nCodePage, 0, lpWideCharStr, -1, str, cbMultiByte, NULL, NULL);
	if (cbMultiByte != cchConvertedMultiByte)
	{
		return VFALSE;
	}

	dst = str;
	delete []str;

	return VTRUE;
}

VBOOL VTextCodecImp::toUnicode(const VSString &src, const VString &srcCodec, VWString &dst)
{
	if (src.empty())
	{
		return VFALSE;
	}

	VUINT32 nCodePage = getCodePage(srcCodec);
	const VCHAR *lpMultiByteStr = src.c_str();
	VINT32 cbMultiByte = src.length() + 1;

	// 获取宽字节字符串长度
	VINT32 cchWideChar = ::MultiByteToWideChar(nCodePage, 0, lpMultiByteStr, -1, NULL, 0);

	// 转换多字节字符串到宽字节字符串
	VWCHAR *str = new VWCHAR[cchWideChar+1];
	VINT32 cchConvertedWideChar = ::MultiByteToWideChar(nCodePage, 0, lpMultiByteStr, -1, str, cchWideChar);
	if (cchWideChar != cchConvertedWideChar)
	{
		return VFALSE;
	}

	dst = str;
	delete []str;

	return VTRUE;
}

VBOOL VTextCodecImp::convert(const VSString &src, const VString &srcCodec, VSString &dst, const VString &dstCodec)
{
	VWString tmp;
	if (!toUnicode(src, srcCodec, tmp))
	{
		return VFALSE;
	}

	if (!fromUnicode(tmp, dst, dstCodec))
	{
		return VFALSE;
	}

	return VTRUE;
}

VUINT32 VTextCodecImp::getCodePage(const VString &codec) const
{
	VUINT32 cp;

	VString s = codec;
	std::transform(s.begin(), s.end(), s.begin(), toupper);

	if (s == "" || s == "ANSI")
	{
		cp = CP_ACP;
	}
	else if (s == "GB2312")
	{
		cp = 936;
	}
	else if (s == "UTF-8")
	{
		cp = 65001;
	}
	else if (s == "UTF-7")
	{
		cp = 65000;
	}
	else if (s == "UTF-16")
	{
		cp = 1200;
	}
	else if (s == "BIG5")
	{
		cp = 950;
	}
	else if (s == "UTF-32")
	{
		cp = 12000;
	}

	return cp;
}
