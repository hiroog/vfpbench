// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP32DP_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_ARM7 || FL_CPU_ARM6
//-----------------------------------------------------------------------------

#if defined(__ARM_VFPV4__)
# define	FL_CPU_VFPV4	1
#endif

using namespace flatlib;
using system::CPUFeature;

namespace VFP32DP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
    q0             |q1             |q2             |q3             |
    d0      d1     |d2      d3     |d4      d5     |d6      d7     |
	s0  s1  s2  s3 |s4  s5  s6  s7 |s8  s9  s10 s11|s12 s13 s14 s15|

    q4             |q5             |q6             |q7             |
    d8      d9     |d10     d11    |d12     d13    |d14     d15    |
	s16 s17 s18 s19|s20 s21 s22 s23|s24 s25 s26 s27|s28 s29 s30 s31|

    q8             |q9             |q10            |q11            |
    d16     d17    |d18     d19    |d20     d21    |d22     d23    |
	d16[0]  d17[0] |d18[0]  d19[0] |d20[0]  d21[0] |d22[0]  d23[0] |

    q12            |q13            |q14            |q15            |
    d24     d25    |d26     d27    |d28     d29    |d30     d31    |
	d24[0]  d25[0] |d26[0]  d27[0] |d28[0]  d29[0] |d30[0]  d31[0] |
*/




//=============================================================================
// VFP
//=============================================================================

static void check_result( double ret, double ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	FL_ASSERT( ret == ans );
}


#define VFP_REG_CLEAR() \
		"mov	r0, #0\n"		\
		"fmdrr	d0, r0, r0\n"	\
		"fmdrr	d1, r0, r0\n"	\
		"fmdrr	d2, r0, r0\n"	\
		"fmdrr	d3, r0, r0\n"	\
		"fmdrr	d4, r0, r0\n"	\
		"fmdrr	d5, r0, r0\n"	\
		"fmdrr	d6, r0, r0\n"	\
		"fmdrr	d7, r0, r0\n"	\
		"fmdrr	d8, r0, r0\n"	\
		"fmdrr	d9, r0, r0\n"	\
		"fmdrr	d10, r0, r0\n"	\
		"fmdrr	d11, r0, r0\n"	\
		"fmdrr	d12, r0, r0\n"	\
		"fmdrr	d13, r0, r0\n"	\
		"fmdrr	d14, r0, r0\n"	\
		"fmdrr	d15, r0, r0\n"




//-----------------------------------------------------------------------------
// Double  Interleave 1
//-----------------------------------------------------------------------------

#define VFP_D_IR1_8(op) \
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"



#define VFP_D_IR1_0(op,name) \
static uint64_t VFP_D_IR1_##name( unsigned int LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d10, r2, r3\n"	\
		"fsitod	d8, s20\n"		\
		"fsitod	d9, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_D_IR1_8( op )	\
		VFP_D_IR1_8( op )	\
		VFP_D_IR1_8( op )	\
		VFP_D_IR1_8( op )	\
		VFP_D_IR1_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fstd	d0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","r0","r2","r3",	\
	"d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","d10","d11","d12","d13","d14","d15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	return	timer.Result();	\
}

#define VFP_D_IR1(op,name) VFP_D_IR1_0(#op,name)







//-----------------------------------------------------------------------------
// Double  Interleave 8
//-----------------------------------------------------------------------------

#define VFP_D_IR8_8(op) \
			op " d0, d8, d9 \n"		\
			op " d1, d8, d9 \n"		\
			op " d2, d8, d9 \n"		\
			op " d3, d8, d9 \n"		\
			op " d4, d8, d9 \n"		\
			op " d5, d8, d9 \n"		\
			op " d6, d8, d9 \n"		\
			op " d7, d8, d9 \n"



#define VFP_D_IR8_0(op,name) \
static uint64_t VFP_D_IR8_##name( unsigned int LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d10, r2, r3\n"	\
		"fsitod	d8, s20\n"		\
		"fsitod	d9, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_D_IR8_8( op )	\
		VFP_D_IR8_8( op )	\
		VFP_D_IR8_8( op )	\
		VFP_D_IR8_8( op )	\
		VFP_D_IR8_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fstd	d0, %[o0]\n"	\
		"fstd	d1, %[o1]\n"	\
		"fstd	d2, %[o2]\n"	\
		"fstd	d3, %[o3]\n"	\
		"fstd	d4, %[o4]\n"	\
		"fstd	d5, %[o5]\n"	\
		"fstd	d6, %[o6]\n"	\
		"fstd	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","r0","r2","r3",	\
	"d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","d10","d11","d12","d13","d14","d15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );	\
	check_result( ret4, answer );	\
	check_result( ret5, answer );	\
	check_result( ret6, answer );	\
	check_result( ret7, answer );	\
	return	timer.Result();	\
}

#define VFP_D_IR8(op,name) VFP_D_IR8_0(#op,name)





//-----------------------------------------------------------------------------
// Double  Interleave src 4
//-----------------------------------------------------------------------------

#define VFP_D_IRS4_8(op) \
			op " d0, d8, d4 \n"		\
			op " d1, d8, d5 \n"		\
			op " d2, d8, d6 \n"		\
			op " d3, d8, d7 \n"		\
			op " d4, d8, d0 \n"		\
			op " d5, d8, d1 \n"		\
			op " d6, d8, d2 \n"		\
			op " d7, d8, d3 \n"



