// flatlib3 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"SystemAPI.h"
#include	"ConsoleIO.h"

#if flOS_UNIX
# include	<time.h>
# include	<sys/time.h>
# if flOS_LINUX
#  include	<sys/types.h>
#  include	<sys/syscall.h>
#  include	<sched.h>
#  include	<unistd.h>
# endif
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace time {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void SleepMS( unsigned int ms )
{
#if flOS_WINDOWS
	::Sleep( ms );
#endif
#if flOS_UNIX
	struct timespec	spec;
	spec.tv_sec= ms/1000;
	spec.tv_nsec= (ms%1000) * 1000000;
	nanosleep( &spec, nullptr );
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TickTime::TickTime()
{
	Init();
}

#if flOS_WINDOWS

void	TickTime::Init()
{
	LARGE_INTEGER	freq;
	QueryPerformanceFrequency( &freq );
	Freq= freq.QuadPart / 10;
}


int64_t	TickTime::GetUS() const
{
	LARGE_INTEGER	clock;
	QueryPerformanceCounter( &clock );
	return	clock.QuadPart * 100000 / Freq;
}

#else

void	TickTime::Init()
{
}

int64_t	TickTime::GetUS() const
{
	struct timeval	time_val;
	gettimeofday( &time_val, nullptr );
	return	static_cast<int64_t>( time_val.tv_sec ) * 1000000 + static_cast<int64_t>( time_val.tv_usec );
}

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	SetAffinityMask( uint64_t cpu_mask )
{
	if( !cpu_mask ){
		return;
	}
#if flOS_LINUX
	cpu_set_t	mask;

	CPU_ZERO( &mask );
	auto	cpu_bit= cpu_mask;
	for( int i= 0 ; i< 64 ; i++ ){
		if( cpu_mask & 1 ){
			CPU_SET( i, &mask );
		}
		cpu_mask>>= 1;
	}
	pid_t	tid= syscall(SYS_gettid);
	unsigned int	count= CPU_COUNT( &mask );
	FL_LOG( "SET CPU AFFINITY MASK=%08llx  CPU_COUNT=%d  (tid=%d)\n", cpu_bit, count, tid );

	int	result= sched_setaffinity( tid, sizeof(mask), &mask );
	if( result != 0 ){
		FL_ERROR( "SetAffinityMask: Error %d\n", result );
	}
#else
	flASSERT( 0 );
#endif
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


