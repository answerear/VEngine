

#ifndef __VISION_XML_NODE_H__
#define __VISION_XML_NODE_H__


#include "VEngineConfig.h"
#include "VisionXMLAttributes.h"
#include <OgreIteratorWrappers.h>
#include <boost/call_traits.hpp>


class XMLNode
{
	typedef std::vector<XMLNode*> XMLNodeVector;
public:
	typedef Ogre::ConstVectorIterator<XMLNodeVector> ChildrenIterator;

	XMLNode(const VString &name);
	virtual ~XMLNode();

	XMLNode *getParent() const
	{
		return mParent;
	}

	const VString &getName() const
	{
		return mName;
	}

	const XMLAttributes &getAttributes() const
	{
		return mAttributes;
	}

	XMLAttributes &attributes()
	{
		return mAttributes;
	}

	template <typename T>
	typename helper<T>::result_type getAttribute(const VString &name) const
	{
		return mAttributes.getValueAs<T>(name);
	}

	template <typename T>
	typename helper<T>::result_type getAttribute(const VString &name, typename boost::call_traits<T>::param_type def) const
	{
		return mAttributes.getValueAs<T>(name, def);
	}

	template <class T>
	void setAttribute(const VString &name, T val)
	{
		mAttributes.setValueAs(name, val);
	}

	template <class T>
	void setAttribute(const VString &name, T val, typename boost::call_traits<T>::param_type def)
	{
		mAttributes.setValueAs(name, val, def);
	}

	void setAttribute(const VString &name, const VString &val)
	{
		mAttributes.setValueAs(name, val);
	}

	void setAttribute(const VString &name, const VString &val, const VString &def)
	{
		mAttributes.setValueAs(name, val, def);
	}

	void setTextContent(const VString &textContent);
	const VString &getTextContent() const;

	XMLNode *createChild(const VString &name);
	void addChild(XMLNode *child);
	void removeChild(size_t index);
	void removeAllChildren();
	size_t getChildCount() const;
	XMLNode *getChild(size_t index) const;
	ChildrenIterator getChildrenIterator() const;

protected:
	XMLNode			*mParent;
	VString			mName;
	VString			mTextContent;
	XMLAttributes	mAttributes;
	XMLNodeVector	mChildren;
};


#endif	/*__VISION_XML_NODE_H__*/
