// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"BenchApplication.h"
#include	<flatlib/core/time/DateTime.h>
#include	<flatlib/core/system/CoreContext.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TestBase.h"
#include	<stdio.h>
#include	<stdlib.h>

#define	USE_MAX_EXPORT	0

using namespace flatlib;
using system::CPUArch;
using system::CPUFeature;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct ArraySize {
	template<size_t N>
	constexpr static size_t	GetArraySize( const char (&data)[N] )
	{
		return	N;
	}
};

const char*	SkipSpace( const char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

const char*	GetWord( char* buffer, size_t buffer_size, const char* ptr )
{
	char*	str= buffer;
	char*	end_str= buffer + buffer_size -1;
	ptr= SkipSpace( ptr );
	if( *ptr == '"' ){
		for( ptr++ ; *ptr && *ptr != '"' && str < end_str ;){
			*str++= *ptr++;
		}
		if( *ptr == '"' ){
			ptr++;
		}
	}else{
		for(; *ptr && *ptr > ' ' && str < end_str ;){
			*str++= *ptr++;
		}
	}
	*str++= '\0';
	return	ptr;
}

static void	format_GFLOPS( text::TextPool& pool, const char* text, double flops )
{
	if( flops != 0.0 ){
		pool.AddFormat( "%s: %8.3f GFLOPS\n", text, flops / 1000.0 );
	}else{
		pool.AddFormat( "%s: -\n", text );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


void BenchApplication::ExportCPUInfo( text::TextPool& pool ) const
{
	const auto&	Info= system::RCore().RSystemInfo();
	auto	arch= Info.GetCompiledArch();

	pool.AddFormat( "ARCH: %s\n", Info.GetArchNameLong( arch ) );
	pool.AddFormat( "FPU :" );


	switch( arch ){
	case CPUArch::CPU_ARM6:
	case CPUArch::CPU_ARM7:
		if( Info.HasInstructionSet( CPUFeature::ARM_VFPV4 ) ){
			pool.AddFormat( " VFPv4" );
		}else{
			pool.AddFormat( " VFPv3" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_NEON ) ){
			pool.AddFormat( "-D32 NEON" );
		}else{
			pool.AddFormat( "-D16" );
		}
		pool.AddFormat( "\n" );
		break;
	case CPUArch::CPU_ARM64:
		pool.AddFormat( " ASIMD(AArch64 NEON)" );
		if( Info.HasInstructionSet( CPUFeature::ARM_FPHP ) ){
			pool.AddFormat( " FPHP" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SIMDHP ) ){
			pool.AddFormat( " ASIMDHP" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SIMDDP ) ){
			pool.AddFormat( " DOTPROD" );
		}
		if( Info.HasInstructionSet( CPUFeature::ARM_SVE ) ){
			pool.AddFormat( " SVE" );
		}
		pool.AddFormat( "\n" );
		break;
	case CPUArch::CPU_X86:
	case CPUArch::CPU_X64:
		if( Info.HasInstructionSet( CPUFeature::IA_SSE ) ){
			pool.AddFormat( " SSE" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE2 ) ){
			pool.AddFormat( " SSE2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSSE3 ) ){
			pool.AddFormat( " SSSE3" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE41 ) ){
			pool.AddFormat( " SSE4.1" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_SSE42 ) ){
			pool.AddFormat( " SSE4.2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){
			pool.AddFormat( " AVX" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX2 ) ){
			pool.AddFormat( " AVX2" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){
			pool.AddFormat( " FMA3" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_FMA4 ) ){
			pool.AddFormat( " FMA4" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_F16C ) ){
			pool.AddFormat( " F16C" );
		}
		if( Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ){
			pool.AddFormat( " AVX512F" );
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512BW ) ){
				pool.AddFormat( "/BW" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512DQ ) ){
				pool.AddFormat( "/DQ" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512VL ) ){
				pool.AddFormat( "/VL" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512VNNI ) ){
				pool.AddFormat( "/VNNI" );
			}
			if( Info.HasInstructionSet( CPUFeature::IA_AVX512BF16 ) ){
				pool.AddFormat( "/BF16" );
			}
		}
		pool.AddFormat( "\n" );
		break;
	case CPUArch::CPU_MIPS32:
	case CPUArch::CPU_MIPS64:
		if( Info.HasInstructionSet( CPUFeature::MIPS_PS ) ){
			pool.AddFormat( " PS" );
		}
		if( Info.HasInstructionSet( CPUFeature::MIPS_F64 ) ){
			pool.AddFormat( " F64" );
		}
		if( Info.HasInstructionSet( CPUFeature::MIPS_MSA ) ){
			pool.AddFormat( " MSA" );
		}
		pool.AddFormat( "\n" );
		break;
	default:
		pool.AddFormat( "\n" );
		break;
	}

	pool.AddFormat( "Name: %s\n", Info.GetDeviceName() );
	pool.AddFormat( "CPU Thread: %2d\n", Info.GetTotalThreadCount() );
	pool.AddFormat( "CPU Core  : %2d\n", Info.GetPhysicalCoreCount() );
	pool.AddFormat( "CPU Group : %2d\n", Info.GetCoreGroupCount() );
	{
		unsigned int 	group_count= Info.GetCoreGroupCount();
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			pool.AddFormat( "  Group %d: Thread=%2d  Clock=%f GHz  (mask:%llx)\n", gi, Info.GetThreadCount( gi ), Info.GetCoreClock( gi )/1000000.0, Info.GetAffinityMask( gi ) );
		}
	}

	switch( arch ){
	case CPUArch::CPU_ARM5:
	case CPUArch::CPU_ARM6:
	case CPUArch::CPU_ARM7:
	case CPUArch::CPU_ARM64:
		pool.AddFormat( "NEON   : %s\n", Info.HasInstructionSet( CPUFeature::ARM_NEON ) ? "yes" : "no" );
		pool.AddFormat( "FMA    : %s\n", Info.HasInstructionSet( CPUFeature::ARM_VFPV4 ) ? "yes" : "no" );
		pool.AddFormat( "FPHP   : %s\n", Info.HasInstructionSet( CPUFeature::ARM_FPHP ) ? "yes" : "no" );
		pool.AddFormat( "SIMDHP : %s\n", Info.HasInstructionSet( CPUFeature::ARM_SIMDHP ) ? "yes" : "no" );
		pool.AddFormat( "DotProd: %s\n", Info.HasInstructionSet( CPUFeature::ARM_SIMDDP ) ? "yes" : "no" );
		break;
	case CPUArch::CPU_X86:
	case CPUArch::CPU_X64:
		pool.AddFormat( "SSE   : %s\n", Info.HasInstructionSet( CPUFeature::IA_SSE2 ) ? "yes" : "no" );
		pool.AddFormat( "AVX   : %s\n", Info.HasInstructionSet( CPUFeature::IA_AVX ) ? "yes" : "no" );
		pool.AddFormat( "FMA   : %s\n", Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ? "yes" : "no" );
		pool.AddFormat( "F16C  : %s\n", Info.HasInstructionSet( CPUFeature::IA_F16C ) ? "yes" : "no" );
		pool.AddFormat( "AVX512: %s\n", Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ? "yes" : "no" );
		break;
	case CPUArch::CPU_MIPS32:
	case CPUArch::CPU_MIPS64:
		pool.AddFormat( "FPU-PS: %s\n", Info.HasInstructionSet( CPUFeature::MIPS_PS ) ? "yes" : "no" );
		pool.AddFormat( "MSA   : %s\n", Info.HasInstructionSet( CPUFeature::MIPS_MSA ) ? "yes" : "no" );
		break;
	default:
		break;
	}

}


void BenchApplication::ExportFlops( text::TextPool& pool ) const
{
	pool.AddFormat( "\nTotal:\n" );
	format_GFLOPS( pool, "SingleThread HP max", GetMaxMFLOPS( LOOPTYPE_HP, false ) );
	format_GFLOPS( pool, "SingleThread SP max", GetMaxMFLOPS( LOOPTYPE_SP, false ) );
	format_GFLOPS( pool, "SingleThread DP max", GetMaxMFLOPS( LOOPTYPE_DP, false ) );
	format_GFLOPS( pool, "MultiThread  HP max", GetTotalMFLOPS( LOOPTYPE_HP, true  ) );
	format_GFLOPS( pool, "MultiThread  SP max", GetTotalMFLOPS( LOOPTYPE_SP, true  ) );
	format_GFLOPS( pool, "MultiThread  DP max", GetTotalMFLOPS( LOOPTYPE_DP, true  ) );

	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	group_count= Info.GetCoreGroupCount();
	for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
		pool.AddFormat( "\nGroup %d:  Thread=%d  Clock=%f GHz  (mask:%llx)\n", gi, Info.GetThreadCount( gi ), Info.GetCoreClock( gi )/1000000.0, Info.GetAffinityMask( gi ) );
		format_GFLOPS( pool, "  SingleThread HP max", GetMFLOPS( LOOPTYPE_HP, false, gi ) );
		format_GFLOPS( pool, "  SingleThread SP max", GetMFLOPS( LOOPTYPE_SP, false, gi ) );
		format_GFLOPS( pool, "  SingleThread DP max", GetMFLOPS( LOOPTYPE_DP, false, gi ) );
		format_GFLOPS( pool, "  MultiThread  HP max", GetMFLOPS( LOOPTYPE_HP, true,  gi ) );
		format_GFLOPS( pool, "  MultiThread  SP max", GetMFLOPS( LOOPTYPE_SP, true,  gi ) );
		format_GFLOPS( pool, "  MultiThread  DP max", GetMFLOPS( LOOPTYPE_DP, true,  gi ) );
	}
	pool.AddFormat( "\n" );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	BenchApplication::ExportLine( text::TextPool& pool, const ResultLine& line )
{
	if( line.IsActive() ){
#if USE_MAX_EXPORT
		pool.AddFormat( "%-34s: %8.3f  %9.1f  %9.1f  (%5.1f %3.1f) %9.1f\n",
				line.Title,
				line.Time,
				line.Flops,
				line.Ops,
				line.Fop,
				line.Ipc,
				line.Max
			);
#else
		pool.AddFormat( "%-34s: %8.3f  %9.1f  %9.1f  (%5.1f %3.1f)\n",
				line.Title,
				line.Time,
				line.Flops,
				line.Ops,
				line.Fop,
				line.Ipc
			);
#endif
	}else{
#if USE_MAX_EXPORT
		pool.AddFormat( "%-34s:        -          -          -       -   -          -\n", line.Title );
#else
		pool.AddFormat( "%-34s:        -          -          -       -   -\n", line.Title );
#endif
	}
}


void	BenchApplication::ExportData( text::TextPool& pool, const ResultData& data )
{
	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	count= data.GetSize();
	unsigned int	group= data.GetCoreGroup();
	pool.AddFormat( "\n* Group %d:  Thread=%d  Clock=%f GHz  (mask:%llx)\n", group, data.IsMultithread() ? Info.GetThreadCount( group ) : 1, Info.GetCoreClock( group )/1000000.0, Info.GetAffinityMask( group ) );
	pool.AddFormat( "* %s\n", data.GetTitle() );
	//				0123456789012345678901234567890123: ________  _________  _________ (___ ___) _________
#if USE_MAX_EXPORT
	pool.AddFormat( "                                      TIME(s)   MFLOPS      MOPS     FOP   IPC  max MFLOPS\n" );
#else
	pool.AddFormat( "                                      TIME(s)   MFLOPS      MOPS     FOP   IPC\n" );
#endif
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		ExportLine( pool, data.Get( ci ) );
	}
	pool.AddFormat( "\n" );
}



void	BenchApplication::ExportLog( text::TextPool& pool ) const
{
	text::TextPool	info;
	if( *DateTimeStr ){
		pool.AddFormat( "Date: %s\n", DateTimeStr );
	}
	ExportCPUInfo( info );
	ExportFlops( info );
	//save_size( buffer, info.GetTop(), info.GetSize() );
	info.AddChar( '\0' );
	pool.AddText( info.GetText() );

	unsigned int	bcount= GetDataCount();
	for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
		ExportData( pool, GetData( bi ) );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
T DateTime
N ProcessorName
D Data
L Line
*/

void	BenchApplication::LoadLine( ResultLine& line, const char* ptr )
{
	const int	WORD_BUFFER_SIZE= 64;
	char	word_buffer[WORD_BUFFER_SIZE];
	ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
	ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
	if( strcmp( line.Title, word_buffer ) != 0 ){
		return;
	}
	FL_ASSERT( (&line.Max - &line.Time + 1) == 6 );
	double*	dptr= &line.Time;
	for( int i= 0 ; i< 6 ; i++ ){
		ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
		*dptr++= atof( word_buffer );
	}
}

void	BenchApplication::LoadFile( const char* file_name )
{
	FILE*	fp= fopen( file_name, "rb" );
	if( !fp ){
		return;
	}
	unsigned int	data_count= GetDataCount();
	const int	LINE_BUFFER_SIZE= 256;
	char		line_buffer[LINE_BUFFER_SIZE];
	const int	WORD_BUFFER_SIZE= 64;
	char		word_buffer[WORD_BUFFER_SIZE];
	unsigned int	line_index= 0;
	ResultData*	current_data= nullptr;
	for(; fgets( line_buffer, LINE_BUFFER_SIZE-2, fp ) ;){
		const char*	ptr= SkipSpace( line_buffer );
		if( *ptr == '#' || *ptr == '\n' ){
			continue;
		}
		if( *ptr == 'T' ){
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( DateTimeStr, ArraySize::GetArraySize( DateTimeStr )-2, ptr );
			continue;
		}
		if( *ptr == 'D' ){
			line_index= 0;
			current_data= nullptr;
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			unsigned int	line_count= atoi( word_buffer );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			unsigned int	core_group= atoi( word_buffer );
			ptr= GetWord( word_buffer, WORD_BUFFER_SIZE, ptr );
			for( unsigned int bi= 0 ; bi< data_count ; bi++ ){
				ResultData&	data= GetData( bi );
				if( data.GetCoreGroup() == core_group && data.GetSize() == line_count && strcmp( data.GetTitle(), word_buffer ) == 0 ){
					current_data= &data;
					line_index= 0;
					break;
				}
			}
		}
		if( current_data ){
			if( *ptr == 'L' ){
				ResultLine&	line= current_data->Get( line_index++ );
				LoadLine( line, ptr );
			}
		}
	}
	fclose( fp );
}

void	BenchApplication::SaveLine( text::TextPool& pool, const ResultLine& line ) const
{
	pool.AddFormat( "L \"%s\" %f %f %f %f %f %f\n",
			line.Title,
			line.Time,
			line.Flops,
			line.Ops,
			line.Fop,
			line.Ipc,
			line.Max
		);
}

void	BenchApplication::SaveData( text::TextPool& pool, const ResultData& data ) const
{
	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	count= data.GetSize();
	unsigned int	group= data.GetCoreGroup();
	pool.AddFormat( "# Group=%d Thread=%d Clock=%d Mask=%llx\n", group, Info.GetThreadCount( group ), Info.GetCoreClock( group ), Info.GetAffinityMask( group ) );
	pool.AddFormat( "D %d %d %d \"%s\"\n", data.GetBenchIndex(), count, group, data.GetTitle() );
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		SaveLine( pool, data.Get( ci ) );
	}
}

void	BenchApplication::SaveFile( const char* file_name ) const
{
	text::TextPool	pool;
	if( *DateTimeStr ){
		pool.AddFormat( "T \"%s\"\n", DateTimeStr );
	}

	unsigned int	bcount= GetDataCount();
	for( unsigned int bi= 0 ; bi< bcount ; bi++ ){
		SaveData( pool, GetData( bi ) );
	}
	{
		FILE*	fp= fopen( file_name, "wb" );
		if( fp ){
			fwrite( pool.GetBuffer(), pool.GetDataSize(), 1, fp );
			fclose( fp );
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

BenchApplication::BenchApplication()
{
	*DateTimeStr= '\0';
}


void	BenchApplication::Init( unsigned int bench_count )
{
	DataArray.SetArraySize( bench_count );
}


void	BenchApplication::InitData( unsigned int btype, ITestBase* bench )
{
	const auto&	Info= system::RCore().RSystemInfo();
	ResultData&	data= GetData( btype );
	data.SetInfo( bench->GetTestName(), bench->GetLoopType(), btype, bench->IsMultithread(), bench->GetCoreGroup(), Info.GetCoreClock( bench->GetCoreGroup() ) );

	unsigned int	title_size= bench->GetResultCount();
	data.InitSize( title_size );
	for( unsigned int ti= 0 ; ti< title_size ; ti++ ){
		ResultLine&	line= data.Get( ti );
		line.Title= bench->GetInstructionName( ti );
	}
}


double	BenchApplication::GetMFLOPS( unsigned int loop_type, bool multithread, unsigned int group ) const
{
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type && data.GetCoreGroup() == group ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				return	line.Max;
			}
		}
	}
	return	0.0;
}

double	BenchApplication::GetTotalMFLOPS( unsigned int loop_type, bool multithread ) const
{
	double	total= 0.0f;
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				total+= line.Max;
			}
		}
	}
	return	total;
}

