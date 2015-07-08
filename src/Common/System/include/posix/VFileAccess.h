

#ifndef __VISION_FILEACCESS_H__
#define __VISION_FILEACCESS_H__


#include "IFileAccess.h"
#include <fstream>


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
	std::fstream mStream;
};


#endif	/*__VISION_FILEACCESS_H__*/
