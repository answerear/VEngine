

#ifndef __VISION_MEMORY_DATA_STREAM_H__
#define __VISION_MEMORY_DATA_STREAM_H__


#include "IDataStream.h"


class VMemoryDataStream : public IDataStream
{
public:
	VMemoryDataStream(VBYTE *data, VINT32 size);
	virtual ~VMemoryDataStream();

private:
	VMemoryDataStream(const VMemoryDataStream &);
	VMemoryDataStream &operator =(const VMemoryDataStream &);

public:
	VINT32 read(VBYTE *buffer, VINT32 size);
	VINT32 write(const VBYTE *buffer, VINT32 size);

	VINT32 readString(VString &str, VCHAR delimiter = ' ');
	VINT32 writeString(const VString &str);

	VINT32 readWord(VWORD &value);
	VINT32 writeWord(VWORD value);

	VINT32 readDWord(VDWORD &value);
	VINT32 writeDWord(VDWORD value);

	VINT32 readULonglong(VUINT64 &value);
	VINT32 writeULonglong(VUINT64 value);

	void seek(VUINT32 pos);
	VUINT32 tell() const;
	VUINT32 size() const;
	VBOOL eof() const;

protected:
	VBYTE	*mData;
	VUINT32	mSize;
	VUINT32	mPos;
};


#endif	/*__VISION_MEMORY_DATA_STREAM_H__*/

