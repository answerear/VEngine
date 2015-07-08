

#include "VFileDataStream.h"
#include "IFileAccess.h"
#include "VSystem.h"
#include "VSystemFactory.h"


const VDWORD VFileDataStream::modeCreate = IFileAccess::modeCreate;
const VDWORD VFileDataStream::modeRead = IFileAccess::modeRead;
const VDWORD VFileDataStream::modeWrite = IFileAccess::modeWrite;
const VDWORD VFileDataStream::modeTruncate = IFileAccess::modeTruncate;
const VDWORD VFileDataStream::modeExisting = IFileAccess::modeExisting;

const VDWORD VFileDataStream::begin = IFileAccess::begin;
const VDWORD VFileDataStream::current = IFileAccess::current;
const VDWORD VFileDataStream::end = IFileAccess::end;


VFileDataStream::VFileDataStream()
	: mFileAccess(VNULL)
{
	mFileAccess = VSYSTEM_FACTORY.createFileAccess();
}

VFileDataStream::VFileDataStream(const VCHAR *filename, VDWORD flags)
{
	mFileAccess = VSYSTEM_FACTORY.createFileAccess();
	mFileAccess->open(filename, flags);
}

VFileDataStream::~VFileDataStream()
{
	delete mFileAccess;
	mFileAccess = VNULL;
}

VBOOL VFileDataStream::open(const VCHAR *filename, VDWORD flags)
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->open(filename, flags);
	}

	return VFALSE;
}

void VFileDataStream::close()
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->close();
	}
}

void VFileDataStream::flush()
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->flush();
	}
}

VINT32 VFileDataStream::read(VBYTE *buffer, VINT32 size)
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->read(buffer, size);
	}

	return 0;
}

VINT32 VFileDataStream::write(const VBYTE *buffer, VINT32 size)
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->write(buffer, size);
	}

	return 0;
}

VINT32 VFileDataStream::readString(VString &str, VCHAR delimiter /* = ' '*/ )
{
	VINT32 numberOfRead = 0;

	if (VNULL != mFileAccess && !mFileAccess->eof())
	{
		str.clear();
		VCHAR c;

		do 
		{
			VINT32 result = mFileAccess->read((VBYTE*)&c, sizeof(c));
			str += c;
			numberOfRead += result;
		} while (!mFileAccess->eof() && c != delimiter);
	}

	return numberOfRead;
}

VINT32 VFileDataStream::writeString(const VString &str)
{
	VINT32 numberOfWritten = 0;

	if (VNULL != mFileAccess && mFileAccess->eof())
	{
		VINT32 length = str.length();
		numberOfWritten =mFileAccess->write((const VBYTE*)str.c_str(), length);
	}

	return numberOfWritten;
}

VINT32 VFileDataStream::readWord(VWORD &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VFileDataStream::writeWord(VWORD value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

VINT32 VFileDataStream::readDWord(VDWORD &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VFileDataStream::writeDWord(VDWORD value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

VINT32 VFileDataStream::readULonglong(VUINT64 &value)
{
	return read((VBYTE*)&value, sizeof(value));
}

VINT32 VFileDataStream::writeULonglong(VUINT64 value)
{
	return write((const VBYTE*)&value, sizeof(value));
}

void VFileDataStream::seekToBegin()
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->seek(0, IFileAccess::begin);
	}
}

void VFileDataStream::seekToEnd()
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->seek(0, IFileAccess::end);
	}
}

void VFileDataStream::seek(VINT32 offset, VDWORD from)
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->seek(offset, from);
	}
}

VUINT32 VFileDataStream::tell() const
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->tell();
	}

	return -1;
}

VUINT32 VFileDataStream::size() const
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->size();
	}

	return 0;
}

VBOOL VFileDataStream::eof() const
{
	if (VNULL != mFileAccess)
	{
		return mFileAccess->eof();
	}

	return VTRUE;
}

void VFileDataStream::seek(VUINT32 pos)
{
	if (VNULL != mFileAccess)
	{
		mFileAccess->seek(pos, IFileAccess::begin);
	}
}


VBOOL VFileDataStream::createDirectory(const VString &dir)
{
	return VSYSTEM_FACTORY.createDirectory(dir.c_str());
}

VBOOL VFileDataStream::deleteDirectory(const VString &dir)
{
	return VSYSTEM_FACTORY.deleteDirectory(dir.c_str());
}

VBOOL VFileDataStream::createFile(const VString &filePath)
{
	return VSYSTEM_FACTORY.createFile(filePath.c_str());
}

VBOOL VFileDataStream::deleteFile(const VString &filePath)
{
	return VSYSTEM_FACTORY.deleteFile(filePath.c_str());
}

VBOOL VFileDataStream::moveFile(const VString &srcFile, const VString &dstFile)
{
	return VSYSTEM_FACTORY.moveFile(srcFile.c_str(), dstFile.c_str());
}

VBOOL VFileDataStream::copyFile(const VString &srcFile, const VString &dstFile)
{
	return VSYSTEM_FACTORY.copyFile(srcFile.c_str(), dstFile.c_str());
}
