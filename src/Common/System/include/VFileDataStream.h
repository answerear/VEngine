

#ifndef __VISION_FILE_DATA_STREAM_H__
#define __VISION_FILE_DATA_STREAM_H__


#include "IDataStream.h"


class IFileAccess;


class VFileDataStream : public IDataStream
{
public:
	static const VDWORD modeCreate;
	static const VDWORD modeRead;
	static const VDWORD modeWrite;
	static const VDWORD modeTruncate;
	static const VDWORD modeExisting;

	static const VDWORD begin;
	static const VDWORD current;
	static const VDWORD end;

public:
	VFileDataStream();
	VFileDataStream(const VCHAR *filename, VDWORD flags);
	virtual ~VFileDataStream();

private:
	VFileDataStream(const VFileDataStream &);
	VFileDataStream &operator =(const VFileDataStream &);

public:
	VBOOL open(const VCHAR *filename, VDWORD flags);
	void close();

	void flush();

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

	void seekToBegin();
	void seekToEnd();
	void seek(VINT32 offset, VDWORD from);

	VUINT32 tell() const;
	VUINT32 size() const;
	VBOOL eof() const;

	static VBOOL createDirectory(const VString &dir);
	static VBOOL deleteDirectory(const VString &dir);
	static VBOOL createFile(const VString &filePath);
	static VBOOL deleteFile(const VString &filePath);
	static VBOOL moveFile(const VString &srcFile, const VString &dstFile);
	static VBOOL copyFile(const VString &srcFile, const VString &dstFile);

protected:
	void seek(VUINT32 pos);

private:
	IFileAccess	*mFileAccess;
};


#endif	/*__VISION_FILE_DATA_STREAM_H__*/
