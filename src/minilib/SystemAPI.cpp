// flatlib3 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"SystemAPI.h"
#include	"ConsoleIO.h"
#include	"Assert.h"

#if flOS_UNIX
# include	<time.h>
# include	<sys/time.h>
# if flOS_LINUX
#  include	<sys/types.h>
#  if !flOS_PPO
#   include	<sys/syscall.h>
#  endif
#  include	<sched.h>
#  include	<unistd.h>
# endif
#endif

#if flOS_DARWIN
# include	<pthread.h>
# include	<mach/mach.h>
# include	<mach/task_info.h>
# include	<mach/thread_policy.h>
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
#if flOS_PPO
	return;
#elif flOS_LINUX
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


#if flOS_DARWIN
void	SetCpuAffinityTag( unsigned int tag )
{
	auto	thread= pthread_mach_thread_np( pthread_self() );
	thread_affinity_policy	affinity_tag;
	affinity_tag.affinity_tag= tag;
	thread_policy_set( thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&affinity_tag, THREAD_AFFINITY_POLICY_COUNT );
}

unsigned int	GetCpuAffinityTag()
{
	auto	thread= pthread_mach_thread_np( pthread_self() );
	thread_affinity_policy	affinity_tag;
	mach_msg_type_number_t	count= THREAD_AFFINITY_POLICY_COUNT;
	boolean_t	getdef= 0;
	thread_policy_get( thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&affinity_tag, &count, &getdef );
	return	affinity_tag.affinity_tag;
}

void	GetAffinityInfo()
{
	mach_msg_type_number_t	count= TASK_AFFINITY_TAG_INFO_COUNT;
	task_affinity_tag_info	info;
	task_info( current_task(), TASK_AFFINITY_TAG_INFO, (task_info_t)&info, &count );

	FL_PRINT( "count=%d min=%x max=%x count=%d count=%d\n", 
		info.set_count,
		info.min,
		info.max,
		info.task_count,
		count);
}
#endif

void	SetCpuAffinity( unsigned int cpu_index )
{
#if flOS_PPO
	return;
#elif flOS_LINUX
	cpu_set_t	mask;

	CPU_ZERO( &mask );
	CPU_SET( cpu_index, &mask );
	pid_t	tid= syscall(SYS_gettid);
	unsigned int	count= CPU_COUNT( &mask );
	flASSERT( count == 1 );
	FL_LOG( "SET AFFINITY CPU=%d  CPU_COUNT=%d  (tid=%d)\n", cpu_index, count, tid );

	int	result= sched_setaffinity( tid, sizeof(mask), &mask );
	if( result != 0 ){
		FL_ERROR( "SetCpuAffinity: Error %d\n", result );
	}
#elif flOS_DARWIN

	mach_port_t	thread= pthread_mach_thread_np( pthread_self() );

	{
		thread_affinity_policy	thread_tag;
		thread_tag.affinity_tag= cpu_index + 1;
		thread_policy_set( thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&thread_tag, THREAD_AFFINITY_POLICY_COUNT ); 
	}

	{
		thread_affinity_policy	thread_tag;
		mach_msg_type_number_t	count= THREAD_AFFINITY_POLICY_COUNT;
		boolean_t	getdef= 0;
		thread_policy_get( thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&thread_tag, &count, &getdef );
		FL_LOG( "SET AFFINITY CPU=%d (%d)\n", cpu_index, thread_tag.affinity_tag );
	}

	{
		task_affinity_tag_info	thread_info;
		mach_msg_type_number_t	count= TASK_AFFINITY_TAG_INFO_COUNT;
		task_info( current_task(), TASK_AFFINITY_TAG_INFO, (task_info_t)&thread_info, &count );
		FL_LOG( "  SetCount=%d min=%d max=%d TaskCount=%d\n",
				thread_info.set_count,
				thread_info.min,
				thread_info.max,
				thread_info.task_count );
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


