// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/time/TimeBase.h"
#include	"flatlib/core/time/SystemClock.h"
#include	"flatlib/core/platform/WindowsAPI.h"

#if FL_OS_UNIX
//# include	<stdio.h>
# include	<time.h>
# include	<sys/time.h>
#endif


#define	USE_CLOCKGETTIME	1


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace time {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_OS_WINDOWS

static uint64_t	SystemFrequency10= 0;
static double	SystemFrequencyD= 0;
static float	SystemFrequencyF= 0;

static void	InitializeFrequency()
{
	LARGE_INTEGER	frequency;
	QueryPerformanceFrequency( &frequency );
	SystemFrequency10= frequency.QuadPart / 10;
	SystemFrequencyD= 1.0  / frequency.QuadPart;
	SystemFrequencyF= 1.0f / frequency.QuadPart;
}


uint64_t	GetPerfCounterUS()
{
	uint64_t	sys_freq10= SystemFrequency10;
	if( sys_freq10 == 0 ){
		InitializeFrequency();
		sys_freq10= SystemFrequency10;
	}
	FL_ASSERT( sys_freq10 != 0 );
	LARGE_INTEGER	Clock;
	QueryPerformanceCounter( &Clock );
	return	Clock.QuadPart * 100000 / sys_freq10;
}


double	GetPerfCounter()
{
	if( SystemFrequency10 == 0 ){
		InitializeFrequency();
	}
	LARGE_INTEGER	Clock;
	QueryPerformanceCounter( &Clock );
	return	Clock.QuadPart * SystemFrequencyD;
}


float	GetPerfCounterFloat()
{
	if( SystemFrequency10 == 0 ){
		InitializeFrequency();
	}
	LARGE_INTEGER	Clock;
	QueryPerformanceCounter( &Clock );
	return	Clock.QuadPart * SystemFrequencyF;
}

#endif


//-----------------------------------------------------------------------------

#if FL_OS_UNIX

uint64_t	GetPerfCounterUS()
{
	struct timespec	tp;

#if USE_CLOCKGETTIME
# if defined(CLOCK_MONOTONIC_RAW)
	clockid_t   t= CLOCK_MONOTONIC_RAW;
# elif defined(CLOCK_MONOTONIC)
	clockid_t   t= CLOCK_MONOTONIC;
# else
	clockid_t   t= CLOCK_REALTIME;
# endif

	clock_gettime( t, &tp );
	return  static_cast<uint64_t>( tp.tv_sec ) * 1000000 + static_cast<uint64_t>( tp.tv_nsec ) / 1000;
#else
	struct timeval tp;
	gettimeofday( &tp, nullptr );
	return	static_cast<uint64_t>( tp.tv_sec ) * 1000000 + static_cast<uint64_t>( tp.tv_usec );
#endif
}


double	GetPerfCounter()
{
	return	GetPerfCounterUS() * (1.0/1000000.0);
}


float	GetPerfCounterFloat()
{
	return	GetPerfCounterUS() / 1000000.0f;
}

#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


