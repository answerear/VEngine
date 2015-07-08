

#ifndef __VISION_DUMP_CONTEXT_H__
#define __VISION_DUMP_CONTEXT_H__


#include "VEngineConfig.h"


class VObject;


class VENGINE_DLL VDumpContext
{
public:
	VDumpContext();
	virtual ~VDumpContext();

	VDumpContext &dumpAsHex(VBYTE value);
	VDumpContext &dumpAsHex(VDWORD value);
	VDumpContext &dumpAsHex(VINT32 value);
	VDumpContext &dumpAsHex(VLONG value);
	VDumpContext &dumpAsHex(VINT64 value);
	VDumpContext &dumpAsHex(VUINT32 value);
	VDumpContext &dumpAsHex(VUINT64 value);
	VDumpContext &dumpAsHex(VWORD value);
	VDumpContext &dumpAsHex(VINT16 value);

	void flush();

	void hexDump(VCHAR *line, VBYTE *data, VINT32 bytes, VINT32 width);

	VDumpContext &operator <<(const VObject *object);
	VDumpContext &operator <<(const VObject &object);

	VDumpContext &operator <<(const VCHAR *str);
	VDumpContext &operator <<(const void *ptr);
	VDumpContext &operator <<(VBYTE value);
	VDumpContext &operator <<(VWORD value);
	VDumpContext &operator <<(VDWORD value);
	VDumpContext &operator <<(VINT32 value);
	VDumpContext &operator <<(VDOUBLE value);
	VDumpContext &operator <<(VFLOAT value);
	VDumpContext &operator <<(VLONG value);
	VDumpContext &operator <<(VUINT32 value);
	VDumpContext &operator <<(VINT64 value);
	VDumpContext &operator <<(VUINT64 value);

protected:
	VDumpContext(const VDumpContext &);
	VDumpContext &operator =(const VDumpContext &);

	void outputString(VCHAR *str);
};


#endif	/*__VISION_DUMP_CONTEXT_H__*/
