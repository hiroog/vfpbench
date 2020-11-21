// 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"MIPSDP_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_MIPS32 || FL_CPU_MIPS64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace MIPSDP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


typedef	uint32_t	CounterType;


//=============================================================================
// MIPS
//=============================================================================

static void check_result( double ret, double ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	flASSERT( ret == ans );
}



#define FPU_REG_CLEAR() \
		"add	$12,$0,$0\n"	\
		"add	$13,$0,$0\n"	\
		"mtc1	$12,$f0\n"	\
		"mtc1	$12,$f2\n"	\
		"mtc1	$12,$f4\n"	\
		"mtc1	$12,$f6\n"	\
		"mtc1	$12,$f8\n"	\
		"mtc1	$12,$f10\n"	\
		"mtc1	$12,$f12\n"	\
		"mtc1	$12,$f14\n"	\
		"mtc1	$12,$f16\n"	\
		"mtc1	$12,$f18\n"	\
		"mtc1	$12,$f20\n"	\
		"mtc1	$12,$f22\n"	\
		"mtc1	$12,$f24\n"	\
		"mtc1	$12,$f26\n"	\
		"mtc1	$12,$f28\n"	\
		"mtc1	$12,$f30\n"	\
		"mthc1	$12,$f0\n"	\
		"mthc1	$12,$f2\n"	\
		"mthc1	$12,$f4\n"	\
		"mthc1	$12,$f6\n"	\
		"mthc1	$12,$f8\n"	\
		"mthc1	$12,$f10\n"	\
		"mthc1	$12,$f12\n"	\
		"mthc1	$12,$f14\n"	\
		"mthc1	$12,$f16\n"	\
		"mthc1	$12,$f18\n"	\
		"mthc1	$12,$f20\n"	\
		"mthc1	$12,$f22\n"	\
		"mthc1	$12,$f24\n"	\
		"mthc1	$12,$f26\n"	\
		"mthc1	$12,$f28\n"	\
		"mthc1	$12,$f30\n"	\




//-----------------------------------------------------------------------------
// Single Interleave 1
//-----------------------------------------------------------------------------

#define FPU_S_IR1_8(op) \
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"		\
			op " $f0, $f16, $f18\n"



#define FPU_S_IR1_0(op,name) \
static uint64_t FPU_S_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.d.w	$f16, $f0\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.d.w	$f18, $f0\n"	\
		"move	$13,%[loop]\n"	\
	"1:\n"	\
		FPU_S_IR1_8( op )	\
		FPU_S_IR1_8( op )	\
		FPU_S_IR1_8( op )	\
		FPU_S_IR1_8( op )	\
		FPU_S_IR1_8( op )	\
		"addi	$13,-1\n"	\
		"bne	$13,$0,1b\n"	 	\
		"nop\n"	\
		"nop\n"	\
		"sdc1	$f0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "$1", "$12", "$13",\
		"$f16","$f18","$f0" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer );	\
	return	timer.Result();	\
}

#define FPU_S_IR1(op,name) FPU_S_IR1_0(#op,name)







//-----------------------------------------------------------------------------
// Single Interleave 1 MAD
//-----------------------------------------------------------------------------

#define FPU_S_IR1_8_MAD(op) \
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"		\
			op " $f0, $f0, $f16, $f18\n"



#define FPU_S_IR1_0_MAD(op,name) \
static uint64_t FPU_S_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.d.w	$f16, $f0\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.d.w	$f18, $f0\n"	\
		"move	$13,%[loop]\n"	\
	"1:\n"	\
		FPU_S_IR1_8_MAD( op )	\
		FPU_S_IR1_8_MAD( op )	\
		FPU_S_IR1_8_MAD( op )	\
		FPU_S_IR1_8_MAD( op )	\
		FPU_S_IR1_8_MAD( op )	\
		"addi	$13,-1\n"	\
		"bne	$13,$0,1b\n"	 	\
		"nop\n"	\
		"nop\n"	\
		"sdc1	$f0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "$1", "$12", "$13",\
		"$f16","$f18","$f0" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );	*/	\
	return	timer.Result();	\
}

#define FPU_S_IR1_MAD(op,name) FPU_S_IR1_0_MAD(#op,name)









//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define FPU_S_IR8_8(op) \
			op " $f0,  $f16, $f18\n"		\
			op " $f2,  $f16, $f18\n"		\
			op " $f4,  $f16, $f18\n"		\
			op " $f6,  $f16, $f18\n"		\
			op " $f8,  $f16, $f18\n"		\
			op " $f10, $f16, $f18\n"		\
			op " $f12, $f16, $f18\n"		\
			op " $f14, $f16, $f18\n"



