

#include <stdarg.h>
#include "VLogManager.h"
#include "VFileDataStream.h"
#include "VUtility.h"


#define MAX_BUFFER_SIZE		1024


template<> VLogManager *VSingleton<VLogManager>::mInstance = VNULL;


VLogManager::VLogManager()
	: mLogFile(VNULL)
	, mExceptFile(VNULL)
{

}

VLogManager::~VLogManager()
{
	delete mLogFile;
	delete mExceptFile;
}

VBOOL VLogManager::startup() throw()
{
	mLogFile = new VFileDataStream("system.log", VFileDataStream::modeCreate|VFileDataStream::modeWrite);
	if (VNULL == mLogFile)
	{
		return VFALSE;
	}

	mExceptFile = new VFileDataStream("exception.log", VFileDataStream::modeCreate|VFileDataStream::modeWrite);
	if (VNULL == mExceptFile)
	{
		delete mLogFile;
		return VFALSE;
	}

	return VTRUE;
}

VBOOL VLogManager::shutdown() throw()
{
	delete mLogFile;
	mLogFile = VNULL;

	delete mExceptFile;
	mExceptFile = VNULL;

	return VTRUE;
}

void VLogManager::logMessage(VCHAR *fmt, ...)
{
	if (VNULL != mLogFile)
	{
		VCHAR buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, MAX_BUFFER_SIZE, fmt, vl);
		va_end(vl);

		VINT32 size = strlen(buffer);
		size = (size >= MAX_BUFFER_SIZE - 1 ? MAX_BUFFER_SIZE - 1 : size);
		buffer[size] = '\n';

		mLogFile->write((const VBYTE*)buffer, size+1);
	}
}

void VLogManager::logMessage(VCHAR *filename, VLONG line, VCHAR *fmt, ...)
{
	if (VNULL != mLogFile)
	{
		VCHAR msg[MAX_BUFFER_SIZE];
		snprintf(msg, 1024, "[%s] [%l]", filename, line);

		VCHAR buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, 1024, fmt, vl);
		va_end(vl);

		strcat(msg, buffer);
		
		VINT32 size = strlen(msg);
		size = (size >= MAX_BUFFER_SIZE - 1 ? MAX_BUFFER_SIZE - 1 : size);
		msg[size] = '\n';

		mLogFile->write((const VBYTE *)msg, size+1);
	}
}

void VLogManager::exceptionMessage(VCHAR *fmt, ...)
{
	if (VNULL != mExceptFile)
	{
		VCHAR buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, MAX_BUFFER_SIZE, fmt, vl);
		va_end(vl);

		VINT32 size = strlen(buffer);
		size = (size >= MAX_BUFFER_SIZE - 1 ? MAX_BUFFER_SIZE - 1 : size);
		buffer[size] = '\n';

		mExceptFile->write((const VBYTE *)buffer, size+1);
	}
}

void VLogManager::exceptionMessage(VCHAR *filename, VLONG line, VCHAR *fmt, ...)
{
	if (VNULL != mExceptFile)
	{
		VCHAR msg[MAX_BUFFER_SIZE];
		snprintf(msg, 1024, "[%s] [%l]", filename, line);

		VCHAR buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		va_list vl;
		va_start(vl, fmt);
		vsnprintf(buffer, 1024, fmt, vl);
		va_end(vl);

		strcat(msg, buffer);

		VINT32 size = strlen(msg);
		size = (size >= MAX_BUFFER_SIZE - 1 ? MAX_BUFFER_SIZE - 1 : size);
		msg[size] = '\n';

		mExceptFile->write((const VBYTE *)msg, size+1);
	}
}