double	BenchApplication::GetMaxMFLOPS( unsigned int loop_type, bool multithread ) const
{
	double	max_flops= 0.0f;
	unsigned int	data_count= GetDataCount();
	for( unsigned int di= 0 ; di< data_count ; di++ ){
		const auto&	data= GetData( di );
		if( data.IsMultithread() == multithread && data.GetLoopType() == loop_type ){
			const auto&	line= data.GetHighest();
			if( line.IsActive() ){
				if( line.Max > max_flops ){
					max_flops= line.Max;
				}
			}
		}
	}
	return	max_flops;
}


void BenchApplication::UpdateResult( unsigned int btype, ITestBase* bench )
{
	const auto&	Info= system::RCore().RSystemInfo();
	ResultData&	data= GetData( btype );
	data.UpdateBegin();
	unsigned int	count= bench->GetResultInfo( InfoType::INFO_COUNT );
	double			loop= bench->GetResultInfo( InfoType::INFO_LOOP );
	unsigned int	group= bench->GetCoreGroup();
	unsigned int	kclock= Info.GetCoreClock( group );
	double			mclock= kclock / 1000.0;
	for( unsigned int ci= 0 ; ci< count ; ci++ ){
		double	time= bench->GetResult( ci ) / 1000000.0;
		double	float_op= bench->GetLoopOp( ci );
		double	inst_fop= bench->GetInstFop( ci );
		double	flops= 0.0;
		double	ops= 0.0;
		double	ipc= 0.0;
		if( time >= 1e-5 ){
			flops= (float_op * loop / 1000000.0) / time;		// MFLOPS
			ops= (float_op * loop / inst_fop / 1000000.0) / time;	// MOPS
			if( kclock ){
				ipc= ops / mclock;
			}
		}
		data.Update( time, flops, ops, inst_fop, ipc );
	}
	data.UpdateEnd();
}


void	BenchApplication::UpdateTimestamp()
{
	time::DateTime	time;
	time::GetLocalTime( time );
	size_t	length= ArraySize::GetArraySize( DateTimeStr );
	snprintf( DateTimeStr, length-2, "%04d%02d%02d %02d%02d%02d", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second );
	DateTimeStr[length-1]= '\0';
}


