

#ifndef __VISION_IDATA_STREAM_H__
#define __VISION_IDATA_STREAM_H__


#include "VSystemConfig.h"


class IDataStream
{
public:
	virtual VINT32 read(VBYTE *buffer, VINT32 size) = 0;
	virtual VINT32 write(const VBYTE *buffer, VINT32 size) = 0;

	virtual VINT32 readString(VString &str, VCHAR delimiter = ' ') = 0;
	virtual VINT32 writeString(const VString &str) = 0;

	virtual VINT32 readWord(VWORD &value) = 0;
	virtual VINT32 writeWord(VWORD value) = 0;

	virtual VINT32 readDWord(VDWORD &value) = 0;
	virtual VINT32 writeDWord(VDWORD value) = 0;

	virtual VINT32 readULonglong(VUINT64 &value) = 0;
	virtual VINT32 writeULonglong(VUINT64 value) = 0;

	virtual void seek(VUINT32 pos) = 0;
	virtual VUINT32 tell() const = 0;
	virtual VUINT32 size() const = 0;
	virtual VBOOL eof() const = 0;
};


#endif	/*__VISION_IDATA_STREAM_H__*/