#define FPU_S_IR8_0(op,name) \
static uint64_t FPU_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.d.w	$f16, $f20\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.d.w	$f18, $f20\n"	\
		"move	$13,%[loop]\n"	\
	"1:\n"	\
		FPU_S_IR8_8( op )	\
		FPU_S_IR8_8( op )	\
		FPU_S_IR8_8( op )	\
		FPU_S_IR8_8( op )	\
		FPU_S_IR8_8( op )	\
		"addi	$13,-1\n"	\
		"bne	$13,$0,1b\n"	 	\
		"nop\n"	\
		"nop\n"	\
		"sdc1	$f0, %[o0]\n"	\
		"sdc1	$f2, %[o1]\n"	\
		"sdc1	$f4, %[o2]\n"	\
		"sdc1	$f6, %[o3]\n"	\
		"sdc1	$f8, %[o4]\n"	\
		"sdc1	$f10, %[o5]\n"	\
		"sdc1	$f12, %[o6]\n"	\
		"sdc1	$f14, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "$1", "$12", "$13",\
		"$f16","$f18","$f20", \
		"$f0", "$f2", "$f4", "$f6", "$f8", "$f10", "$f12", "$f14" ); \
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

#define FPU_S_IR8(op,name) FPU_S_IR8_0(#op,name)




//-----------------------------------------------------------------------------
// Single Interleave 8 MADD
//-----------------------------------------------------------------------------

#define FPU_S_IR8_8_MAD(op) \
			op " $f0,  $f0, $f16, $f18\n"		\
			op " $f2,  $f2, $f16, $f18\n"		\
			op " $f4,  $f4, $f16, $f18\n"		\
			op " $f6,  $f6, $f16, $f18\n"		\
			op " $f8,  $f8, $f16, $f18\n"		\
			op " $f10, $f10, $f16, $f18\n"		\
			op " $f12, $f12, $f16, $f18\n"		\
			op " $f14, $f14, $f16, $f18\n"



#define FPU_S_IR8_0_MAD(op,name) \
static uint64_t FPU_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.d.w	$f16, $f20\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.d.w	$f18, $f20\n"	\
		"move	$13,%[loop]\n"	\
	"1:\n"	\
		FPU_S_IR8_8_MAD( op )	\
		FPU_S_IR8_8_MAD( op )	\
		FPU_S_IR8_8_MAD( op )	\
		FPU_S_IR8_8_MAD( op )	\
		FPU_S_IR8_8_MAD( op )	\
		"addi	$13,-1\n"	\
		"bne	$13,$0,1b\n"	 	\
		"nop\n"	\
		"nop\n"	\
		"sdc1	$f0, %[o0]\n"	\
		"sdc1	$f2, %[o1]\n"	\
		"sdc1	$f4, %[o2]\n"	\
		"sdc1	$f6, %[o3]\n"	\
		"sdc1	$f8, %[o4]\n"	\
		"sdc1	$f10, %[o5]\n"	\
		"sdc1	$f12, %[o6]\n"	\
		"sdc1	$f14, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "$1", "$12", "$13",\
		"$f16","$f18","$f20", \
		"$f0", "$f2", "$f4", "$f6", "$f8", "$f10", "$f12", "$f14" ); \
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

#define FPU_S_IR8_MAD(op,name) FPU_S_IR8_0_MAD(#op,name)

























//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




static const char*	Instruction_Title[]= {

	"FPU mul.d (64bit x1) n8",
	"FPU add.d (64bit x1) n8",
	"FPU madd.d (64bit x1) n8",

	"FPU mul.d (64bit x1) n1",
	"FPU add.d (64bit x1) n1",
	"FPU madd.d (64bit x1) n1",
};






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_FPU_MULD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_FPU_ADDD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_FPU_MADDD_IR8,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_FPU_MULD_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_FPU_ADDD_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_FPU_MADDD_IR1,	PER_LOOP_INST * 2, 2	);

}








//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



FPU_S_IR8( mul.d,  muld_ir8 );
FPU_S_IR8( add.d,  addd_ir8 );
FPU_S_IR8_MAD( madd.d,  maddd_ir8 );

FPU_S_IR1( mul.d,  muld_ir1 );
FPU_S_IR1( add.d,  addd_ir1 );
FPU_S_IR1_MAD( madd.d,  maddd_ir1 );






void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "MIPSDP loop=%d\n", Loop );

	double	sum= 0;
	double	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 2.0f;
		sum_2+= 10.0f;
	}




	//------------------------------------------------------
	// IR8
	//------------------------------------------------------
	SetResult( RESULT_FPU_MULD_IR8,	FPU_S_IR8_muld_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_FPU_ADDD_IR8,	FPU_S_IR8_addd_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_FPU_MADDD_IR8,FPU_S_IR8_maddd_ir8( Loop, sum_2	) );
	Progress++;


	//------------------------------------------------------
	// IR1
	//------------------------------------------------------
	SetResult( RESULT_FPU_MULD_IR1,	FPU_S_IR1_muld_ir1( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_FPU_ADDD_IR1,	FPU_S_IR1_addd_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_FPU_MADDD_IR1,	FPU_S_IR1_maddd_ir1( Loop, 0.0f		) );
	Progress++;


	//------------------------------------------------------
	//------------------------------------------------------

	DoneFlag= true;
}


const char*	FloatTest::GetInstructionName( unsigned int result_index ) const
{
	flASSERT( result_index < GetResultCount() );
	flASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
	return	Instruction_Title[result_index];
}

const char*	FloatTest::GetTestName() const
{
	if( !IsMultithread() ){
		return	"FPU (DP fp)";
	}else{
		return	"FPU (DP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // mips
//-----------------------------------------------------------------------------

