

#ifndef __VISION_IFILE_ACCESS_H__
#define __VISION_IFILE_ACCESS_H__


#include "VSystemConfig.h"


class IFileAccess
{
public:
	enum OpenMode
	{
		modeCreate		= 1,	///< ����ģʽ������ļ������򸲸�ԭ�ļ�����
		modeRead		= 2,	///< ��ģʽ
		modeWrite		= 4,	///< дģʽ
		modeTruncate	= 8,	///< ���ڵ��ļ��������
		modeExisting	= 16	///< ���Ѵ����ļ�
	};

	enum SeekMethod
	{
		begin = 0,
		current = 1,
		end = 2
	};

public:
	virtual VBOOL open(const VCHAR *filename, VDWORD flags) = 0;
	virtual void close() = 0;

	virtual void flush() = 0;

	virtual VINT32 read(VBYTE *buffer, VINT32 size) = 0;
	virtual VINT32 write(const VBYTE *buffer, VINT32 size) = 0;
	
	virtual void seek(VINT32 offset, VDWORD from) = 0;
	virtual VINT32 tell() const = 0;
	virtual VUINT32 size() const = 0;
	virtual VBOOL eof() const = 0;
};


#endif	/*__VISION_IFILE_ACCESS_H__*/