#define VFP_D_IRS4_0(op,name) \
static uint64_t VFP_D_IRS4_##name( unsigned int LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #1  \n"		\
		"mov	r3, #2  \n"		\
		"fmdrr  d10, r2, r3\n"	\
		"fsitod	d8, s20\n"		\
		"fsitod	d9, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_D_IRS4_8( op )	\
		VFP_D_IRS4_8( op )	\
		VFP_D_IRS4_8( op )	\
		VFP_D_IRS4_8( op )	\
		VFP_D_IRS4_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fstd	d0, %[o0]\n"	\
		"fstd	d1, %[o1]\n"	\
		"fstd	d2, %[o2]\n"	\
		"fstd	d3, %[o3]\n"	\
		"fstd	d4, %[o4]\n"	\
		"fstd	d5, %[o5]\n"	\
		"fstd	d6, %[o6]\n"	\
		"fstd	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","r0","r2","r3",	\
	"d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","d10","d11","d12","d13","d14","d15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );	\
	check_result( ret4, answer );	\
	check_result( ret5, answer );	\
	check_result( ret6, answer );	\
	check_result( ret7, answer );*/	\
	return	timer.Result();	\
}

#define VFP_D_IRS4(op,name) VFP_D_IRS4_0(#op,name)




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


static const char*	Instruction_Title[]= {


	"VFP fmuld (64bit x1) n8",
	"VFP faddd (64bit x1) n8",
	"VFP fmacd (64bit x1) n8",
	"VFP vfma.f64 (64bit x1) n8",

	"VFP fmuld (64bit x1) ns4",
	"VFP faddd (64bit x1) ns4",
	"VFP fmacd (64bit x1) ns4",
	"VFP vfma.f64 (64bit x1) ns4",

	"VFP fmuld (64bit x1) n1",
	"VFP faddd (64bit x1) n1",
	"VFP fmacd (64bit x1) n1",
	"VFP vfma.f64 (64bit x1) n1",

};





//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	FL_ASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_VFP_FMULD_IR8,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDD_IR8,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACD_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F64_IR8,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_VFP_FMULD_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDD_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACD_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F64_IRS4,PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_VFP_FMULD_IR1,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDD_IR1,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACD_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F64_IR1,	PER_LOOP_INST * 2, 2	);

}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


VFP_D_IR8( fmuld, fmuld_ir8 );
VFP_D_IR8( faddd, faddd_ir8 );
VFP_D_IR8( fmacd, fmacd_ir8 );

VFP_D_IRS4( fmuld, fmuld_irs4 );
VFP_D_IRS4( faddd, faddd_irs4 );
VFP_D_IRS4( fmacd, fmacd_irs4 );

VFP_D_IR1( fmuld, fmuld_ir1 );
VFP_D_IR1( faddd, faddd_ir1 );
VFP_D_IR1( fmacd, fmacd_ir1 );

#if FL_CPU_VFPV4
VFP_D_IR8(  vfma.f64, vfma_f64_ir8 );
VFP_D_IRS4( vfma.f64, vfma_f64_irs4 );
VFP_D_IR1(  vfma.f64, vfma_f64_ir1 );
#endif




void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP2(dp) loop=%d\n", Loop );

	//---------------------------------------
	// IR8
	//---------------------------------------

	SetResult( RESULT_VFP_FMULD_IR8,	VFP_D_IR8_fmuld_ir8( Loop, 10.0		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADDD_IR8,	VFP_D_IR8_faddd_ir8( Loop, 7.0		) );
	Progress.Increment();

	double	sum= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 10.0;
	}

	SetResult( RESULT_VFP_FMACD_IR8,	VFP_D_IR8_fmacd_ir8( Loop, sum ) );
	Progress.Increment();

#if FL_CPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F64_IR8,	VFP_D_IR8_vfma_f64_ir8( Loop, sum ) );
#endif
	Progress.Increment();

	//---------------------------------------
	// IRS4
	//---------------------------------------


	SetResult( RESULT_VFP_FMULD_IRS4,	VFP_D_IRS4_fmuld_irs4( Loop, 10.0		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADDD_IRS4,	VFP_D_IRS4_faddd_irs4( Loop, 7.0		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FMACD_IRS4,	VFP_D_IRS4_fmacd_irs4( Loop, 0.0 ) );
	Progress.Increment();

#if FL_CPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F64_IRS4,	VFP_D_IRS4_vfma_f64_irs4( Loop, 0.0 ) );
#endif
	Progress.Increment();

	//---------------------------------------
	// IR1
	//---------------------------------------


	SetResult( RESULT_VFP_FMULD_IR1,	VFP_D_IR1_fmuld_ir1( Loop, 10.0		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADDD_IR1,	VFP_D_IR1_faddd_ir1( Loop, 7.0		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FMACD_IR1,	VFP_D_IR1_fmacd_ir1( Loop, 0.0 ) );
	Progress.Increment();

#if FL_CPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F64_IR1,	VFP_D_IR1_vfma_f64_ir1( Loop, 0.0 ) );
#endif
	Progress.Increment();

	//---------------------------------------
	//---------------------------------------

	DoneFlag= true;
}


const char*	FloatTest::GetInstructionName( unsigned int result_index ) const
{
	FL_ASSERT( result_index < GetResultCount() );
	FL_ASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
	return	Instruction_Title[result_index];
}


const char*	FloatTest::GetTestName() const
{
	if( !IsMultithread() ){
		return	"VFP/NEON (DP fp)";
	}else{
		return	"VFP/NEON (DP fp) multi-thread";
	}
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

