// 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"MIPSSP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_MIPS32 || flCPU_MIPS64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace MIPSSP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


typedef	uint32_t	CounterType;


//=============================================================================
// MIPS
//=============================================================================

static void check_result( float ret, float ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	flASSERT( ret == ans );
}



#define FPU_REG_CLEAR() \
		"mtc1	$0,$f0\n"	\
		"mtc1	$0,$f2\n"	\
		"mtc1	$0,$f4\n"	\
		"mtc1	$0,$f6\n"	\
		"mtc1	$0,$f8\n"	\
		"mtc1	$0,$f10\n"	\
		"mtc1	$0,$f12\n"	\
		"mtc1	$0,$f14\n"	\
		"mtc1	$0,$f16\n"	\
		"mtc1	$0,$f18\n"	\
		"mtc1	$0,$f20\n"	\
		"mtc1	$0,$f22\n"	\
		"mtc1	$0,$f24\n"	\
		"mtc1	$0,$f26\n"	\
		"mtc1	$0,$f28\n"	\
		"mtc1	$0,$f30\n"	\




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
static uint64_t FPU_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.s.w	$f16, $f0\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.s.w	$f18, $f0\n"	\
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
		"swc1	$f0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "$12", "$13",\
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
static uint64_t FPU_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.s.w	$f16, $f0\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f0\n"		\
		"cvt.s.w	$f18, $f0\n"	\
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
		"swc1	$f0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "$12", "$13",\
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
static uint64_t FPU_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.s.w	$f16, $f20\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.s.w	$f18, $f20\n"	\
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
		"swc1	$f0, %[o0]\n"	\
		"swc1	$f2, %[o1]\n"	\
		"swc1	$f4, %[o2]\n"	\
		"swc1	$f6, %[o3]\n"	\
		"swc1	$f8, %[o4]\n"	\
		"swc1	$f10, %[o5]\n"	\
		"swc1	$f12, %[o6]\n"	\
		"swc1	$f14, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "$12", "$13",\
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
static uint64_t FPU_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		FPU_REG_CLEAR()			\
		"li		$12, 2\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.s.w	$f16, $f20\n"	\
		"li		$12, 5\n"		\
		"mtc1	$12, $f20\n"		\
		"cvt.s.w	$f18, $f20\n"	\
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
		"swc1	$f0, %[o0]\n"	\
		"swc1	$f2, %[o1]\n"	\
		"swc1	$f4, %[o2]\n"	\
		"swc1	$f6, %[o3]\n"	\
		"swc1	$f8, %[o4]\n"	\
		"swc1	$f10, %[o5]\n"	\
		"swc1	$f12, %[o6]\n"	\
		"swc1	$f14, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "$12", "$13",\
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

	"FPU mul.s (32bit x1) n8",
	"FPU add.s (32bit x1) n8",
	"FPU madd.s (32bit x1) n8",

	"FPU mul.s (32bit x1) n1",
	"FPU add.s (32bit x1) n1",
	"FPU madd.s (32bit x1) n1",
};



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_FPU_MULS_IR8,		PER_LOOP_INST * 1	);
	SetOp( RESULT_FPU_ADDS_IR8,		PER_LOOP_INST * 1	);
	SetOp( RESULT_FPU_MADDS_IR8,	PER_LOOP_INST * 2	);

	SetOp( RESULT_FPU_MULS_IR1,		PER_LOOP_INST * 1	);
	SetOp( RESULT_FPU_ADDS_IR1,		PER_LOOP_INST * 1	);
	SetOp( RESULT_FPU_MADDS_IR1,	PER_LOOP_INST * 2	);

}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



FPU_S_IR8( mul.s,  muls_ir8 );
FPU_S_IR8( add.s,  adds_ir8 );
FPU_S_IR8_MAD( madd.s,  madds_ir8 );

FPU_S_IR1( mul.s,  muls_ir1 );
FPU_S_IR1( add.s,  adds_ir1 );
FPU_S_IR1_MAD( madd.s,  madds_ir1 );






void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "MIPS loop=%d\n", Loop );

	float	sum= 0;
	float	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 2.0f;
		sum_2+= 10.0f;
	}




	//------------------------------------------------------
	// IR8
	//------------------------------------------------------
	SetResult( RESULT_FPU_MULS_IR8,	FPU_S_IR8_muls_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_FPU_ADDS_IR8,	FPU_S_IR8_adds_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_FPU_MADDS_IR8,FPU_S_IR8_madds_ir8( Loop, sum_2	) );
	Progress++;


	//------------------------------------------------------
	// IR1
	//------------------------------------------------------
	SetResult( RESULT_FPU_MULS_IR1,	FPU_S_IR1_muls_ir1( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_FPU_ADDS_IR1,	FPU_S_IR1_adds_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_FPU_MADDS_IR1,	FPU_S_IR1_madds_ir1( Loop, 0.0f		) );
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
		return	"FPU (SP fp)";
	}else{
		return	"FPU (SP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // mips
//-----------------------------------------------------------------------------

