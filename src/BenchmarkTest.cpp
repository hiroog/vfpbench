// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/CoreContext.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"BenchmarkTest.h"

using namespace flatlib;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

BenchmarkTestBase::BenchmarkTestBase()
{
}

BenchmarkTestBase::~BenchmarkTestBase()
{
	Quit();
}

void	BenchmarkTestBase::Quit()
{
	unsigned int	bench_count= BenchArray.GetDataSize();
	for( unsigned int bi= 0 ; bi< bench_count ; bi++ ){
		memory::ZDelete( BenchArray[bi] );
	}
}

unsigned int	BenchmarkTestBase::Init( unsigned int unit_count )
{
	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	group_count= Info.GetCoreGroupCount();
	BenchArray.SetArraySize( unit_count * group_count );
	BenchIndex= 0;
	return	group_count;
}

void	BenchmarkTestBase::AddBench( ITestBase* bench )
{
	BenchArray[BenchIndex]= bench;
	BenchIndex++;
}

unsigned int	BenchmarkTestBase::GetBenchCount() const
{
	return	BenchArray.GetDataSize();
}

ITestBase*		BenchmarkTestBase::GetBenchmark( unsigned int index ) const
{
	return	BenchArray[ index ];
}

void	BenchmarkTestBase::SetLoop( unsigned int index, unsigned int loop )
{
	if( loop ){
		GetBenchmark( index )->SetLoop( loop );
	}
}

void	BenchmarkTestBase::UpdateLoop( float loop_sclae )
{
	const auto&	Info= system::RCore().RSystemInfo();
	unsigned int	bench_count= GetBenchCount();
	for( unsigned int bi= 0 ; bi< bench_count ; bi++ ){
		auto*	bench= BenchArray[bi];
		unsigned int	group= bench->GetCoreGroup();
		unsigned int	kclock= Info.GetCoreClock( group );
		double			gclock= kclock ? kclock / 1000000.0 : 1.0;
		double			clock_scale= gclock / 2.0;
		unsigned int	hp_default= static_cast<unsigned int>( 30000000 * loop_sclae * clock_scale );
		unsigned int	sp_default= static_cast<unsigned int>( 30000000 * loop_sclae * clock_scale );
		unsigned int	dp_default= static_cast<unsigned int>( 30000000 * loop_sclae * clock_scale );
		unsigned int	mat_default= static_cast<unsigned int>( 1000000 * loop_sclae * clock_scale );
		unsigned int	loop= 0;
		switch( bench->GetLoopType() ){
		case LOOPTYPE_HP: loop= hp_default; break;
		case LOOPTYPE_SP: loop= sp_default; break;
		case LOOPTYPE_DP: loop= dp_default; break;
		case LOOPTYPE_MAT: loop= mat_default; break;
		}
		FL_LOG( "%2d: loop=%8d  %s\n", bi, loop, bench->GetTestName() );
		bench->SetLoop( loop );
	}
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
