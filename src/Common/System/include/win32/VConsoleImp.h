

#ifndef __VISION_CONSOLE_IMP_H__
#define __VISION_CONSOLE_IMP_H__


#include "IConsoleImp.h"


class IFileAccess;


class VConsoleImp : public IConsoleImp
{
public:
	VConsoleImp();
	virtual ~VConsoleImp();

private:
	VConsoleImp(const VConsoleImp &);
	VConsoleImp &operator =(const VConsoleImp &);

public:
	VBOOL startup(VCHAR *filename = VNULL, VBOOL isFile = VFALSE);
	void shutdown();

	VINT32 debugOutputString(VCHAR *str);
	void flush();

protected:
	IFileAccess	*mFile;	
};


#endif	/*__VISION_CONSOLE_IMP_H__*/
