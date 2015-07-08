

#ifndef __VISION_SINGLETON_H__
#define __VISION_SINGLETON_H__


#include "VSystemConfig.h"
#include "VAssert.h"


#if defined PLATFORM_WIN32
#pragma warning(disable:4661)
#endif


template<typename T>
class VSingleton
{
	friend class VSystem;

	VSingleton(const VSingleton<T> &);
	VSingleton &operator =(const VSingleton<T> &);

public:
	VSingleton()
	{
		V_ASSERT(VNULL == mInstance);
		mInstance = static_cast<T*>(this);
	}

	~VSingleton()
	{
		V_ASSERT(VNULL != mInstance);
		mInstance = VNULL;
	}

	static T &instance()
	{
		V_ASSERT(VNULL != mInstance);
		return (*mInstance);
	}

protected:
	static T *mInstance;
};


#endif	/*__VISION_SINGLETON_H__*/
