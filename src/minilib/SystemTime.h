// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4:

#ifndef	SYSTEMTIME_H_
#define	SYSTEMTIME_H_


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct SystemTime {	// 16byte
	unsigned short	Year;
	unsigned short	Month;			// 1-12
	unsigned short	Day;			// 1-31
	unsigned short	Hour;			// 0-23
	unsigned short	Minute;			// 0-59
	unsigned short	Second;			// 0-59
	unsigned short	Milliseconds;	// 0-999
	unsigned short	DayOfWeek;		// 0-6
};


void	GetLocalTime( SystemTime& systime );


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

