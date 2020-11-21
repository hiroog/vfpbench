// 2014/01/15 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	TIMER_CLASS_H_
#define	TIMER_CLASS_H_

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/time/SystemClock.h>

struct TimerClass {
	uint64_t	start_time;
	uint64_t	result_time;
public:
	TimerClass()
	{
	}
	void 	Begin()
	{
		start_time= flatlib::time::GetPerfCounterUS();
	}
	void 	End()
	{
		uint64_t	end_time= flatlib::time::GetPerfCounterUS();
		result_time= end_time - start_time;
	}
	void 	Dump( const char* msg= nullptr, int cycle= 0 )
	{
		uint32_t	diff_time= static_cast<uint32_t>( result_time );
		FL_LOG( "%-12s %7.3fsec   %dusec\n",
				msg ? msg : "",
				diff_time/1000000.0f,
				diff_time
				);
	}
	uint64_t 	Result()
	{
		return	result_time;
	}
};

#endif

