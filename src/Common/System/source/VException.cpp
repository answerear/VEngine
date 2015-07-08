

#include "VException.h"
#include "VLogManager.h"
#include <sstream>

VException::VException(VINT32 number, const VString &description, const VString &source)
	: mLine(0)
	, mNumber(number)
	, mDescription(description)
	, mSource(source)
{

}

VException::VException(VINT32 number, const VString &description, const VString &source, 
	const VCHAR *type, const VCHAR *file, VLONG line)
	: mLine(line)
	, mNumber(number)
	, mTypeName(type)
	, mDescription(description)
	, mSource(source)
	, mFile(file)
{
	VLOG_MANAGER.exceptionMessage((VCHAR*)getFullDescription().c_str());
}

VException::VException(const VException &other)
	: mLine(other.mLine)
	, mNumber(other.mNumber)
	, mTypeName(other.mTypeName)
	, mDescription(other.mDescription)
	, mSource(other.mSource)
	, mFile(other.mFile)
{

}


VException &VException::operator =(const VException &other)
{
	mLine = other.mLine;
	mNumber = other.mNumber;
	mTypeName = other.mTypeName;
	mDescription = other.mDescription;
	mSource = other.mSource;
	mFile = other.mFile;

	return *this;
}

const VString &VException::getFullDescription() const
{
	if (mFullDesc.empty())
	{
		std::ostringstream desc;
		desc << "VISION EXCEPTION(" << mNumber << ":" << mTypeName << "): "
			<< mDescription << "in" << mSource;

		if (mLine > 0)
		{
			desc << " at " << mFile << " (line " << mLine << ")";
		}

		mFullDesc = desc.str();
	}

	return mFullDesc;
}


VINT32 VException::getNumber() const throw()
{
	return mNumber;
}
