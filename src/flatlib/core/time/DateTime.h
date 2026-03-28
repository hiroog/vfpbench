// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace time {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct DateTime {
	uint16_t	Year;			// 2013
	uint16_t	Month;			// 1-12
	uint16_t	Day;			// 1-31
	uint16_t	Hour;			// 0-23
	uint16_t	Minute;			// 0-59
	uint16_t	Second;			// 0-59
	uint16_t	Milliseconds;	// 0-999
	uint16_t	DayOfWeek;		// 0-6		SMTWTFS
};


//-----------------------------------------------------------------------------

// move to DateTime_Platform.h
//void	SystemTimeToDateTime( DateTime& datetime, const SYSTEMTIME& system_time );
//void	SystemTimeToDateTime( DateTime& datetime, const struct tm& system_time );

void	GetLocalTime( DateTime& datetime );
void	GetUTC( DateTime& datetime );


unsigned int	YMDToDays( unsigned int year, unsigned int month, unsigned int day );
void		DaysToDateTime( DateTime& date, unsigned int days );

/*!
	YMD-HMS 32bit encoding
*/
uint32_t	DateTimeToD32( const DateTime& date );
void		D32ToDateTime( DateTime& date, uint32_t date32 );

/*!
	Unix time
*/
//double		DateTimeToSeconds( const DateTime& date );
//void		SecondsToDateTime( double perf_clock );


//-----------------------------------------------------------------------------

/*!
	@code
	%Y  2010
	%y  10
	%m  01-12
	%d  01-31
	%H  00-23
	%M  00-59
	%S  00-59
	%f  000-999 ms
	@endcode
	@code
		GetDateTimeFormat( buffer, 128, "%Y-%m-%d %H:%M:%S.%f", time );
	@endcode
*/
void	GetDateTimeFormat( char* buffer, unsigned int buffer_size, const char* format, const DateTime& time );
template<typename T>
void	GetDateTimeFormat( T& buffer, const char* format, const DateTime& time )
{
	return	GetDateTimeFormat( reinterpret_cast<char*>(buffer.GetBuffer()), buffer.GetBufferSize(), format, time );
}

// "%Y/%m/%d %H:%M:%S" 19
void	GetDateTimeString( char* buffer, unsigned int buffer_size, const DateTime& time );
template<typename T>
void	GetDateTimeString( T& buffer, const DateTime& time )
{
	return	GetDateTimeString( reinterpret_cast<char*>(buffer.GetBuffer()), buffer.GetBufferSize(), time );
}

// "%Y%m%d%H%M%S" 14
void	GetDateTimeFileName( char* buffer, unsigned int buffer_size, const DateTime& time );
template<typename T>
void	GetDateTimeFileName( T& buffer, const DateTime& time )
{
	return	GetDateTimeFileName( reinterpret_cast<char*>(buffer.GetBuffer()), buffer.GetBufferSize(), time );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


