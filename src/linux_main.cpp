// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/system/CoreContext.h>
#include	<flatlib/core/thread/Sleep.h>
#include	"BenchmarkTest.h"
#include	"BenchApplication.h"
#include	<stdio.h>
#include	<stdlib.h>

using namespace flatlib;

enum : unsigned int {
	BTYPE_ALL	=	0x10000,
};

enum : unsigned int {
	INFO_BENCH,
	INFO_CPU,
	INFO_LOG,
};

enum : unsigned int {
	FLAG_PRINT_RESULT	=	1<<0,
	FLAG_LIST_MODE		=	1<<1,
	FLAG_INFO_CPU		=	1<<2,
	FLAG_INFO_LOG		=	1<<3,
	FLAG_INFO_BENCH		=	1<<4,
};

class AppModule {
private:
	BenchmarkTest		BenchmarkInstance;
	BenchApplication	App;
	unsigned int		Flags= 0;
public:
	AppModule()
	{
	}
	~AppModule()
	{
	}
	void	Init( float loop_scale, const char* save_file= ".save.log", unsigned int flags= 0 )
	{
		system::RCore().RSystemInfo().Init();
		Flags= flags;
		auto	bcount= BenchmarkInstance.GetBenchCount();
		App.Init( bcount );
		for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
			App.InitData( bi, BenchmarkInstance.GetBenchmark( bi ) );
		}
		BenchmarkInstance.UpdateLoop( loop_scale );
		App.LoadFile( save_file );
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
	void	List( unsigned int info_flags )
	{
		if( info_flags & FLAG_INFO_CPU ){
			PrintInfo();
			system::RCore().RSystemInfo().DumpSystemInfo();
		}
		if( info_flags & FLAG_INFO_LOG ){
			PrintLog();
		}
		if( info_flags & FLAG_INFO_BENCH ){
			auto	bcount= BenchmarkInstance.GetBenchCount();
			for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
				const auto*	bench= BenchmarkInstance.GetBenchmark( bi );
				FL_PRINT( "%2d: G=%d T=%d %s\n", bi, bench->GetCoreGroup(), bench->IsMultithread(), bench->GetTestName() );
			}
		}
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
	void RunBenchmark_All( unsigned int btype )
	{
		if( btype != BTYPE_ALL ){
			Wait( btype );
		}else{
			auto	bcount= BenchmarkInstance.GetBenchCount();
			for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
				Wait( bi );
			}
		}
	}
	void RunBenchmark( unsigned int btype, const char* log_file )
	{
		PrintInfo();
		RunBenchmark_All( btype );
		App.UpdateTimestamp();
		App.SaveFile( ".save.log" );

		if( log_file ){
			text::TextPool	pool;
			App.ExportLog( pool );

			FILE*	fp= fopen( log_file, "wb" );
			if( fp ){
				fwrite( pool.GetBuffer(), pool.GetDataSize(), 1, fp );
				fclose( fp );
			}else{
				FL_ERROR( "write error %s\n", log_file );
			}
			FL_PRINT( "output = %s\n", log_file );
		}

		PrintLog();
		if( Flags & FLAG_PRINT_RESULT ){
			PrintInfo();
			PrintFlops();
		}
	}
};


static void usage()
{
	FL_PRINT(
		"vfpbench v2.3 2014-2026 Hiroyuki Ogasawara\n"
		"usage: vfpbench [options]\n"
		" -l                  List benchmark tests\n"
		" -i                  Display system info\n"
		" -r[<savefile>]      Show last results\n"
		" -b<bench_index>     Select benchmark test 0..n (1000=all)\n"
		" -c<loop_scale>      Loop count scale (default 1.0)\n"
		" --log <log_file>    Log filename (default 'output_log.txt')\n"
		" --pr                Print result\n"
		);
	exit( 1 );
}

int main( int argc, char** argv )
{
	auto*	context= system::CreateContext();
	context->RConsoleLog().SetConsoleOutputMode( true );
	{
		system::RCore().RSystemInfo().Init();
		float	loop_scale= 1.0f;
		unsigned int	btype= BTYPE_ALL;
		const char*		log_file= "output_log.txt";
		const char*		save_file= ".save.log";
		unsigned int	flags= 0;
		for(; --argc ;){
			if( **++argv == '-' ){
				switch( (*argv)[1] ){
				case 'l':
					flags|= FLAG_LIST_MODE|FLAG_INFO_BENCH;
					break;
				case 'i':
					flags|= FLAG_LIST_MODE|FLAG_INFO_CPU;
					break;
				case 'r':
					flags|= FLAG_LIST_MODE|FLAG_INFO_LOG;
					if( (*argv)[2] ){
						save_file= *argv+2;
					}
					break;
				case 'b':
					if( (*argv)[2] != '\0' ){
						btype= atoi( *argv + 2 );
					}else{
						usage();
					}
					break;
				case 'c':
					loop_scale= atof( *argv+2 );
					break;
				case '-': {
						char	ch= (*argv)[2];
						switch( ch ){
						case 'p': {
								flags|= FLAG_PRINT_RESULT;
							}
							break;
						case 'l': {
								if( argc >= 2 ){
									log_file= *++argv;
									argc--;
								}else{
									usage();
									break;
								}
							}
							break;
						default:
							usage();
							break;
						}
					}
					break;
				default:
					usage();
					break;
				}
			}else{
				usage();
			}
		}

		{
			AppModule	app;
			app.Init( loop_scale, save_file, flags );
			if( flags & FLAG_LIST_MODE ){
				app.List( flags );
			}else{
				app.RunBenchmark( btype, log_file );
			}
		}

		ZRelease( context );
	}
	memory::RAllocator().DumpStatus();
	FL_ASSERT( memory::RAllocator().GetTotalCount() == 0 );
	return 0;
}


