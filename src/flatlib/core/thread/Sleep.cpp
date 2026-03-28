// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/thread/ThreadBase.h"
#include	"flatlib/core/thread/Sleep.h"

#if FL_OS_WINDOWS
# include	"flatlib/core/platform/WindowsAPI.h"
#endif

#if FL_OS_LINUX
# include	<sched.h>
#endif

#if FL_OS_UNIX
# include	<time.h>
# include	<math.h>
#endif

#if FL_CC_VC
# include	<intrin.h>
#endif

#if FL_CC_CLANG_GCC && (FL_CPU_X86 || FL_CPU_X64)
# include	<x86intrin.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_CC_VC

void	CPU_Pause()
{
#if FL_CPU_X64 || FL_CPU_X86
	_mm_pause();
#else
	//__nop();
	__yield();
#endif
}

#endif



#if FL_CC_CLANG_GCC

void	CPU_Pause()
{
#if FL_CPU_X64 || FL_CPU_X86
	_mm_pause();
#elif FL_CPU_ARM64 || FL_CPU_ARM7 || FL_CPU_ARM6
	//asm( "nop" );
	asm( "yield" );
#else
	asm( "nop" );
#endif
}

#endif


//-----------------------------------------------------------------------------


void	SleepThread( float sec )
{
#if FL_OS_WINDOWS
	::Sleep( static_cast<unsigned int>( sec * 1000 ) );
#elif FL_OS_UNIX
	struct timespec	sleep_time;
	sleep_time.tv_sec= static_cast<int>(sec);
	sleep_time.tv_nsec= static_cast<int>( (sec - floorf(sec)) * 1.0e9);
	nanosleep( &sleep_time, nullptr );
#else
	FL_ASSERT( 0 );
#endif
}


void	SleepThreadMS( unsigned int ms )
{
#if FL_OS_WINDOWS
	::Sleep( ms );
#elif FL_OS_NX
	nn::os::SleepThread( nn::TimeSpan::FromMilliSeconds( ms ) );
#elif FL_OS_UNIX
	struct timespec	sleep_time;
	sleep_time.tv_sec= ms / 1000;
	sleep_time.tv_nsec= (ms % 1000) * 1000000;
	nanosleep( &sleep_time, nullptr );
#else
	FL_ASSERT( 0 );
#endif
}


void	YieldThread()
{
#if FL_OS_WINDOWS
	::SwitchToThread();
#elif FL_OS_LINUX
	sched_yield();
#elif FL_OS_UNIX
	struct timespec	sleep_time;
	sleep_time.tv_sec= 0;
	sleep_time.tv_nsec= 0;
	nanosleep( &sleep_time, nullptr );
#else
	FL_ASSERT( 0 );
#endif
}


void	OS_Pause()
{
	YieldThread();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

