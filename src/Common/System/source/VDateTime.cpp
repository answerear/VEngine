

#include <time.h>
#include <sys/timeb.h>
#include "VDateTime.h"
#include "VUtility.h"


VDateTime::VDateTime()
	: mYear(0)
	, mMonth(0)
	, mDay(0)
	, mHour(0)
	, mMinute(0)
	, mSecond(0)
	, mMillisecond(0)
{

}

VDateTime::VDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
	: mYear(year)
	, mMonth(month)
	, mDay(day)
	, mHour(hour)
	, mMinute(minute)
	, mSecond(second)
	, mMillisecond(millisecond)
{

}

VUINT64 VDateTime::toMSecsSinceEpoch() const
{
	tm dt;
	dt.tm_year = mYear;
	dt.tm_mon = mMonth;
	dt.tm_mday = mDay;
	dt.tm_hour = mHour;
	dt.tm_min = mMinute;
	dt.tm_sec = mSecond;

	time_t t = mktime(&dt);
	VUINT64 time = t * 1000 + mMillisecond;
	return time;
}

VString VDateTime::toString() const
{
	VCHAR text[128];
	snprintf(text, 128, "%d-%02d-%02d %02d:%02d:02d.%03d", mYear, mMonth, mDay, mHour, mMinute, mSecond, mMillisecond);
	VString s(text);
	return s;
}

VBOOL VDateTime::operator ==(const VDateTime &other) const
{
	return (other.mYear == mYear && other.mMonth == mMonth && other.mDay == mDay
		&& other.mHour == mHour && other.mMinute == mMinute && other.mSecond == mSecond
		&& other.mMillisecond == mMinute);
}

VBOOL VDateTime::operator !=(const VDateTime &other) const
{
	return (other.mYear != mYear || other.mMonth != mMonth || other.mDay != mDay
		|| other.mHour != mHour || other.mMinute != mMinute || other.mSecond != mSecond
		|| other.mMillisecond != mMinute);
}

VBOOL VDateTime::operator <(const VDateTime &other) const
{
	return (mYear < other.mYear
		|| (mYear == other.mYear && mMonth < other.mMonth)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay < other.mDay)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay 
			&& mHour < other.mHour)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay 
			&& mHour == other.mHour && mMinute < other.mMinute)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay
			&& mHour == other.mHour && mMinute == other.mMinute && mSecond < other.mSecond)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay
			&& mHour == other.mHour && mMinute == other.mMinute && mSecond == other.mSecond
			&& mMillisecond < other.mMillisecond));
}

VBOOL VDateTime::operator <=(const VDateTime &other) const
{
	return (*this < other || *this == other);
}

VBOOL VDateTime::operator >(const VDateTime &other) const
{
	return (mYear > other.mYear
		|| (mYear == other.mYear && mMonth > other.mMonth)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay > other.mDay)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay 
			&& mHour > other.mHour)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay 
			&& mHour == other.mHour && mMinute > other.mMinute)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay
			&& mHour == other.mHour && mMinute == other.mMinute && mSecond > other.mSecond)
		|| (mYear == other.mYear && mMonth == other.mMonth && mDay == other.mDay
			&& mHour == other.mHour && mMinute == other.mMinute && mSecond == other.mSecond
			&& mMillisecond > other.mMillisecond));
}

VBOOL VDateTime::operator >=(const VDateTime &other) const
{
	return (*this > other || *this == other);
}

VDateTime VDateTime::currentDateTime()
{
	timeb timebuffer;
	ftime(&timebuffer);
	time_t time = timebuffer.time;
	VINT32 millisecond = timebuffer.millitm;
	VINT32 timezone = timebuffer.timezone * 60;
	time = time + timezone;
	tm *t = gmtime(&time);
	return VDateTime(t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, millisecond);
}

VDateTime VDateTime::currentDateTimeUTC()
{
	timeb timebuffer;
	ftime(&timebuffer);
	time_t time = timebuffer.time;
	VINT32 millisecond = timebuffer.millitm;
	time = time + timezone;
	tm *t = gmtime(&time);
	return VDateTime(t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, millisecond);
}

VUINT64 VDateTime::currentMSecsSinceEpoch()
{
	timeb timebuffer;
	ftime(&timebuffer);
	time_t time = timebuffer.time;
	VINT32 millisecond = timebuffer.millitm;
	VUINT64 t = time * 1000 + millisecond;
	return t;
}

VDateTime VDateTime::fromMSecsSinceEpoch(VUINT64 msecs)
{
	time_t s = msecs / 1000;
	VINT32 ms = msecs % 1000;
	tm *t = gmtime(&s);
	return VDateTime(t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ms);
}
