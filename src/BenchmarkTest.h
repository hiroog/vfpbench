// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	BENCHMARK_TEST_H_
#define	BENCHMARK_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/ut/FixedArray.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"ThreadAdapter.h"
#include	"MultiAdapter.h"

#define	USE_MATRIX_TEST		0

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class BenchmarkTestBase {
public:
	flatlib::ut::FixedArray<ITestBase*>	BenchArray;
	unsigned int	BenchIndex= 0;
private:
	void	Quit();
public:
	BenchmarkTestBase();
	virtual ~BenchmarkTestBase();
	unsigned int	Init( unsigned int unit_count );
	void			AddBench( ITestBase* bench );
	template<typename T>
	void			AddBenchSingle( unsigned int group )
	{
		AddBench( FL_MEMORY::New<ThreadAdapter<T>>( group ) );
	}
	template<typename T>
	void			AddBenchMulti( unsigned int group )
	{
		AddBench( FL_MEMORY::New<MultiAdapter<T>>( group ) );
	}
	unsigned int	GetBenchCount() const;
	ITestBase*		GetBenchmark( unsigned int index ) const;
	void	SetLoop( unsigned int index, unsigned int loop );
	void	UpdateLoop( float loop_scale );
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CPU_ARM7 || FL_CPU_ARM6

# include	"VFP32SP_Test.h"
# include	"VFP32DP_Test.h"
typedef	VFP32SP::FloatTest		FPU_SP_Test;
typedef	VFP32DP::FloatTest		FPU_DP_Test;
#if USE_MATRIX_TEST
# include	"MatrixTest_VFP32SP.h"
typedef	VFP32SP::MatrixTest		MatrixTest;
#endif

class BenchmarkTest : public BenchmarkTestBase {
public:
	BenchmarkTest()
	{
#if USE_MATRIX_TEST
		unsigned int	group_count= Init( 6 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchSingle<MatrixTest>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
			AddBenchMulti<MatrixTest>( gi );
		}
#else
		unsigned int	group_count= Init( 4 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
		}
#endif
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CPU_ARM64

# include	"VFP64HP_Test.h"
# include	"VFP64SP_Test.h"
# include	"VFP64DP_Test.h"
typedef	VFP64HP::FloatTest	FPU_HP_Test;
typedef	VFP64SP::FloatTest	FPU_SP_Test;
typedef	VFP64DP::FloatTest	FPU_DP_Test;
#if USE_MATRIX_TEST
# include	"MatrixTest_VFP64SP.h"
typedef	VFP64SP::MatrixTest	MatrixTest;
#endif

class BenchmarkTest : public BenchmarkTestBase {
public:
	BenchmarkTest()
	{
#if USE_MATRIX_TEST
		unsigned int	test_count= 6;
#else
		unsigned int	test_count= 4;
#endif
		bool	half= flatlib::Info.HasInstructionSet( flatlib::CPUFeature::ARM_FPHP );
		if( half ){
			test_count+= 2;
		}
		unsigned int	group_count= Init( test_count );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			if( half ){
				AddBenchSingle<FPU_HP_Test>( gi );
			}
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
#if USE_MATRIX_TEST
			AddBenchSingle<MatrixTest>( gi );
#endif
			if( half ){
				AddBenchMulti<FPU_HP_Test>( gi );
			}
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
#if USE_MATRIX_TEST
			AddBenchMulti<MatrixTest>( gi );
#endif
		}
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CPU_X86

# include	"SSE32SP_Test.h"
# include	"SSE32DP_Test.h"
typedef	SSE32SP::FloatTest		FPU_SP_Test;
typedef	SSE32DP::FloatTest		FPU_DP_Test;
#if USE_MATRIX_TEST
# include	"MatrixTest_SSESP.h"
typedef	SSESP::MatrixTest		MatrixTest;
#endif

class BenchmarkTest : public BenchmarkTestBase {
public:
	BenchmarkTest()
	{
#if USE_MATRIX_TEST
		unsigned int	group_count= Init( 6 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchSingle<MatrixTest>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
			AddBenchMulti<MatrixTest>( gi );
		}
#else
		unsigned int	group_count= Init( 4 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
		}
#endif
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CPU_X64

# include	"SSE64SP_Test.h"
# include	"SSE64DP_Test.h"
typedef	SSE64SP::FloatTest	FPU_SP_Test;
typedef	SSE64DP::FloatTest	FPU_DP_Test;
#if USE_MATRIX_TEST
# include	"MatrixTest_SSESP.h"
typedef	SSESP::MatrixTest	MatrixTest;
#endif

class BenchmarkTest : public BenchmarkTestBase {
public:
	BenchmarkTest()
	{
#if USE_MATRIX_TEST
		unsigned int	group_count= Init( 6 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchSingle<MatrixTest>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
			AddBenchMulti<MatrixTest>( gi );
		}
#else
		unsigned int	group_count= Init( 4 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
		}
#endif
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CPU_MIPS32 || FL_CPU_MIPS64

# include	"MIPSSP_Test.h"
# include	"MIPSDP_Test.h"
typedef	MIPSSP::FloatTest	FPU_SP_Test;
typedef	MIPSDP::FloatTest	FPU_DP_Test;

class BenchmarkTest : public BenchmarkTestBase {
public:
	BenchmarkTest()
	{
		unsigned int	group_count= Init( 4 );
		for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
			AddBenchSingle<FPU_SP_Test>( gi );
			AddBenchSingle<FPU_DP_Test>( gi );
			AddBenchMulti<FPU_SP_Test>( gi );
			AddBenchMulti<FPU_DP_Test>( gi );
		}
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif

