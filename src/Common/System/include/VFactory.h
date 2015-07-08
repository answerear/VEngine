

#ifndef __VISION_FACTORY_H__
#define __VISION_FACTORY_H__


#include "VSystemConfig.h"


template <typename T>
class VFactory
{
public:
	class VDeleter
	{
	protected:
		VFactory *mFactory;

	public:
		VDeleter(VFactory *factory) : mFactory(factory)	{}
		void operator() (T *object) const
		{
			mFactory->destroyInstance(object);
		}
	};

public:
	VFactory()	{}
	virtual ~VFactory()	{}

	virtual const VString &getType() const = 0;
	virtual T *createInstance() = 0;
	virtual void destroyInstance(T *object) = 0;
};


#endif	/*__VISION_FACTORY_H__*/

