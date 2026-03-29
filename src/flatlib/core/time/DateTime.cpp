// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/time/TimeBase.h"
#include	"flatlib/core/time/DateTime.h"
#include	"flatlib/core/platform/WinString.h"

#if FL_OS_WINDOWS
# include	"flatlib/core/platform/WindowsAPI.h"
#endif

#if FL_OS_UNIX
# include	<time.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace time {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_OS_WINDOWS
void	SystemTimeToDateTime( DateTime& datetime, const SYSTEMTIME& system_time )
{
	datetime.Year= system_time.wYear;
	datetime.Month= system_time.wMonth;
	datetime.Day= system_time.wDay;
	datetime.Hour= system_time.wHour;
	datetime.Minute= system_time.wMinute;
	datetime.Second= system_time.wSecond;
	datetime.Milliseconds= system_time.wMilliseconds;
	datetime.DayOfWeek= system_time.wDayOfWeek;
}
#endif


#if FL_OS_UNIX
void	SystemTimeToDateTime( DateTime& datetime, const struct tm& system_time )
{
	datetime.Year= system_time.tm_year + 1900;
	datetime.Month= system_time.tm_mon + 1;
	datetime.Day= system_time.tm_mday;
	datetime.Hour= system_time.tm_hour;
	datetime.Minute= system_time.tm_min;
	datetime.Second= system_time.tm_sec;
	datetime.Milliseconds= 0;
	datetime.DayOfWeek= system_time.tm_wday;
}
#endif


//-----------------------------------------------------------------------------

void	GetLocalTime( DateTime& datetime )
{
#if FL_OS_WINDOWS
	SYSTEMTIME	local_time;
	::GetLocalTime( &local_time );
	SystemTimeToDateTime( datetime, local_time );
#elif FL_OS_LINUX || FL_OS_DARWIN || FL_OS_CS2
	time_t	system_time;
	::time( &system_time );
	struct tm	local_time;
	::localtime_r( &system_time, &local_time );
	SystemTimeToDateTime( datetime, local_time );
#elif FL_OS_UNIX
	time_t	system_time;
	::time( &system_time );
	struct tm	local_time;
	::localtime_s( &system_time, &local_time );
	SystemTimeToDateTime( datetime, local_time );
#else
	FL_ASSERT( 0 );
#endif
}


void  GetUTC( DateTime& datetime )
{
#if FL_OS_WINDOWS
	SYSTEMTIME	local_time;
	::GetSystemTime( &local_time );
	SystemTimeToDateTime( datetime, local_time );
#elif FL_OS_LINUX || FL_OS_DARWIN || FL_OS_CS2
	time_t	curtime;
	::time( &curtime );
	struct tm	local_time;
	::gmtime_r( &curtime, &local_time );
	SystemTimeToDateTime( datetime, local_time );
#elif FL_OS_UNIX
	time_t	curtime;
	::time( &curtime );
	struct tm	local_time;
	::gmtime_s( &curtime, &local_time );
	SystemTimeToDateTime( datetime, local_time );
#else
	FL_ASSERT( 0 );
#endif
}


//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------
	static constexpr unsigned short	m_day[]= {
		0,
		31,
		31 + 28,
		31 + 28 + 31,
		31 + 28 + 31 + 30,
		31 + 28 + 31 + 30 + 31,
		31 + 28 + 31 + 30 + 31 + 30,
		31 + 28 + 31 + 30 + 31 + 30 + 31,
		31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
		31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};
//-----------------------------------------------------------------------------
}


unsigned int  YMDToDays( unsigned int year, unsigned int month, unsigned int day )
{
	FL_ASSERT( month >= 1 && month <= 12 );
	FL_ASSERT( day >= 1 && day <= 31 );
	unsigned int	y= month <= 2 ? year-1 : year;
	return	year*365 + (y>>2) - (y/100) + (y/400) + m_day[ month-1 ] + day - (1+365);
}


void  DaysToSystemTime( DateTime& date, unsigned int days )
{
	unsigned int	year= (days+365) * 400 / (365*400 + 100 - 4 + 1); // 146097
	unsigned int	ym= year-1;
	unsigned int	d= ym*365 + (ym>>2) - (ym/100) + (ym/400);
	unsigned int	diff= days - d;

	unsigned int	isLeapYear= 0;
	unsigned int	yday= 365;
	if( !(year&3) && ( (year%100) || !(year%400)) ){
		isLeapYear= 1;
		yday++;
	}
	if( diff >= yday ){
		FL_ASSERT( diff < yday*2 );
		year++;
		diff-= yday;
	}
	if( !(year&3) && ( (year%100) || !(year%400)) ){
		isLeapYear= 1;
	}else{
		isLeapYear= 0;
	}

	unsigned int	m= (diff <<8) /7787;
	if( m < 1 ){
		if( diff >= m_day[m+1] ){
			m++;
		}
	}else if( m < 11 ){
		if( diff >= m_day[m+1]+isLeapYear ){
			m++;
		}
	}
	if( m < 2 ){
		isLeapYear= 0;
	}
	unsigned int	d2= diff - m_day[m] - isLeapYear;

	date.Year= year;
	date.Month= m + 1;
	date.Day= d2 + 1;

	date.Hour= 0;
	date.Minute= 0;
	date.Second= 0;
	date.Milliseconds= 0;
	date.DayOfWeek= 0;
}


