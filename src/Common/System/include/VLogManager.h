

#ifndef __VISION_LOG_MANAGER_H__
#define __VISION_LOG_MANAGER_H__


#include "VSystemConfig.h"
#include "VSingleton.h"


class IDataStream;


class VSYSTEM_DLL VLogManager : public VSingleton<VLogManager>
{
	friend class VSystem;

	VLogManager();
	~VLogManager();

	VLogManager(const VLogManager &);
	VLogManager &operator =(const VLogManager &);

	VBOOL startup() throw();
	VBOOL shutdown() throw();

public:
	void logMessage(VCHAR *fmt, ...);
	void logMessage(VCHAR *filename, VLONG line, VCHAR *fmt, ...);

	void exceptionMessage(VCHAR *fmt, ...);
	void exceptionMessage(VCHAR *filenem, VLONG line, VCHAR *fmt, ...);

protected:
	IDataStream	*mLogFile;
	IDataStream	*mExceptFile;
};


#define VLOG_MANAGER	VLogManager::instance()

#define V_LOG_MESSAGE(fmt, ...)		VLOG_MANAGER.logMessage(__FILE__, __LINE__, fmt, __VA_ARGS__)
#define V_EXCEPT_MESSAGE(fmt, ...)	VLOG_MANAGER.exceptionMessage(__FILE__, __LINE__, fmt, __VA_ARGS__)


#endif	/*__VISION_LOG_MANAGER_H__*/
