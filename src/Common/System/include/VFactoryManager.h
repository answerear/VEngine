

#ifndef __VISION_FACTORY_MANAGER_H__
#define __VISION_FACTORY_MANAGER_H__


#include "VSystemConfig.h"
#include "VException.h"
#include <map>
#include <assert.h>
#include "VFactory.h"


template <typename T>
class VFactoryManager
{
public:
	typedef std::map<VString, VFactory<T>*> VFactoryMap;

	VFactoryManager()	{}
	virtual ~VFactoryManager()	{}

	virtual void registerFactory(VFactory<T> *factory)
	{
		assert(factory);
		std::pair<typename VFactoryMap::iterator, VBOOL> inserted = 
			mFactories.insert(typename VFactoryMap::value_type(factory->getType(), factory));
		assert(inserted.second);
	}

	virtual void unregisterFactory(VFactory<T> *factory)
	{
		assert(factory);
		size_t count = mFactories.erase(factory->getType());
		assert(count == 1);
	}

	const VFactoryMap &getFactories() const
	{
		return mFactories;
	}

	virtual VBOOL hasFactory(const VString &type) const
	{
		return mFactories.find(type) != mFactories.end();
	}

	virtual VFactory<T> *findFactory(const VString &type) const
	{
		typename VFactoryMap::const_iterator itr = mFactories.find(type);
		if (itr == mFactories.end())
		{
			VISION_EXCEPT(VException::VERR_ITEM_NOT_FOUND, 
				"Can't found factory for type '" + type + "'", 
				"VFactoryManager::findFactory");
		}

		return itr->second;
	}

protected:
	VFactoryMap mFactories;
};


#endif	/*__VISION_FACTORY_MANAGER_H__*/
