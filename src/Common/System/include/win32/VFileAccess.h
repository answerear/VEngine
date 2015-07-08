

#ifndef __VISION_FILE_ACCESS_H__
#define __VISION_FILE_ACCESS_H__


#include "IFileAccess.h"


class VFileAccess : public IFileAccess
{
public:
	VFileAccess();
	virtual ~VFileAccess();

private:
	VFileAccess(const VFileAccess &);
	VFileAccess &operator =(const VFileAccess &);

public:
	VBOOL open(const VCHAR *filename, VDWORD flags);
	void close();

	void flush();

	VINT32 read(VBYTE *buffer, VINT32 size);
	VINT32 write(const VBYTE *buffer, VINT32 size);

	void seek(VINT32 offset, VDWORD from);
	VINT32 tell() const;
	VUINT32 size() const;
	VBOOL eof() const;

protected:
	HANDLE	mFile;
	VINT32	mCurPos;
};


#endif	/*__VISION_FILE_ACCESS_H__*/
