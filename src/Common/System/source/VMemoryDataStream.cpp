

#include "VMemoryDataStream.h"


VMemoryDataStream::VMemoryDataStream(VBYTE *data, VINT32 size)
	: mData(data)
	, mSize(size)
	, mPos(0)
{

}

VMemoryDataStream::~VMemoryDataStream()
{
	mData = VNULL;
	mSize = 0;
	mPos = 0;
}

VINT32 VMemoryDataStream::read(VBYTE *buffer, VINT32 size)
{
	VINT32 numberOfRead = 0;

	if (VNULL != mData && !eof() && VNULL != buffer && size > 0)
	{
		numberOfRead = (mPos + size > mSize ? (mSize - mPos) : size);
		memcpy(buffer, mData+mPos, numberOfRead);
		mPos += numberOfRead;
	}

	return numberOfRead;
}

VINT32 VMemoryDataStream::write(const VBYTE *buffer, VINT32 size)
{
	VINT32 numberOfWritten = 0;

	if (VNULL != mData && !eof() && VNULL != buffer && size > 0)
	{
		numberOfWritten = (mPos + size > mSize ? (mSize - mPos) : size);
		memcpy(mData+mPos, buffer, numberOfWritten);
		mPos += numberOfWritten;
	}

	return numberOfWritten;
}

VINT32 VMemoryDataStream::readString(VString &str, VCHAR delimiter /* = ' ' */ )
{
	VINT32 numberOfRead = 0;

	if (VNULL != mData && !eof())
	{
		str.clear();

		VUINT32 pos = mPos;
		while (pos < mSize && mData[pos] != delimiter)
		{
			++pos;
		}

		if (pos < mSize)
		{
			numberOfRead = pos - mPos;
			str.resize(numberOfRead);
			str.assign(mData+mPos, mData+pos);
			mPos += numberOfRead;
		}
	}

	return numberOfRead;
}

VINT32 VMemoryDataStream::writeString(const VString &str)
{
	VINT32 numberOfWritten = 0;

	if (VNULL != mData && !eof())
	{
		VINT32 length = str.length();
		numberOfWritten = (mPos + length > mSize ? (mSize - mPos) : length);
		memcpy(mData, str.c_str(), numberOfWritten);
		mPos += numberOfWritten;
	}

	return numberOfWritten;
}

VINT32 VMemoryDataStream::readWord(VWORD &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VMemoryDataStream::writeWord(VWORD value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

VINT32 VMemoryDataStream::readDWord(VDWORD &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VMemoryDataStream::writeDWord(VDWORD value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

VINT32 VMemoryDataStream::readULonglong(VUINT64 &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VMemoryDataStream::writeULonglong(VUINT64 value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

void VMemoryDataStream::seek(VUINT32 pos)
{
	if (pos < mSize)
	{
		mPos = pos;
	}
}

VUINT32 VMemoryDataStream::tell() const
{
	return mPos;
}

VUINT32 VMemoryDataStream::size() const
{
	return mSize;
}

VBOOL VMemoryDataStream::eof() const
{
	return (mPos == mSize);
}
