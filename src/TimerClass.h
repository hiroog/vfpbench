// 2014/01/15 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	TIMER_CLASS_H_
#define	TIMER_CLASS_H_

#include	<minilib/CoreLib.h>
#include	<minilib/SystemAPI.h>

struct TimerClass {
	uint64_t	start_time;
	uint64_t	result_time;
	flatlib::time::TickTime	Time;
public:
	TimerClass()
	{
		Time.Init();
	}
	void 	Begin()
	{
		start_time= Time.GetUS();
	}
	void 	End()
	{
		uint64_t	end_time= Time.GetUS();
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

