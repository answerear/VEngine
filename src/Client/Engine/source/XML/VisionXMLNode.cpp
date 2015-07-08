

#include "VisionXMLNode.h"


XMLNode::XMLNode(const VString &name)
	: mParent(VNULL)
	, mName(name)
	, mTextContent()
	, mAttributes()
	, mChildren()
{

}

XMLNode::~XMLNode()
{
	for (XMLNodeVector::const_iterator itr = mChildren.begin(); itr != mChildren.end(); ++itr)
	{
		XMLNode *child = *itr;
		child->mParent = VNULL;
		delete child;
	}
}

void XMLNode::setTextContent(const VString &textContent)
{
	mTextContent = textContent;
}

const VString &XMLNode::getTextContent() const
{
	return mTextContent;
}

XMLNode *XMLNode::createChild(const VString &name)
{
	std::auto_ptr<XMLNode> child(new XMLNode(name));
	addChild(child.get());
	return child.release();
}

void XMLNode::addChild(XMLNode *child)
{
	assert(child);
	assert(child->getParent() == VNULL);
	assert(std::find(mChildren.begin(), mChildren.end(), child) == mChildren.end());
	mChildren.push_back(child);
	child->mParent = this;
}

void XMLNode::removeChild(size_t index)
{
	if (index >= mChildren.size())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The specified index is out of range for this XMLNode",
			"XMLNode::removeChild");
	}

	XMLNodeVector::iterator itr = mChildren.begin();
	std::advance(itr, index);

	XMLNode *child = *itr;
	mChildren.erase(itr);
	child->mParent = VNULL;
	delete child;
}

void XMLNode::removeAllChildren()
{
	for (XMLNodeVector::const_iterator itr = mChildren.begin(); itr != mChildren.end(); ++itr)
	{
		XMLNode *child = *itr;
		child->mParent = VNULL;
		delete child;
	}
	mChildren.clear();
}

size_t XMLNode::getChildCount() const
{
	return mChildren.size();
}

XMLNode *XMLNode::getChild(size_t index) const
{
	if (index >= mChildren.size())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"The specified index is out of range for this XMLNode",
			"XMLNode::getChild");
	}

	XMLNodeVector::const_iterator itr = mChildren.begin();
	std::advance(itr, index);

	XMLNode* child = *itr;
	return child;
}

XMLNode::ChildrenIterator XMLNode::getChildrenIterator() const
{
	return ChildrenIterator(mChildren.begin(), mChildren.end());
}