uint32_t	SystemTimeToD32( const DateTime& date )
{
	FL_ASSERT( date.Year >= 2000 );
	FL_ASSERT( date.Month >= 1 && date.Month <= 12 );
	FL_ASSERT( date.Day >= 1 && date.Day <= 31 );
	FL_ASSERT( date.Hour < 24 );
	FL_ASSERT( date.Minute < 60 );
	FL_ASSERT( date.Second < 60 );

	unsigned int	y= date.Year-2000;
	FL_ASSERT( y < 133 );
	return	// 32140800
			 (( y          )  * (12*31*24*60*60) )	// 0-132
			+((date.Month-1)  * (   31*24*60*60) )	// 0-11
			+((date.Day-1)    * (      24*60*60) )	// 0-30
			+( date.Hour      * (         60*60) )	// 0-23
			+( date.Minute    * (            60) )	// 0-59
			+  date.Second;                      	// 0-59
}


void  D32ToSystemTime( DateTime& time, uint32_t d32 )
{
	time.Year=     d32 / (12*31*24*60*60)        + 2000;
	time.Month=  ((d32 / (   31*24*60*60)) % 12) + 1;
	time.Day=    ((d32 / (      24*60*60)) % 31) + 1;
	time.Hour=   ((d32 / (         60*60)) % 24);
	time.Minute= ((d32 / (            60)) % 60);
	time.Second=  (d32                     % 60);
	time.Milliseconds= 0;
	time.DayOfWeek= 0;

	FL_ASSERT( time.Year >= 2000 && time.Year <= 2133 );
	FL_ASSERT( time.Month >= 1 && time.Month <= 12 );
	FL_ASSERT( time.Day >= 1 && time.Day <= 31 );
	FL_ASSERT( time.Hour < 24 );
	FL_ASSERT( time.Minute < 60 );
	FL_ASSERT( time.Second < 60 );
}


//-----------------------------------------------------------------------------

namespace {
	char*	CopyStrNum( char* str, char* end_str, const char* format, unsigned int digit, unsigned int num )
	{
		if( str+digit < end_str ){
			sprintf_s( str, end_str-str, format, num );
		}
		return	str + digit;
	}
}

/*
	%Y%m%d%H%M%S
*/

void	GetDateTimeFormat( char* buffer, unsigned int buffer_size, const char* format, const DateTime& time )
{
#if 0
	sprintf_s( buffer, buffer_size, format,
				time.Year,
				time.Month,
				time.Day,
				time.Hour,
				time.Minute,
				time.Second );
#else
	char*	str= buffer;
	char*	end_str= buffer + buffer_size;
	const char*	ptr= format;
	for(; str < end_str && *ptr ;){
		auto	ch= *ptr++;
		if( ch == '%' ){
			auto	nch= *ptr++;
			switch( nch ){
			case 0:
				*str= '\0';
				return;
			case 'y':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Year%100 );
				break;
			case 'Y':
				str= CopyStrNum( str, end_str, "%04d", 4, time.Year );
				break;
			case 'm':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Month );
				break;
			case 'd':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Day );
				break;
			case 'H':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Hour );
				break;
			case 'M':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Minute );
				break;
			case 'S':
				str= CopyStrNum( str, end_str, "%02d", 2, time.Second );
				break;
			case '%':
				*str++= '%';
				break;
			}
		}else{
			*str++= ch;
		}
	}
	*str= '\0';
#endif
}


void	GetDateTimeString( char* buffer, unsigned int buffer_size, const DateTime& time )
{
	//GetDateTimeFormat( buffer, buffer_size, "%04d/%02d/%02d %02d:%02d:%02d", time );
	GetDateTimeFormat( buffer, buffer_size, "%Y/%m/%d %H:%M:%S", time );
}

void	GetDateTimeFileName( char* buffer, unsigned int buffer_size, const DateTime& time )
{
	//GetDateTimeFormat( buffer, buffer_size, "%04d%02d%02d%02d%02d%02d", time );
	GetDateTimeFormat( buffer, buffer_size, "%Y%m%d%H%M%S", time );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


