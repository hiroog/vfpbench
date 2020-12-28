// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/system/CoreContext.h>
#include	<flatlib/core/thread/Sleep.h>
#include	"BenchmarkTest.h"
#include	"BenchApplication.h"
#include	"quick_bench.h"
//#include	<stdio.h>
//#include	<stdlib.h>

using namespace flatlib;


class AppModule {
private:
	BenchmarkTest		BenchmarkInstance;
	BenchApplication	App;
public:
	AppModule()
	{
	}
	~AppModule()
	{
	}
	void	Init( float loop_scale )
	{
		system::RCore().RSystemInfo().Init();
		system::RCore().RSystemInfo().DumpSystemInfo();
		auto	bcount= BenchmarkInstance.GetBenchCount();
		App.Init( bcount );
		for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
			App.InitData( bi, BenchmarkInstance.GetBenchmark( bi ) );
		}
		BenchmarkInstance.UpdateLoop( loop_scale );
	}
	void	PrintInfo()
	{
		text::TextPool	pool;
		App.ExportCPUInfo( pool );
		pool.AddChar( '\0' );
		FL_PRINT( "%s\n", pool.GetText() );
	}
	void	PrintLog()
	{
		text::TextPool	pool;
		App.ExportLog( pool );
		pool.AddChar( '\0' );
		system::RConsoleLog().LogString( pool.GetText() );
	}
	void	PrintFlops()
	{
		text::TextPool	pool;
		App.ExportFlops( pool );
		pool.AddChar( '\0' );
		FL_PRINT( "%s\n", pool.GetText() );
	}
	void Dump( unsigned int btype )
	{
		FL_PRINT( "-------------------\n" );
		App.GetData( btype ).Dump();
		FL_PRINT( "-------------------\n" );
		FL_PRINT( "\n" );
	}
	void Wait( unsigned int btype )
	{
		auto*	bench= BenchmarkInstance.GetBenchmark( btype );
		bench->Run();
		for(;;){
			if( bench->IsDone() ){
				break;
			}
			thread::SleepThread( 1.0f );
		}
		App.UpdateResult( btype, bench );
		Dump( btype );
	}
	void RunBenchmark_All()
	{
		auto	bcount= BenchmarkInstance.GetBenchCount();
		for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
			Wait( bi );
		}
	}
	void RunBenchmark()
	{
		PrintInfo();
		RunBenchmark_All();
		PrintLog();
	}
};


void quick_bench( float loop_scale )
{
	system::RCore().RSystemInfo().Init();
	{
		AppModule	app;
		app.Init( loop_scale );
		app.RunBenchmark();
	}
}


