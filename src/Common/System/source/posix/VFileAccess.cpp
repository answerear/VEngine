

#include "VFileAccess.h"


VFileAccess::VFileAccess()
{

}

VFileAccess::~VFileAccess()
{
	close();
}

VBOOL VFileAccess::open(const VCHAR *filename, VDWORD flags)
{


	return VTRUE;
}

void VFileAccess::close()
{

}

void VFileAccess::flush()
{

}

VINT32 VFileAccess::read(VBYTE *buffer, VINT32 size)
{

	return 0;
}

VINT32 VFileAccess::write(const VBYTE *buffer, VINT32 size)
{

	return 0;
}

void VFileAccess::seek(VINT32 offset, VDWORD from)
{

}

VINT32 VFileAccess::tell() const
{
	return 0;
}

VUINT32 VFileAccess::size() const
{

	return 0;
}

VBOOL VFileAccess::eof() const
{

	return VTRUE;
}

