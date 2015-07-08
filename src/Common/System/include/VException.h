

#ifndef __VISION_EXCEPTION_H__
#define __VISION_EXCEPTION_H__


#include "VSystemConfig.h"
#include <exception>


class VSYSTEM_DLL VException : public std::exception
{
public:
	enum VExceptionCodes 
	{
		VERR_INVALID_PARAM,			/**< 参数错误 */
		VERR_FILE_NOT_FOUND,		/**< 文件不存在 */
		VERR_IO_ERROR,				/**< IO操作异常 */
		VERR_ITEM_NOT_FOUND,		/**< 项不存在 */
		VERR_ITEM_DUPLICATED,		/**< 重复项 */
		VERR_NOT_IMPLEMENTED,		/**< 没有实现 */
		VERR_INTERNAL_ERROR,		/**< 内部错误 */
	};

public:
	VException(VINT32 number, const VString &description, const VString &source);
	VException(VINT32 number, const VString &description, const VString &source, const VCHAR *type, const VCHAR *file, VLONG line);
	VException(const VException &other);
	~VException() throw() {}

	VException &operator =(const VException &other);

	virtual const VString &getFullDescription() const;
	virtual VINT32 getNumber() const throw();
	virtual const VString &getSource() const { return mSource; }
	virtual const VString &getFile() const { return mFile; }
	virtual VLONG getLine() const { return mLine; }
	virtual const VString &getDescription() const { return mDescription; }
	const VCHAR *what() const throw() { return getFullDescription().c_str(); }

protected:
	VLONG mLine;
	VINT32 mNumber;
	VString mTypeName;
	VString mDescription;
	VString mSource;
	VString mFile;
	mutable VString mFullDesc;
};


template <VINT32 num>
struct VExceptionCodeType
{
	enum { number = num };
};


/**
 * @class VInvalidParamException
 */
class VSYSTEM_DLL VInvalidParamException : public VException
{
public:
	VInvalidParamException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "InvalidParamException", file, line) {}
};

/**
 * @class VFileNotFoundException
 */
class VSYSTEM_DLL VFileNotFoundException : public VException
{
public:
	VFileNotFoundException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "FileNotFoundException", file, line) {}
};

/** 
 * @class VIOException
 */
class VSYSTEM_DLL VIOException : public VException
{
public:
	VIOException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "IOExceptionException", file, line) {}
};

/** 
 * @class VItemNotFoundException
 */
class VSYSTEM_DLL VItemNotFoundException : public VException
{
public:
	VItemNotFoundException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "ItemNotFoundException", file, line) {}
};

/** 
 * @class VItemDuplicatedException
 */
class VSYSTEM_DLL VItemDuplicatedException : public VException
{
public:
	VItemDuplicatedException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "ItemDuplicatedException", file, line) {}
};

/** 
 * @class VUnimplementedException
 */
class VSYSTEM_DLL VUnimplementedException : public VException
{
public:
	VUnimplementedException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "UnimplementedException", file, line) {}
};

/** 
 * @class VInternalException
 */
class VSYSTEM_DLL VInternalException : public VException
{
public:
	VInternalException(VINT32 number, const VString &description, const VString &source, const VCHAR *file, VLONG line)
		: VException(number, description, source, "VInternalException", file, line) {}
};



class VSYSTEM_DLL VExceptionFactory
{
private:
	VExceptionFactory() {}
public:
	static VInvalidParamException create(
		VExceptionCodeType<VException::VERR_INVALID_PARAM> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VInvalidParamException(code.number, desc, src, file, line);
	}

	static VFileNotFoundException create(
		VExceptionCodeType<VException::VERR_FILE_NOT_FOUND> code, 
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VFileNotFoundException(code.number, desc, src, file, line);
	}

	static VIOException create(
		VExceptionCodeType<VException::VERR_IO_ERROR> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VIOException(code.number, desc, src, file, line);
	}

	static VItemNotFoundException create(
		VExceptionCodeType<VException::VERR_ITEM_NOT_FOUND> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VItemNotFoundException(code.number, desc, src, file, line);
	}

	static VItemDuplicatedException create(
		VExceptionCodeType<VException::VERR_ITEM_DUPLICATED> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VItemDuplicatedException(code.number, desc, src, file, line);
	}

	static VUnimplementedException create(
		VExceptionCodeType<VException::VERR_NOT_IMPLEMENTED> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VUnimplementedException(code.number, desc, src, file, line);
	}
	
	static VInternalException create(
		VExceptionCodeType<VException::VERR_INTERNAL_ERROR> code,
		const VString &desc, const VString &src, const VCHAR *file, VLONG line)
	{
		return VInternalException(code.number, desc, src, file, line);
	}	
};


#ifndef VISION_EXCEPT
#define VISION_EXCEPT(num, desc, src) throw VExceptionFactory::create(\
	VExceptionCodeType<num>(), desc, src, __FILE__, __LINE__);
#endif


#endif	/*__VISION_EXCEPTION_H__*/

