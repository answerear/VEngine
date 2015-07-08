

#ifndef __VISION_DATE_TIME_H__
#define __VISION_DATE_TIME_H__


#include "VSystemConfig.h"


class VSYSTEM_DLL VDateTime
{
public:
	VDateTime();
	VDateTime(VINT32 year, VINT32 month, VINT32 day, VINT32 hour, VINT32 minute, VINT32 second, VINT32 millisecond);

	VUINT64 toMSecsSinceEpoch() const; 
	VString toString() const;

	VBOOL operator ==(const VDateTime &other) const;
	VBOOL operator !=(const VDateTime &oter) const;
	VBOOL operator <(const VDateTime &other) const;
	VBOOL operator <=(const VDateTime &other) const;
	VBOOL operator >(const VDateTime &other) const;
	VBOOL operator >=(const VDateTime &other) const;

public:
	static VDateTime currentDateTime();
	static VDateTime currentDateTimeUTC();
	static VUINT64 currentMSecsSinceEpoch();
	static VDateTime fromMSecsSinceEpoch(VUINT64 msecs);

public:
	VINT32 year() const { return mYear; }
	VINT32 month() const { return mMonth; }
	VINT32 day() const { return mDay; }
	VINT32 hour() const { return mHour; }
	VINT32 minute() const { return mMinute; }
	VINT32 second() const { return mSecond; }
	VINT32 millisecond() const { return mMillisecond; }

private:
	VINT32 mYear;
	VINT32 mMonth;
	VINT32 mDay;
	VINT32 mHour;
	VINT32 mMinute;
	VINT32 mSecond;
	VINT32 mMillisecond;
};


#endif	/*__VISION_DATE_TIME_H__*/
