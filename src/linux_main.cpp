// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemAPI.h>
#include	<minilib/SystemInfo.h>
#include	"BenchmarkTest.h"
#include	"BenchApplication.h"

#include	<stdio.h>
#include	<stdlib.h>


using namespace flatlib;


enum : unsigned int {
	BTYPE_ALL	=	1000,
};

enum : unsigned int {
	INFO_BENCH,
	INFO_CPU,
	INFO_LOG,
};


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
	void	Init( float loop_scale, const char* save_file= ".save.log" )
	{
		Info.Init();
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
		ut::BinaryBuffer	buffer;
		App.ExportCPUInfo( buffer );
		*reinterpret_cast<char*>(buffer.Alloc( 1 ))= '\0';
		FL_PRINT( "%s\n", buffer.GetTop() );
	}
	void	PrintLog()
	{
		ut::BinaryBuffer	buffer;
		App.ExportLog( buffer );
		*reinterpret_cast<char*>(buffer.Alloc( 1 ))= '\0';
		FL_OUTPUT( reinterpret_cast<const char*>(buffer.GetTop()) );
	}
	void	PrintFlops()
	{
		ut::BinaryBuffer	buffer;
		App.ExportFlops( buffer );
		*reinterpret_cast<char*>(buffer.Alloc( 1 ))= '\0';
		FL_PRINT( "%s\n", buffer.GetTop() );
	}
	void	List( unsigned int info_mode )
	{
		switch( info_mode ){
		case INFO_CPU:
			PrintInfo();
			Info.DumpSystemInfo();
			break;
		case INFO_BENCH: {
				auto	bcount= BenchmarkInstance.GetBenchCount();
				for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
					auto*	bench= BenchmarkInstance.GetBenchmark( bi );
					FL_PRINT( "%d: %s\n", bi, bench->GetTestName() );
				}
			}
			break;
		case INFO_LOG:
			PrintLog();
			break;
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
			time::SleepMS( 1000 );
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
			ut::BinaryBuffer	buffer;
			App.ExportLog( buffer );

			FILE*	fp= fopen( log_file, "wb" );
			if( fp ){
				fwrite( buffer.GetTop(), buffer.GetSize(), 1, fp );
				fclose( fp );
			}else{
				FL_ERROR( "write error %s\n", log_file );
			}
			FL_PRINT( "output = %s\n", log_file );
		}

		PrintLog();
#if 0
		PrintInfo();
		PrintFlops();
#endif
	}

};




static void usage()
{
	FL_PRINT(
		"vfpbench v2.2 2014-2020 Hiroyuki Ogasawara\n"
		"usage: vfpbench [options]\n"
		" -l                  List benchmark tests\n"
		" -i                  Display system info\n"
		" -r[<savefile>]      Show last results\n"
		" -b<bench_index>     Select benchmark test 0..n (1000=all)\n"
		" -c<loop_scale>      Loop count scale (default 1.0)\n"
		" --log <log_file>    Log filename (default 'output_log.txt')\n"
		);
	exit( 1 );
}



int main( int argc, char** argv )
{
	{
		Info.Init();
		float	loop_scale= 1.0f;
		unsigned int	btype= BTYPE_ALL;
		const char*		log_file= "output_log.txt";
		const char*		save_file= ".save.log";
		bool			list_mode= false;
		unsigned int	info_mode= INFO_BENCH;
		for(; --argc ;){
			if( **++argv == '-' ){
				switch( (*argv)[1] ){
				case 'l':
					list_mode= true;
					break;
				case 'i':
					info_mode= INFO_CPU;
					list_mode= true;
					break;
				case 'r':
					info_mode= INFO_LOG;
					list_mode= true;
					if( (*argv)[2] ){
						save_file= *argv+2;
					}
					break;
				case 'b':
					btype= atoi( *argv + 2 );
					break;
				case 'c':
					loop_scale= atof( *argv + 3 );
					break;
				case '-': {
						char	ch= (*argv)[2];
						switch( ch ){
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
			app.Init( loop_scale, save_file );
			if( list_mode ){
				app.List( info_mode );
			}else{
				app.RunBenchmark( btype, log_file );
			}
		}

	}
	FL_LOG( "memory=%zd %zd\n", memory::GetAllocCount(), memory::GetAllocSize() );
	FL_ASSERT( memory::GetAllocCount() == 0 );
	FL_ASSERT( memory::GetAllocSize() == 0 );

	return 0;
}



