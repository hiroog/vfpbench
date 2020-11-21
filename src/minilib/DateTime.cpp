// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"DateTime.h"
#include	<time.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace time {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	GetLocalTime( DateTime& systime )
{
	time_t	curtime;
	::time( &curtime );
	struct tm	timebuf;
	::localtime_r( &curtime, &timebuf );
	systime.Year= timebuf.tm_year + 1900;
	systime.Month= timebuf.tm_mon + 1;
	systime.Day= timebuf.tm_mday;
	systime.Hour= timebuf.tm_hour;
	systime.Minute= timebuf.tm_min;
	systime.Second= timebuf.tm_sec;
	systime.Milliseconds= 0;
	systime.DayOfWeek= timebuf.tm_wday;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

