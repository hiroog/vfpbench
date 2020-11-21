// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/CoreContext.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	<flatlib/core/memory/MemoryCopy.h>
#include	<flatlib/core/thread/Processor.h>
#include	"TestBase.h"

using namespace flatlib;



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void	ITestBase::SetCpuAffinity()
{
	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	group= GetCoreGroup();
	unsigned int	thread_count= Info.GetThreadCount( group );
	unsigned int	core_clock= Info.GetCoreClock( group );
	auto			affinity_mask= Info.GetAffinityMask( group );
	thread::SetAffinityMask( affinity_mask );
	FL_PRINT( "GROUP=%d  THREAD=%d  Affinity=%08llx  Clock=%d\n",
					group,
					thread_count,
					affinity_mask,
					core_clock );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TestBase::TestBase() :
	Info( system::RCore().RSystemInfo() ),
	LoopCount( BASE_DEFAULT_LOOP_FPU )
{
	InitClear();
}

void	TestBase::InitClear()
{
	memory::MemClear( TestResult );
	ClearResult();
}


void	TestBase::ClearResult()
{
	for( unsigned int ri= 0 ; ri< RESULT_BUFFER_MAX ; ri++ ){
		TestResult[ri].Time= 0;
	}
	Progress.Set( 0 );
	DoneFlag.Set( 0 );
}


void	TestBase::SetLoop( unsigned int loop )
{
	LoopCount= loop;
}

volatile unsigned int	TestBase::GetProgress()
{
	return	Progress.Get();
}

volatile unsigned int	TestBase::IsDone()
{
	return	DoneFlag.Get();
}


void	TestBase::SetOp( unsigned int index, unsigned int lop, unsigned int fop )
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	auto&	result= TestResult[index];
	result.Lop= lop;
	result.Fop= fop;
}

void	TestBase::SetOp2( unsigned int index, unsigned int ipl, unsigned int fop )
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	auto&	result= TestResult[index];
	result.Lop= ipl * fop;
	result.Fop= fop;
}

void	TestBase::SetOp2f( unsigned int index, unsigned int ipl, float fop )
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	auto&	result= TestResult[index];
	result.Lop= static_cast<unsigned int>( ipl * fop );
	result.Fop= fop;
}

void	TestBase::SetResult( unsigned int index, uint64_t time )
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	auto&	result= TestResult[index];
	result.Time= time;
}



unsigned int	TestBase::GetResult( unsigned int index ) const
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	return	static_cast<uint32_t>( TestResult[index].Time );
}
unsigned int	TestBase::GetLoopOp( unsigned int index ) const
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	return	TestResult[index].Lop;
}
float	TestBase::GetInstFop( unsigned int index ) const
{
	FL_ASSERT( index < RESULT_BUFFER_MAX );
	return	TestResult[index].Fop;
}

