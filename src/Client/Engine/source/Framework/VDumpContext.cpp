

#include "VDumpContext.h"
#include "VConsole.h"
#include "VObject.h"
#include "VRuntimeClass.h"


VDumpContext::VDumpContext()
{

}


VDumpContext::~VDumpContext()
{

}

VDumpContext &VDumpContext::dumpAsHex(VBYTE value)
{
	VCHAR str[8];
	snprintf(str, sizeof(str), "0x%02x", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VDWORD value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%04x", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VINT32 value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%08X", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VLONG value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%08X", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VINT64 value)
{
	VCHAR str[32];
	VUINT32 high = (value >> 32) & 0xFFFFFFFF;
	VUINT32 low = (value & 0xFFFFFFFF);
	snprintf(str, sizeof(str), "0x%08X%08X", high, low);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VUINT32 value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%08X", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VUINT64 value)
{
	VCHAR str[32];
	VUINT32 high = (value >> 32) & 0xFFFFFFFF;
	VUINT32 low = (value & 0xFFFFFFFF);
	snprintf(str, sizeof(str), "0x%08X%08X", high, low);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VWORD value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%08X", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::dumpAsHex(VINT16 value)
{
	VCHAR str[8];
	snprintf(str, sizeof(str), "0x%04X", value);
	outputString(str);
	return (*this);
}

void VDumpContext::flush()
{
	VCONSOLE.flush();
}

void VDumpContext::hexDump(VCHAR *line, VBYTE *data, VINT32 bytes, VINT32 width)
{
	outputString(line);
	outputString("\n");

	VINT32 i = 0;
	VINT32 index = 0;
	for (i = 0; i < width; ++i)
	{
		VCHAR str[8];
		snprintf(str, sizeof(str), "0x%02X", data[index]);
		outputString(str);
		++index;

		if (index >= bytes)
		{
			outputString("\n");
			break;
		}

		if (i == width)
		{
			outputString("\n");
		}
		else
		{
			outputString(" ");
		}
	}
}

VDumpContext &VDumpContext::operator <<(const VObject *object)
{
	VINT32 bytes = object->getRuntimeClass()->m_nObjectSize;
	VBYTE *data = new VBYTE[bytes];
	memcpy(data, object, bytes);
	hexDump("Object data :", data, bytes, 16);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(const VObject &object)
{
	VINT32 bytes = object.getRuntimeClass()->m_nObjectSize;
	VBYTE *data = new VBYTE[bytes];
	memcpy(data, &object, bytes);
	hexDump("Object data :", data, bytes, 16);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(const VCHAR *str)
{
	outputString((VCHAR*)str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(const void *ptr)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "0x%08p", ptr);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VBYTE value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%u", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VWORD value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%u", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VDWORD value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%u", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VINT32 value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%d", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VDOUBLE value)
{
	VCHAR str[64];
	snprintf(str, sizeof(str), "%f", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VFLOAT value)
{
	VCHAR str[64];
	snprintf(str, sizeof(str), "%f", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VLONG value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%d", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VUINT32 value)
{
	VCHAR str[16];
	snprintf(str, sizeof(str), "%u", value);
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VINT64 value)
{
	VCHAR str[64];
#if defined PLATFORM_WIN32
	snprintf(str, sizeof(str), "%I64d", value);
#else
	snprintf(str, sizeof(str), "%lld", value);
#endif
	outputString(str);
	return (*this);
}

VDumpContext &VDumpContext::operator <<(VUINT64 value)
{
	VCHAR str[64];
#if defined PLATFORM_WIN32
	snprintf(str, sizeof(str), "%I64u", value);
#else
	snprintf(str, sizeof(str), "%llu", value);
#endif
	outputString(str);
	return (*this);
}

void VDumpContext::outputString(VCHAR *str)
{
	VCONSOLE.debugOutputString(str);
}
