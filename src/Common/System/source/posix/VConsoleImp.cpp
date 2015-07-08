

#include "VAssert.h"
#include "VConsoleImp.h"
#include "VFileAccess.h"
#include "VDateTime.h"


VConsoleImp::VConsoleImp()
	: mFile(VNULL)
{

}

VConsoleImp::~VConsoleImp()
{
	delete mFile;
	mFile = VNULL;
}

VBOOL VConsoleImp::startup(VCHAR *filename /* = VNULL */, VBOOL isFile /* = VFALSE */)
{
	if (isFile && VNULL != filename)
	{
		// 创建文件，强制输出到文件中
		mFile = new VFileAccess();
		VBOOL result = mFile->open(filename, IFileAccess::modeCreate|IFileAccess::modeWrite);
		if (!result)
		{
			return result;
		}
	}
	else
	{
		// 输出到console中，在windows是输出到output panel上
	}

	return VTRUE;
}

void VConsoleImp::shutdown()
{
	if (VNULL != mFile)
	{
		mFile->close();
		delete mFile;
		mFile = VNULL;
	}
}

VINT32 VConsoleImp::debugOutputString(VCHAR *str)
{
	VINT32 result = 0;

	VCHAR msg[MAX_BUFFER_SIZE];

	VDateTime date = VDateTime::currentDateTime();
	snprintf(msg, MAX_BUFFER_SIZE, "<%d-%d-%d %d:%d:%d.%d>---%s\n", date.year(), date.month(), date.day(),
		date.hour(), date.minute(), date.second(), date.millisecond(), str);


	if (VNULL != mFile)
	{
		// 输出到文件
		result = mFile->write((const VBYTE*)msg, strlen(msg));
	}
	else
	{
		// 输出到output panel上
		printf(msg);
	}

	return result;
}

void VConsoleImp::flush()
{
	if (VNULL != mFile)
	{
		mFile->flush();
	}
}


