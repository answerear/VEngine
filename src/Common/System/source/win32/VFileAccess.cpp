

#include <windows.h>

#include "VFileAccess.h"


VFileAccess::VFileAccess()
	: mFile(INVALID_HANDLE_VALUE)
{

}

VFileAccess::~VFileAccess()
{
	close();
}

VBOOL VFileAccess::open(const VCHAR *filename, VDWORD flags)
{
	if (INVALID_HANDLE_VALUE != mFile)
	{
		return VFALSE;
	}

	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	if (flags & modeCreate)
	{
		dwCreationDisposition |= OPEN_ALWAYS;

		::DeleteFile(filename);
	}
	else if (flags & modeExisting)
	{
		dwCreationDisposition |= OPEN_EXISTING;
	}
	else if (flags & modeTruncate)
	{
		dwCreationDisposition |= TRUNCATE_EXISTING;
	}

	if (flags & modeRead)
	{
		dwDesiredAccess |= GENERIC_READ;
		dwShareMode |= FILE_SHARE_READ;
		dwCreationDisposition |= OPEN_ALWAYS;
	}
	else if (flags & modeWrite)
	{
		dwDesiredAccess |= GENERIC_WRITE;
		dwShareMode |= FILE_SHARE_WRITE;
		dwCreationDisposition |= OPEN_ALWAYS;
	}

	mFile = ::CreateFile(
		filename, 
		dwDesiredAccess, 
		dwShareMode, 
		NULL, 
		dwCreationDisposition, 
		dwFlagsAndAttributes, 
		NULL);

	if (INVALID_HANDLE_VALUE == mFile)
		return VFALSE;

	mCurPos = 0;

	return VTRUE;
}

void VFileAccess::close()
{
	if (INVALID_HANDLE_VALUE != mFile)
	{
		::CloseHandle(mFile);
		mFile = INVALID_HANDLE_VALUE;
	}
}

void VFileAccess::flush()
{
	if (INVALID_HANDLE_VALUE != mFile)
	{
		::FlushFileBuffers(mFile);
	}
}

VINT32 VFileAccess::read(VBYTE *buffer, VINT32 size)
{
	if (INVALID_HANDLE_VALUE == mFile)
	{
		return -1;
	}

	VINT32 nNumberOfBytesRead;

	BOOL bResult = ::ReadFile(mFile, buffer, (DWORD)size, (DWORD*)&nNumberOfBytesRead, NULL);

	if (!bResult || nNumberOfBytesRead != size)
	{
		return -1;
	}

	mCurPos += nNumberOfBytesRead;

	return nNumberOfBytesRead;
}

VINT32 VFileAccess::write(const VBYTE *buffer, VINT32 size)
{
	if (INVALID_HANDLE_VALUE == mFile)
	{
		return -1;
	}

	VINT32 nNumberOfBytesWritten;

	BOOL bResult = ::WriteFile(mFile, buffer, (DWORD)size, (DWORD*)&nNumberOfBytesWritten, NULL);

	if (!bResult || nNumberOfBytesWritten != size)
	{
		return -1;
	}

	mCurPos += nNumberOfBytesWritten;

	return nNumberOfBytesWritten;
}

void VFileAccess::seek(VINT32 offset, VDWORD from)
{
	if (INVALID_HANDLE_VALUE == mFile)
	{
		return;
	}

	DWORD dwNewPosition = SetFilePointer(mFile, offset, NULL, from);

	if (INVALID_SET_FILE_POINTER == dwNewPosition)
	{	
		return;
	}

	mCurPos = (VINT32)dwNewPosition;
}

VINT32 VFileAccess::tell() const
{
	return mCurPos;
}

VUINT32 VFileAccess::size() const
{
	if (INVALID_HANDLE_VALUE == mFile)
	{
		return 0;
	}

	DWORD dwFileSizeHigh = 0;
	DWORD dwSize = ::GetFileSize(mFile, &dwFileSizeHigh);
	return dwSize;
}

VBOOL VFileAccess::eof() const
{
	if (INVALID_HANDLE_VALUE == mFile)
	{
		return VTRUE;
	}

	VUINT32 s = size();
	return (s == (VUINT32)mCurPos);
}

