// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE64SP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_X64
//-----------------------------------------------------------------------------
#if defined(__AVX512F__) && __AVX512F__
# define	ENABLE_AVX512	1
#else
# define	ENABLE_AVX512	0
#endif

using namespace flatlib;

namespace SSE64SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	xmm0-xmm7
*/

#define	USE_ASM_DEBUG_TEST	0


typedef	uint64_t CounterType;


//=============================================================================
// SSE
//=============================================================================

static void check_result( float ret, float ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	flASSERT( ret == ans );
}


#define SSE_REG_CLEAR() \
		"xorps	%%xmm0, %%xmm0\n"	\
		"xorps	%%xmm1, %%xmm1\n"	\
		"xorps	%%xmm2, %%xmm2\n"	\
		"xorps	%%xmm3, %%xmm3\n"	\
		"xorps	%%xmm4, %%xmm4\n"	\
		"xorps	%%xmm5, %%xmm5\n"	\
		"xorps	%%xmm6, %%xmm6\n"	\
		"xorps	%%xmm7, %%xmm7\n"	\
		"xorps	%%xmm8, %%xmm8\n"	\
		"xorps	%%xmm9, %%xmm9\n"	\
		"xorps	%%xmm10, %%xmm10\n"	\
		"xorps	%%xmm11, %%xmm11\n"	\
		"xorps	%%xmm12, %%xmm12\n"	\
		"xorps	%%xmm13, %%xmm13\n"	\
		"xorps	%%xmm14, %%xmm14\n"	\
		"xorps	%%xmm15, %%xmm15\n"	\





//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define SSE_S_IR8_8(op) \
			op " %%xmm0, %%xmm0\n"		\
			op " %%xmm1, %%xmm1\n"		\
			op " %%xmm2, %%xmm2\n"		\
			op " %%xmm3, %%xmm3\n"		\
			op " %%xmm4, %%xmm4\n"		\
			op " %%xmm5, %%xmm5\n"		\
			op " %%xmm6, %%xmm6\n"		\
			op " %%xmm7, %%xmm7\n"



#define SSE_S_IR8_0(op,name) \
static uint64_t SSE_S_IR8_##name( CounterType LoopCount, float answer, float f0, float f1 ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	float	fp0= f0;	\
	float	fp1= f1;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"movss	    %[fp0], %%xmm0\n"	\
		"shufps		$0, %%xmm0, %%xmm0\n"	\
		"movaps		%%xmm0, %%xmm2\n"	\
		"movaps		%%xmm0, %%xmm4\n"	\
		"movaps		%%xmm0, %%xmm6\n"	\
		"movss	    %[fp1], %%xmm1\n"	\
		"shufps		$0, %%xmm1, %%xmm1\n"	\
		"movaps		%%xmm1, %%xmm3\n"	\
		"movaps		%%xmm1, %%xmm5\n"	\
		"movaps		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount), [fp0]"m"(fp0), [fp1]"m"(fp1)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	FL_LOG( "%f %f %f %f %f %f %f %f\n", ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7 ); \
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

#define SSE_S_IR8(op,name) SSE_S_IR8_0(#op,name)











//-----------------------------------------------------------------------------
// Single Interleave src 4
//-----------------------------------------------------------------------------

#define SSE_S_IRS4_8(op) \
			op " %%xmm0, %%xmm4\n"		\
			op " %%xmm1, %%xmm5\n"		\
			op " %%xmm2, %%xmm6\n"		\
			op " %%xmm3, %%xmm7\n"		\
			op " %%xmm4, %%xmm0\n"		\
			op " %%xmm5, %%xmm1\n"		\
			op " %%xmm6, %%xmm2\n"		\
			op " %%xmm7, %%xmm3\n"



#define SSE_S_IRS4_0(op,name) \
static uint64_t SSE_S_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm0\n"	\
		"shufps		$0, %%xmm0, %%xmm0\n"	\
		"movaps		%%xmm0, %%xmm2\n"	\
		"movaps		%%xmm0, %%xmm4\n"	\
		"movaps		%%xmm0, %%xmm6\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm1\n"	\
		"shufps		$0, %%xmm1, %%xmm1\n"	\
		"movaps		%%xmm1, %%xmm3\n"	\
		"movaps		%%xmm1, %%xmm5\n"	\
		"movaps		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
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

#define SSE_S_IRS4(op,name) SSE_S_IRS4_0(#op,name)























//-----------------------------------------------------------------------------
// Single Interleave 8 MAD
//-----------------------------------------------------------------------------

#define SSE_M_IR8_8(op1,op2) \
			op1 " %%xmm0, %%xmm0\n"		\
			op2 " %%xmm1, %%xmm1\n"		\
			op1 " %%xmm2, %%xmm2\n"		\
			op2 " %%xmm3, %%xmm3\n"		\
			op1 " %%xmm4, %%xmm4\n"		\
			op2 " %%xmm5, %%xmm5\n"		\
			op1 " %%xmm6, %%xmm6\n"		\
			op2 " %%xmm7, %%xmm7\n"



#define SSE_M_IR8_0(op1,op2,name) \
static uint64_t SSE_M_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm0\n"	\
		"shufps		$0, %%xmm0, %%xmm0\n"	\
		"movaps		%%xmm0, %%xmm2\n"	\
		"movaps		%%xmm0, %%xmm4\n"	\
		"movaps		%%xmm0, %%xmm6\n"	\
		"movaps		%%xmm0, %%xmm8\n"	\
		"movaps		%%xmm0, %%xmm10\n"	\
		"movaps		%%xmm0, %%xmm12\n"	\
		"movaps		%%xmm0, %%xmm14\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm1\n"	\
		"shufps		$0, %%xmm1, %%xmm1\n"	\
		"movaps		%%xmm1, %%xmm3\n"	\
		"movaps		%%xmm1, %%xmm5\n"	\
		"movaps		%%xmm1, %%xmm7\n"	\
		"movaps		%%xmm1, %%xmm9\n"	\
		"movaps		%%xmm1, %%xmm11\n"	\
		"movaps		%%xmm1, %%xmm13\n"	\
		"movaps		%%xmm1, %%xmm15\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op1 );	\
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

#define SSE_M_IR8(op1,op2,name) SSE_M_IR8_0(#op1,#op2,name)








//-----------------------------------------------------------------------------
// Single Interleave 8 MAD op3
//-----------------------------------------------------------------------------

#define SSE_M_IR9_9(op1,op2,op3) \
			op1 " %%xmm0, %%xmm0\n"	\
			op2 " %%xmm1, %%xmm1\n"	\
			op3 " %%xmm2, %%xmm2\n"	\
			op1 " %%xmm3, %%xmm3\n"	\
			op2 " %%xmm4, %%xmm4\n"	\
			op3 " %%xmm5, %%xmm5\n"	\
			op1 " %%xmm6, %%xmm6\n"	\
			op2 " %%xmm7, %%xmm7\n"	\
			op3 " %%xmm8, %%xmm8\n"



#define SSE_M_IR9_0(op1,op2,op3,name) \
static uint64_t SSE_M_IR9_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm0\n"	\
		"shufps		$0, %%xmm0, %%xmm0\n"	\
		"movaps		%%xmm0, %%xmm2\n"	\
		"movaps		%%xmm0, %%xmm4\n"	\
		"movaps		%%xmm0, %%xmm6\n"	\
		"movaps		%%xmm0, %%xmm8\n"	\
		"movaps		%%xmm0, %%xmm10\n"	\
		"movaps		%%xmm0, %%xmm12\n"	\
		"movaps		%%xmm0, %%xmm14\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm1\n"	\
		"shufps		$0, %%xmm1, %%xmm1\n"	\
		"movaps		%%xmm1, %%xmm3\n"	\
		"movaps		%%xmm1, %%xmm5\n"	\
		"movaps		%%xmm1, %%xmm7\n"	\
		"movaps		%%xmm1, %%xmm9\n"	\
		"movaps		%%xmm1, %%xmm11\n"	\
		"movaps		%%xmm1, %%xmm13\n"	\
		"movaps		%%xmm1, %%xmm15\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op1 );	\
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

#define SSE_M_IR9(op1,op2,op3,name) SSE_M_IR9_0(#op1,#op2,#op3,name)






//-----------------------------------------------------------------------------
// Single Interleave 8 FMA
//-----------------------------------------------------------------------------

#define SSE_S_FMA_IR8_8(op) \
			op " %%xmm0, %%xmm0, %%xmm0\n"		\
			op " %%xmm1, %%xmm1, %%xmm1\n"		\
			op " %%xmm2, %%xmm2, %%xmm2\n"		\
			op " %%xmm3, %%xmm3, %%xmm3\n"		\
			op " %%xmm4, %%xmm4, %%xmm4\n"		\
			op " %%xmm5, %%xmm5, %%xmm5\n"		\
			op " %%xmm6, %%xmm6, %%xmm6\n"		\
			op " %%xmm7, %%xmm7, %%xmm7\n"



#define SSE_S_FMA_IR8_0(op,name) \
static uint64_t SSE_S_FMA_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm0\n"	\
		"shufps		$0, %%xmm0, %%xmm0\n"	\
		"movaps		%%xmm0, %%xmm2\n"	\
		"movaps		%%xmm0, %%xmm4\n"	\
		"movaps		%%xmm0, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm1\n"	\
		"shufps		$0, %%xmm1, %%xmm1\n"	\
		"movaps		%%xmm1, %%xmm3\n"	\
		"movaps		%%xmm1, %%xmm5\n"	\
		"movaps		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	FL_LOG( "%f %f %f %f %f %f %f %f\n", ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7 ); \
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

#define SSE_S_FMA_IR8(op,name) SSE_S_FMA_IR8_0(#op,name)



//-----------------------------------------------------------------------------
// Single Interleave Register 12 FMA
//-----------------------------------------------------------------------------

#define SSE_S_FMA_IR12_12(op) \
			op " %%xmm9, %%xmm8, %%xmm0\n"		\
			op " %%xmm9, %%xmm8, %%xmm1\n"		\
			op " %%xmm9, %%xmm8, %%xmm2\n"		\
			op " %%xmm9, %%xmm8, %%xmm3\n"		\
			op " %%xmm9, %%xmm8, %%xmm4\n"		\
			op " %%xmm9, %%xmm8, %%xmm5\n"		\
			op " %%xmm9, %%xmm8, %%xmm6\n"		\
			op " %%xmm9, %%xmm8, %%xmm7\n"		\
			op " %%xmm9, %%xmm8, %%xmm10\n"		\
			op " %%xmm9, %%xmm8, %%xmm11\n"		\
			op " %%xmm9, %%xmm8, %%xmm12\n"		\
			op " %%xmm9, %%xmm8, %%xmm13\n"



#define SSE_S_FMA_IR12_0(op,name) \
static uint64_t SSE_S_FMA_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"movaps		%%xmm8, %%xmm2\n"	\
		"movaps		%%xmm8, %%xmm4\n"	\
		"movaps		%%xmm8, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"movaps		%%xmm9, %%xmm3\n"	\
		"movaps		%%xmm9, %%xmm5\n"	\
		"movaps		%%xmm9, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	FL_LOG( "%f %f %f %f %f %f %f %f\n", ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7 ); \
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

#define SSE_S_FMA_IR12(op,name) SSE_S_FMA_IR12_0(#op,name)




//-----------------------------------------------------------------------------
// Single Interleave Register 12 FMA+SSE
//-----------------------------------------------------------------------------

#if 0
#define SSE_M_FMA_IR12_12(op1,op2) \
			op1 " %%xmm9, %%xmm8, %%xmm0\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm1\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm2\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm3\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm4\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm5\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm6\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm7\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm10\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm11\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm12\n"		\
			op2 " %%xmm9, %%xmm8, %%xmm13\n"
#else
#define SSE_M_FMA_IR12_12(op1,op2) \
			op1 " %%xmm9, %%xmm8, %%xmm0\n"		\
			op2 " %%xmm9, %%xmm1\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm2\n"		\
			op2 " %%xmm9, %%xmm3\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm4\n"		\
			op2 " %%xmm9, %%xmm5\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm6\n"		\
			op2 " %%xmm9, %%xmm7\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm10\n"		\
			op2 " %%xmm9, %%xmm11\n"		\
			op1 " %%xmm9, %%xmm8, %%xmm12\n"		\
			op2 " %%xmm9, %%xmm13\n"
#endif


#define SSE_M_FMA_IR12_0(op1,op2,name) \
static uint64_t SSE_M_FMA_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"movaps		%%xmm8, %%xmm2\n"	\
		"movaps		%%xmm8, %%xmm4\n"	\
		"movaps		%%xmm8, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"movaps		%%xmm9, %%xmm3\n"	\
		"movaps		%%xmm9, %%xmm5\n"	\
		"movaps		%%xmm9, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" ); \
\
	timer.End();		\
	timer.Dump( op1 );	\
	FL_LOG( "%f %f %f %f %f %f %f %f\n", ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7 ); \
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

#define SSE_M_FMA_IR12(op1,op2,name) SSE_M_FMA_IR12_0(#op1,#op2,name)






//=============================================================================
// AVX
//=============================================================================

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define AVX_REG_CLEAR() \
		"vxorps	%%ymm0, %%ymm0, %%ymm0\n"	\
		"vxorps	%%ymm1, %%ymm1, %%ymm1\n"	\
		"vxorps	%%ymm2, %%ymm2, %%ymm2\n"	\
		"vxorps	%%ymm3, %%ymm3, %%ymm3\n"	\
		"vxorps	%%ymm4, %%ymm4, %%ymm4\n"	\
		"vxorps	%%ymm5, %%ymm5, %%ymm5\n"	\
		"vxorps	%%ymm6, %%ymm6, %%ymm6\n"	\
		"vxorps	%%ymm7, %%ymm7, %%ymm7\n"	\
		"vxorps	%%ymm8, %%ymm8, %%ymm8\n"	\
		"vxorps	%%ymm9, %%ymm9, %%ymm9\n"	\
		"vxorps	%%ymm10, %%ymm10, %%ymm10\n"	\
		"vxorps	%%ymm11, %%ymm11, %%ymm11\n"	\
		"vxorps	%%ymm12, %%ymm12, %%ymm12\n"	\
		"vxorps	%%ymm13, %%ymm13, %%ymm13\n"	\
		"vxorps	%%ymm14, %%ymm14, %%ymm14\n"	\
		"vxorps	%%ymm15, %%ymm15, %%ymm15\n"	\



//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define AVX_S_IR8_8(op) \
			op " %%ymm9, %%ymm8, %%ymm0\n"		\
			op " %%ymm9, %%ymm8, %%ymm1\n"		\
			op " %%ymm9, %%ymm8, %%ymm2\n"		\
			op " %%ymm9, %%ymm8, %%ymm3\n"		\
			op " %%ymm9, %%ymm8, %%ymm4\n"		\
			op " %%ymm9, %%ymm8, %%ymm5\n"		\
			op " %%ymm9, %%ymm8, %%ymm6\n"		\
			op " %%ymm9, %%ymm8, %%ymm7\n"



#define AVX_S_IR8_0(op,name) \
static uint64_t AVX_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_S_IR8_8( op )	\
		AVX_S_IR8_8( op )	\
		AVX_S_IR8_8( op )	\
		AVX_S_IR8_8( op )	\
		AVX_S_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
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

#define AVX_S_IR8(op,name) AVX_S_IR8_0(#op,name)







//-----------------------------------------------------------------------------
// Single Interleave 8 MAD
//-----------------------------------------------------------------------------

#define AVX_M_IR8_8(op1,op2) \
			op1 " %%ymm9, %%ymm8, %%ymm0\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm1\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm2\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm3\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm4\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm5\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm6\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm7\n"



#define AVX_M_IR8_0(op1,op2,name) \
static uint64_t AVX_M_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_M_IR8_8( op1,op2 )	\
		AVX_M_IR8_8( op1,op2 )	\
		AVX_M_IR8_8( op1,op2 )	\
		AVX_M_IR8_8( op1,op2 )	\
		AVX_M_IR8_8( op1,op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
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

#define AVX_M_IR8(op1,op2,name) AVX_M_IR8_0(#op1,#op2,name)






//-----------------------------------------------------------------------------
// Single Interleave 9 MAD op3
//-----------------------------------------------------------------------------


#define AVX_M_IR9_9(op1,op2,op3) \
			op1 " %%ymm9, %%ymm8, %%ymm0\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm1\n"		\
			op3 " %%ymm9, %%ymm8, %%ymm2\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm3\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm4\n"		\
			op3 " %%ymm9, %%ymm8, %%ymm5\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm6\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm7\n"		\
			op3 " %%ymm9, %%ymm8, %%ymm8\n"



#define AVX_M_IR9_0(op1,op2,op3,name) \
static uint64_t AVX_M_IR9_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_M_IR9_9( op1,op2,op3 )	\
		AVX_M_IR9_9( op1,op2,op3 )	\
		AVX_M_IR9_9( op1,op2,op3 )	\
		AVX_M_IR9_9( op1,op2,op3 )	\
		AVX_M_IR9_9( op1,op2,op3 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
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

#define AVX_M_IR9(op1,op2,op3,name) AVX_M_IR9_0(#op1,#op2,#op3,name)



//-----------------------------------------------------------------------------
// Single Interleave 8 FMA
//-----------------------------------------------------------------------------

#define AVX_S_FMA_IR8_8(op) \
			op " %%ymm9, %%ymm8, %%ymm0\n"		\
			op " %%ymm9, %%ymm8, %%ymm1\n"		\
			op " %%ymm9, %%ymm8, %%ymm2\n"		\
			op " %%ymm9, %%ymm8, %%ymm3\n"		\
			op " %%ymm9, %%ymm8, %%ymm4\n"		\
			op " %%ymm9, %%ymm8, %%ymm5\n"		\
			op " %%ymm9, %%ymm8, %%ymm6\n"		\
			op " %%ymm9, %%ymm8, %%ymm7\n"



#define AVX_S_FMA_IR8_0(op,name) \
static uint64_t AVX_S_FMA_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_S_FMA_IR8_8( op )	\
		AVX_S_FMA_IR8_8( op )	\
		AVX_S_FMA_IR8_8( op )	\
		AVX_S_FMA_IR8_8( op )	\
		AVX_S_FMA_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
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

#define AVX_S_FMA_IR8(op,name) AVX_S_FMA_IR8_0(#op,name)





//-----------------------------------------------------------------------------
// Single Interleave 12 FMA
//-----------------------------------------------------------------------------

#define AVX_S_FMA_IR12_12(op) \
			op " %%ymm9, %%ymm8, %%ymm0\n"		\
			op " %%ymm9, %%ymm8, %%ymm1\n"		\
			op " %%ymm9, %%ymm8, %%ymm2\n"		\
			op " %%ymm9, %%ymm8, %%ymm3\n"		\
			op " %%ymm9, %%ymm8, %%ymm4\n"		\
			op " %%ymm9, %%ymm8, %%ymm5\n"		\
			op " %%ymm9, %%ymm8, %%ymm6\n"		\
			op " %%ymm9, %%ymm8, %%ymm7\n"		\
			op " %%ymm9, %%ymm8, %%ymm10\n"		\
			op " %%ymm9, %%ymm8, %%ymm11\n"		\
			op " %%ymm9, %%ymm8, %%ymm12\n"		\
			op " %%ymm9, %%ymm8, %%ymm13\n"



#define AVX_S_FMA_IR12_0(op,name) \
static uint64_t AVX_S_FMA_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_S_FMA_IR12_12( op )	\
		AVX_S_FMA_IR12_12( op )	\
		AVX_S_FMA_IR12_12( op )	\
		AVX_S_FMA_IR12_12( op )	\
		AVX_S_FMA_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
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

#define AVX_S_FMA_IR12(op,name) AVX_S_FMA_IR12_0(#op,name)




//-----------------------------------------------------------------------------
// Single Interleave Register 12 FMA+AVX
//-----------------------------------------------------------------------------

#define AVX_M_FMA_IR12_12(op1,op2) \
			op1 " %%ymm9, %%ymm8, %%ymm0\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm1\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm2\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm3\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm4\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm5\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm6\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm7\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm10\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm11\n"		\
			op1 " %%ymm9, %%ymm8, %%ymm12\n"		\
			op2 " %%ymm9, %%ymm8, %%ymm13\n"



#define AVX_M_FMA_IR12_0(op1,op2,name) \
static uint64_t AVX_M_FMA_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm8\n"	\
		"shufps		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2ss	%%eax, %%xmm9\n"	\
		"shufps		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_M_FMA_IR12_12( op1, op2 )	\
		AVX_M_FMA_IR12_12( op1, op2 )	\
		AVX_M_FMA_IR12_12( op1, op2 )	\
		AVX_M_FMA_IR12_12( op1, op2 )	\
		AVX_M_FMA_IR12_12( op1, op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movss	%%xmm0, %[o0]\n"	\
		"movss	%%xmm1, %[o1]\n"	\
		"movss	%%xmm2, %[o2]\n"	\
		"movss	%%xmm3, %[o3]\n"	\
		"movss	%%xmm4, %[o4]\n"	\
		"movss	%%xmm5, %[o5]\n"	\
		"movss	%%xmm6, %[o6]\n"	\
		"movss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7",\
		"%ymm8","%ymm9","%ymm10","%ymm11","%ymm12","%ymm13","%ymm14","%ymm15"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
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

#define AVX_M_FMA_IR12(op1,op2,name) AVX_M_FMA_IR12_0(#op1,#op2,name)




//=============================================================================
// AVX512
//=============================================================================

#if ENABLE_AVX512


#define AVX512_REG_CLEAR() \
		"vxorps	%%zmm0, %%zmm0, %%zmm0\n"	\
		"vxorps	%%zmm1, %%zmm1, %%zmm1\n"	\
		"vxorps	%%zmm2, %%zmm2, %%zmm2\n"	\
		"vxorps	%%zmm3, %%zmm3, %%zmm3\n"	\
		"vxorps	%%zmm4, %%zmm4, %%zmm4\n"	\
		"vxorps	%%zmm5, %%zmm5, %%zmm5\n"	\
		"vxorps	%%zmm6, %%zmm6, %%zmm6\n"	\
		"vxorps	%%zmm7, %%zmm7, %%zmm7\n"	\
		"vxorps	%%zmm8, %%zmm8, %%zmm8\n"	\
		"vxorps	%%zmm9, %%zmm9, %%zmm9\n"	\
		"vxorps	%%zmm10, %%zmm10, %%zmm10\n"	\
		"vxorps	%%zmm11, %%zmm11, %%zmm11\n"	\
		"vxorps	%%zmm12, %%zmm12, %%zmm12\n"	\
		"vxorps	%%zmm13, %%zmm13, %%zmm13\n"	\
		"vxorps	%%zmm14, %%zmm14, %%zmm14\n"	\
		"vxorps	%%zmm15, %%zmm15, %%zmm15\n"	\
		"vxorps	%%zmm16, %%zmm16, %%zmm16\n"	\
		"vxorps	%%zmm17, %%zmm17, %%zmm17\n"	\
		"vxorps	%%zmm18, %%zmm18, %%zmm18\n"	\
		"vxorps	%%zmm19, %%zmm19, %%zmm19\n"	\
		"vxorps	%%zmm20, %%zmm20, %%zmm20\n"	\
		"vxorps	%%zmm21, %%zmm21, %%zmm21\n"	\
		"vxorps	%%zmm22, %%zmm22, %%zmm22\n"	\
		"vxorps	%%zmm23, %%zmm23, %%zmm23\n"	\
		"vxorps	%%zmm24, %%zmm24, %%zmm24\n"	\
		"vxorps	%%zmm25, %%zmm25, %%zmm25\n"	\
		"vxorps	%%zmm26, %%zmm26, %%zmm26\n"	\
		"vxorps	%%zmm27, %%zmm27, %%zmm27\n"	\
		"vxorps	%%zmm28, %%zmm28, %%zmm28\n"	\
		"vxorps	%%zmm29, %%zmm29, %%zmm29\n"	\
		"vxorps	%%zmm30, %%zmm30, %%zmm30\n"	\
		"vxorps	%%zmm31, %%zmm31, %%zmm31\n"	\


//-----------------------------------------------------------------------------
// Single Interleave 12
//-----------------------------------------------------------------------------

#define AVX512_S_IR12_12(op) \
			op " %%zmm13, %%zmm12, %%zmm0\n"		\
			op " %%zmm12, %%zmm13, %%zmm1\n"		\
			op " %%zmm13, %%zmm12, %%zmm2\n"		\
			op " %%zmm12, %%zmm13, %%zmm3\n"		\
			op " %%zmm13, %%zmm12, %%zmm4\n"		\
			op " %%zmm12, %%zmm13, %%zmm5\n"		\
			op " %%zmm13, %%zmm12, %%zmm6\n"		\
			op " %%zmm12, %%zmm13, %%zmm7\n"		\
			op " %%zmm13, %%zmm12, %%zmm8\n"		\
			op " %%zmm12, %%zmm13, %%zmm9\n"		\
			op " %%zmm13, %%zmm12, %%zmm10\n"		\
			op " %%zmm12, %%zmm13, %%zmm11\n"		\



#define AVX512_S_IR12_0(op,name) \
static uint64_t AVX512_S_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
		"vmovss	%%xmm4, %[o4]\n"	\
		"vmovss	%%xmm5, %[o5]\n"	\
		"vmovss	%%xmm6, %[o6]\n"	\
		"vmovss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%zmm0","%zmm1","%zmm2","%zmm3","%zmm4","%zmm5","%zmm6","%zmm7",\
		"%zmm8","%zmm9","%zmm10","%zmm11","%zmm12","%zmm13","%zmm14","%zmm15",\
		"%zmm16","%zmm17","%zmm18","%zmm19","%zmm20","%zmm21","%zmm22","%zmm23",\
		"%zmm24","%zmm25","%zmm26","%zmm27","%zmm28","%zmm29","%zmm30","%zmm31",\
		"%k1","%k2","%k3","%k4","%k5","%k6","%k7"\
		); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	/*check_result( ret1, answer );*/	\
	/*check_result( ret2, answer );*/	\
	/*check_result( ret3, answer );*/	\
	/*check_result( ret4, answer );*/	\
	/*check_result( ret5, answer );*/	\
	/*check_result( ret6, answer );*/	\
	/*check_result( ret7, answer );*/	\
	return	timer.Result();	\
}

#define AVX512_S_IR12(op,name) AVX512_S_IR12_0(#op,name)



//-----------------------------------------------------------------------------
// Single Interleave 12 MAD
//-----------------------------------------------------------------------------

#define AVX512_M_IR12_12(op1,op2) \
			op1 " %%zmm13, %%zmm12, %%zmm0\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm1\n"		\
			op1 " %%zmm13, %%zmm12, %%zmm2\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm3\n"		\
			op1 " %%zmm13, %%zmm12, %%zmm4\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm5\n"		\
			op1 " %%zmm13, %%zmm12, %%zmm6\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm7\n"		\
			op1 " %%zmm13, %%zmm12, %%zmm8\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm9\n"		\
			op1 " %%zmm13, %%zmm12, %%zmm10\n"		\
			op2 " %%zmm13, %%zmm12, %%zmm11\n"		\


#define AVX512_M_IR12_0(op1,op2,name) \
static uint64_t AVX512_M_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
		"vmovss	%%xmm4, %[o4]\n"	\
		"vmovss	%%xmm5, %[o5]\n"	\
		"vmovss	%%xmm6, %[o6]\n"	\
		"vmovss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%zmm0","%zmm1","%zmm2","%zmm3","%zmm4","%zmm5","%zmm6","%zmm7",\
		"%zmm8","%zmm9","%zmm10","%zmm11","%zmm12","%zmm13","%zmm14","%zmm15",\
		"%zmm16","%zmm17","%zmm18","%zmm19","%zmm20","%zmm21","%zmm22","%zmm23",\
		"%zmm24","%zmm25","%zmm26","%zmm27","%zmm28","%zmm29","%zmm30","%zmm31",\
		"%k1","%k2","%k3","%k4","%k5","%k6","%k7"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
	/*check_result( ret0, answer );*/	\
	/*check_result( ret1, answer );*/	\
	/*check_result( ret2, answer );*/	\
	/*check_result( ret3, answer );*/	\
	/*check_result( ret4, answer );*/	\
	/*check_result( ret5, answer );*/	\
	/*check_result( ret6, answer );*/	\
	/*check_result( ret7, answer );*/	\
	return	timer.Result();	\
}

#define AVX512_M_IR12(op1,op2,name) AVX512_M_IR12_0(#op1,#op2,name)



//-----------------------------------------------------------------------------
// Single Interleave 12 AVX512 + ymm256
//-----------------------------------------------------------------------------

#define AVX512_YMM_S_IR12_12(op) \
			op " %%ymm13, %%ymm12, %%ymm0%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm1%{%%k1%}%{z%}\n"		\
			op " %%ymm13, %%ymm12, %%ymm2%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm3%{%%k1%}%{z%}\n"		\
			op " %%ymm13, %%ymm12, %%ymm4%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm5%{%%k1%}%{z%}\n"		\
			op " %%ymm13, %%ymm12, %%ymm6%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm7%{%%k1%}%{z%}\n"		\
			op " %%ymm13, %%ymm12, %%ymm8%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm9%{%%k1%}%{z%}\n"		\
			op " %%ymm13, %%ymm12, %%ymm10%{%%k1%}%{z%}\n"		\
			op " %%ymm12, %%ymm13, %%ymm11%{%%k1%}%{z%}\n"		\



#define AVX512_YMM_S_IR12_0(op,name) \
static uint64_t AVX512_YMM_S_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$0xffffffff, %%eax\n"	\
		"kmovw		%%eax, %%k1\n"	\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_YMM_S_IR12_12( op )	\
		AVX512_YMM_S_IR12_12( op )	\
		AVX512_YMM_S_IR12_12( op )	\
		AVX512_YMM_S_IR12_12( op )	\
		AVX512_YMM_S_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
		"vmovss	%%xmm4, %[o4]\n"	\
		"vmovss	%%xmm5, %[o5]\n"	\
		"vmovss	%%xmm6, %[o6]\n"	\
		"vmovss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%zmm0","%zmm1","%zmm2","%zmm3","%zmm4","%zmm5","%zmm6","%zmm7",\
		"%zmm8","%zmm9","%zmm10","%zmm11","%zmm12","%zmm13","%zmm14","%zmm15",\
		"%zmm16","%zmm17","%zmm18","%zmm19","%zmm20","%zmm21","%zmm22","%zmm23",\
		"%zmm24","%zmm25","%zmm26","%zmm27","%zmm28","%zmm29","%zmm30","%zmm31",\
		"%k1","%k2","%k3","%k4","%k5","%k6","%k7"\
		); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	/*check_result( ret1, answer );*/	\
	/*check_result( ret2, answer );*/	\
	/*check_result( ret3, answer );*/	\
	/*check_result( ret4, answer );*/	\
	/*check_result( ret5, answer );*/	\
	/*check_result( ret6, answer );*/	\
	/*check_result( ret7, answer );*/	\
	return	timer.Result();	\
}

#define AVX512_YMM_S_IR12(op,name) AVX512_YMM_S_IR12_0(#op,name)





//-----------------------------------------------------------------------------
// Single Interleave 12 MAD AVX512 + ymm256
//-----------------------------------------------------------------------------

#define AVX512_YMM_M_IR12_12(op1,op2) \
			op1 " %%ymm13, %%ymm12, %%ymm0%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm1%{%%k1%}%{z%}\n"		\
			op1 " %%ymm13, %%ymm12, %%ymm2%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm3%{%%k1%}%{z%}\n"		\
			op1 " %%ymm13, %%ymm12, %%ymm4%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm5%{%%k1%}%{z%}\n"		\
			op1 " %%ymm13, %%ymm12, %%ymm6%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm7%{%%k1%}%{z%}\n"		\
			op1 " %%ymm13, %%ymm12, %%ymm8%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm9%{%%k1%}%{z%}\n"		\
			op1 " %%ymm13, %%ymm12, %%ymm10%{%%k1%}%{z%}\n"		\
			op2 " %%ymm13, %%ymm12, %%ymm11%{%%k1%}%{z%}\n"		\


#define AVX512_YMM_M_IR12_0(op1,op2,name) \
static uint64_t AVX512_YMM_M_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$0xffffffff, %%eax\n"	\
		"kmovw		%%eax, %%k1\n"	\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufps	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastss	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_YMM_M_IR12_12( op1, op2)	\
		AVX512_YMM_M_IR12_12( op1, op2)	\
		AVX512_YMM_M_IR12_12( op1, op2)	\
		AVX512_YMM_M_IR12_12( op1, op2)	\
		AVX512_YMM_M_IR12_12( op1, op2)	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
		"vmovss	%%xmm4, %[o4]\n"	\
		"vmovss	%%xmm5, %[o5]\n"	\
		"vmovss	%%xmm6, %[o6]\n"	\
		"vmovss	%%xmm7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%rax",	\
		"%zmm0","%zmm1","%zmm2","%zmm3","%zmm4","%zmm5","%zmm6","%zmm7",\
		"%zmm8","%zmm9","%zmm10","%zmm11","%zmm12","%zmm13","%zmm14","%zmm15",\
		"%zmm16","%zmm17","%zmm18","%zmm19","%zmm20","%zmm21","%zmm22","%zmm23",\
		"%zmm24","%zmm25","%zmm26","%zmm27","%zmm28","%zmm29","%zmm30","%zmm31",\
		"%k1","%k2","%k3","%k4","%k5","%k6","%k7"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
	/*check_result( ret0, answer );*/	\
	/*check_result( ret1, answer );*/	\
	/*check_result( ret2, answer );*/	\
	/*check_result( ret3, answer );*/	\
	/*check_result( ret4, answer );*/	\
	/*check_result( ret5, answer );*/	\
	/*check_result( ret6, answer );*/	\
	/*check_result( ret7, answer );*/	\
	return	timer.Result();	\
}

#define AVX512_YMM_M_IR12(op1,op2,name) AVX512_YMM_M_IR12_0(#op1,#op2,name)







//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



static const char*	Instruction_Title[]= {

	"SSE mulss (32bit x1) n8",
	"SSE addss (32bit x1) n8",
	"FMA vfmaddss (32bit x1) n8",
	"FMA vfmaddss (32bit x1) n12",
	"FMA vfma+mlss (32bit x1) n12",
	"FMA vfma+adss (32bit x1) n12",

	"SSE mulps (32bit x4) n8",
	"SSE addps (32bit x4) n8",
	"SSE mul+addps (32bit x4) n8",
	"FMA vfmaddps (32bit x4) n8",
	"FMA vfmaddps (32bit x4) n12",
	"FMA vfma+mlps (32bit x4) n12",
	"FMA vfma+adps (32bit x4) n12",

	"SSE ml+ad+adps (32bit x4) n9",


	"SSE mulss (32bit x1) ns4",
	"SSE addss (32bit x1) ns4",

	"SSE mulps (32bit x4) ns4",
	"SSE addps (32bit x4) ns4",



	"AVX vmulps (32bit x8) n8",
	"AVX vaddps (32bit x8) n8",
	"AVX vmul+addps (32bit x8) n8",
	"FMA vfmaddps (32bit x8) n8",
	"FMA vfmaddps (32bit x8) n12",
	"FMA vfma+mlps (32bit x8) n12",
	"FMA vfma+adps (32bit x8) n12",

	"AVX vml+ad+adps (32bit x8) n9",
//	"FMA vfma+ml+adps (32bit x8) n9",

	"AVX512 vmulps (32bit x16) n12",
	"AVX512 vaddps (32bit x16) n12",
	"AVX512 vfmaddps (32bit x16) n12",
	"AVX512 vfma+mps (32bit x16) n12",
	"AVX512 vfma+aps (32bit x16) n12",

	"AVX512 vmulps (32bit x8) n12",
	"AVX512 vaddps (32bit x8) n12",
	"AVX512 vfmaddps (32bit x8) n12",
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if USE_ASM_DEBUG_TEST
static void asm_debug_test()
{
	// 132
	// 213
	// 231
	float	va= 2.0f;
	float	vb= 3.0f;
	float	vc= 4.0f;
	__asm__ __volatile__(
		"xorps %%xmm0, %%xmm0\n"
		"xorps %%xmm1, %%xmm1\n"
		"xorps %%xmm2, %%xmm2\n"
		"movss %[a], %%xmm0\n"
		"movss %[b], %%xmm1\n"
		"movss %[c], %%xmm2\n"
		"vfmadd132ss  %%xmm0,%%xmm1,%%xmm2\n"
		"movss %%xmm0, %[a]\n"
		"movss %%xmm1, %[b]\n"
		"movss %%xmm2, %[c]\n"
		:
			[a]"=g"(va),
			[b]"=g"(vb),
			[c]"=g"(vc)
		:
			"g"(va),
			"g"(vb),
			"g"(vc)
		:
			"cc",
			"%xmm0",
			"%xmm1",
			"%xmm2"
	);
	FL_PRINT( "va,vb,vc=%f %f %f\n", va, vb, vc );
	// vfmadd132( 2 3 4 )= xmm2=xmm0*xmm2+xmm1 = 2 3 11
	// vfmadd213( 2 3 4 )= xmm2=xmm1*xmm2+xmm0 = 2 3 14
	// vfmadd231( 2 3 4 )= xmm2=xmm0*xmm1+xmm2 = 2 3 10
}
#endif


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_SSE_MULSS_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_ADDSS_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_FMADDSS_IR8,		PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_FMADDSS_IR12,		PER_LOOP_INST_12 * 2, 2	);
	SetOp2f( RESULT_SSE_FMA_MULSS_IR12,	PER_LOOP_INST_12, (2+1)/2.0f	);
	SetOp2f( RESULT_SSE_FMA_ADDSS_IR12,	PER_LOOP_INST_12, (2+1)/2.0f	);

	SetOp( RESULT_SSE_MULPS_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_ADDPS_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_MULPS_ADDPS_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_FMADDPS_IR8,		PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_SSE_FMADDPS_IR12,		PER_LOOP_INST_12 * 8, 8	);
	SetOp2f( RESULT_SSE_FMA_MULPS_IR12,	PER_LOOP_INST_12, (8+4)/2.0f	);
	SetOp2f( RESULT_SSE_FMA_ADDPS_IR12,	PER_LOOP_INST_12, (8+4)/2.0f	);

	SetOp( RESULT_SSE_ML_AD_ADDPS_IR9,	PER_LOOP_INST_9 * 4, 4	);


	SetOp( RESULT_SSE_MULSS_IRS4,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_ADDSS_IRS4,		PER_LOOP_INST * 1, 1	);

	SetOp( RESULT_SSE_MULPS_IRS4,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_ADDPS_IRS4,		PER_LOOP_INST * 4, 4	);



	SetOp( RESULT_AVX_VMULPS_IR8,			PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VADDPS_IR8,			PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VMULPS_VADDPS_IR8,	PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VFMADDPS_IR8,			PER_LOOP_INST * 16, 16	);
	SetOp( RESULT_AVX_VFMADDPS_IR12,		PER_LOOP_INST_12 * 16, 16	);
	SetOp2f( RESULT_AVX_FMA_MUL_IR12,		PER_LOOP_INST_12, (16+8)/2.0f	);
	SetOp2f( RESULT_AVX_FMA_ADD_IR12,		PER_LOOP_INST_12, (16+8)/2.0f	);

	SetOp( RESULT_AVX_VML_AD_VADDPS_IR9,	PER_LOOP_INST_9 * 8, 8	);
#if 0
	SetOp2f( RESULT_AVX_VFMA_ML_VADDPS_IR9,	PER_LOOP_INST_9, (16+8+8)/3.0f	);
#endif


	SetOp2( RESULT_AVX512_VMULPS_IR12,		PER_LOOP_INST_12, 16	);
	SetOp2( RESULT_AVX512_VADDPS_IR12,		PER_LOOP_INST_12, 16	);
	SetOp2( RESULT_AVX512_VFMADDPS_IR12,	PER_LOOP_INST_12, 32	);
	SetOp2f( RESULT_AVX512_FMA_MUL_IR12,	PER_LOOP_INST_12, (32+16)/2.0f	);
	SetOp2f( RESULT_AVX512_FMA_ADD_IR12,	PER_LOOP_INST_12, (32+16)/2.0f	);

	SetOp2( RESULT_AVX512_YMM_VMULPS_IR12,		PER_LOOP_INST_12, 8	);
	SetOp2( RESULT_AVX512_YMM_VADDPS_IR12,		PER_LOOP_INST_12, 8	);
	SetOp2( RESULT_AVX512_YMM_VFMADDPS_IR12,	PER_LOOP_INST_12, 16	);
}







//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



SSE_S_IR8( mulss,  mulss_ir8 );
SSE_S_IR8( addss,  addss_ir8 );
SSE_S_FMA_IR8( vfmadd231ss,  fmaddss_ir8 );			// FMA3
SSE_S_FMA_IR12( vfmadd231ss,  fmaddss_ir12 );		// FMA3
#if 0
SSE_M_FMA_IR12( vfmadd231ss,  vmulss,	fmaddss_mulss_ir12 );		// FMA3
SSE_M_FMA_IR12( vfmadd231ss,  vaddss,	fmaddss_addss_ir12 );		// FMA3
#else
SSE_M_FMA_IR12( vfmadd231ss,  mulss,	fmaddss_mulss_ir12 );		// FMA3
SSE_M_FMA_IR12( vfmadd231ss,  addss,	fmaddss_addss_ir12 );		// FMA3
#endif

SSE_S_IR8( mulps,  mulps_ir8 );
SSE_S_IR8( addps,  addps_ir8 );
SSE_M_IR8( mulps, addps,  mulps_addps_ir8 );
SSE_S_FMA_IR8( vfmadd231ps,  fmaddps_ir8 );			// FMA3
SSE_S_FMA_IR12( vfmadd231ps,  fmaddps_ir12 );		// FMA3
#if 0
SSE_M_FMA_IR12( vfmadd231ps,  vmulps,	fmaddps_mulps_ir12 );		// FMA3
SSE_M_FMA_IR12( vfmadd231ps,  vaddps,	fmaddps_addps_ir12 );		// FMA3
#else
SSE_M_FMA_IR12( vfmadd231ps,  mulps,	fmaddps_mulps_ir12 );		// FMA3
SSE_M_FMA_IR12( vfmadd231ps,  addps,	fmaddps_addps_ir12 );		// FMA3
#endif

SSE_M_IR9( mulps, addps, addps,  ml_ad_addps_ir9 );


SSE_S_IRS4( mulss,  mulss_irs4 );
SSE_S_IRS4( addss,  addss_irs4 );

SSE_S_IRS4( mulps,  mulps_irs4 );
SSE_S_IRS4( addps,  addps_irs4 );



AVX_S_IR8( vmulps,  vmulps_ir8 );
AVX_S_IR8( vaddps,  vaddps_ir8 );
AVX_M_IR8( vmulps, vaddps,  vmulps_vaddps_ir8 );
AVX_S_FMA_IR8( vfmadd231ps,  vfmaddps_ir8 );			// FMA3
AVX_S_FMA_IR12( vfmadd231ps,  vfmaddps_ir12 );			// FMA3
AVX_M_FMA_IR12( vfmadd231ps,  vmulps,	vfmaddps_vmulps_ir12 );			// FMA3
AVX_M_FMA_IR12( vfmadd231ps,  vaddps,	vfmaddps_vaddps_ir12 );			// FMA3

AVX_M_IR9( vmulps, vaddps, vaddps, vml_ad_vaddps_ir9 );

#if 0
AVX_M_IR9( vfmadd231ps, vmulps, vaddps, vfma_ml_vaddps_ir9 );		// FMA3
#endif

#if ENABLE_AVX512
AVX512_S_IR12( vmulps,  vmulps_ir12 );
AVX512_S_IR12( vaddps,  vaddps_ir12 );
AVX512_M_IR12( vfmadd231ps,  vfmadd231ps,	vfmaddps_ir12 );
AVX512_M_IR12( vfmadd231ps,  vmulps,		vfmaddps_vmulps_ir12 );
AVX512_M_IR12( vfmadd231ps,  vaddps,		vfmaddps_vaddps_ir12 );

AVX512_YMM_S_IR12( vmulps,  vmulps_ir12 );
AVX512_YMM_S_IR12( vaddps,  vaddps_ir12 );
AVX512_YMM_M_IR12( vfmadd231ps,  vfmadd231ps,	vfmaddps_ir12 );
#endif



void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "SSE loop=%d\n", Loop );

	float	sum= 0;
	float	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 2.0f;
		sum_2+= 10.0f;
	}

#if USE_ASM_DEBUG_TEST
asm_debug_test();
DoneFlag= true;
return;
#endif

	//------------------------------------------------------
	// IR8
	//------------------------------------------------------

	SetResult( RESULT_SSE_MULSS_IR8,	SSE_S_IR8_mulss_ir8( Loop, 10.0f,	0.9999f, 0.9998f	) );
	Progress++;

	SetResult( RESULT_SSE_ADDSS_IR8,	SSE_S_IR8_addss_ir8( Loop, 10.0f,  1e-35f, 1e-35f		) );
	Progress++;

	if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

		SetResult( RESULT_SSE_FMADDSS_IR8,	SSE_S_FMA_IR8_fmaddss_ir8( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMADDSS_IR12,	SSE_S_FMA_IR12_fmaddss_ir12( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMA_MULSS_IR12,	SSE_M_FMA_IR12_fmaddss_mulss_ir12( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMA_ADDSS_IR12,	SSE_M_FMA_IR12_fmaddss_addss_ir12( Loop, 10.0f		) );
		Progress++;

	}else{
		Progress++;
		Progress++;
		Progress++;
		Progress++;
	}


	SetResult( RESULT_SSE_MULPS_IR8,	SSE_S_IR8_mulps_ir8( Loop, 10.0f,	0.99999f, 0.99998f		) );
	Progress++;

	SetResult( RESULT_SSE_ADDPS_IR8,	SSE_S_IR8_addps_ir8( Loop, 10.0f,	1e-37f, 1e-37f		) );
	Progress++;


	SetResult( RESULT_SSE_MULPS_ADDPS_IR8,	SSE_M_IR8_mulps_addps_ir8( Loop, 10.0f		) );
	Progress++;


	if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

		SetResult( RESULT_SSE_FMADDPS_IR8,	SSE_S_FMA_IR8_fmaddps_ir8( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMADDPS_IR12,	SSE_S_FMA_IR12_fmaddps_ir12( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMA_MULPS_IR12,	SSE_M_FMA_IR12_fmaddps_mulps_ir12( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_SSE_FMA_ADDPS_IR12,	SSE_M_FMA_IR12_fmaddps_addps_ir12( Loop, 10.0f		) );
		Progress++;

	}else{
		Progress++;
		Progress++;
		Progress++;
		Progress++;
	}

	//------------------------------------------------------
	// IR9
	//------------------------------------------------------

	SetResult( RESULT_SSE_ML_AD_ADDPS_IR9,	SSE_M_IR9_ml_ad_addps_ir9( Loop, 10.0f		) );
	Progress++;


	//------------------------------------------------------
	// IRS4
	//------------------------------------------------------

	SetResult( RESULT_SSE_MULSS_IRS4,	SSE_S_IRS4_mulss_irs4( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_SSE_ADDSS_IRS4,	SSE_S_IRS4_addss_irs4( Loop, 10.0f		) );
	Progress++;



	SetResult( RESULT_SSE_MULPS_IRS4,	SSE_S_IRS4_mulps_irs4( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_SSE_ADDPS_IRS4,	SSE_S_IRS4_addps_irs4( Loop, 10.0f		) );
	Progress++;



	//------------------------------------------------------
	//------------------------------------------------------




	//------------------------------------------------------
	// AVX
	//------------------------------------------------------

	if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){

		//------------------------------------------------------
		// IR8
		//------------------------------------------------------

		SetResult( RESULT_AVX_VMULPS_IR8,	AVX_S_IR8_vmulps_ir8( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_AVX_VADDPS_IR8,	AVX_S_IR8_vaddps_ir8( Loop, 7.0f		) );
		Progress++;



		SetResult( RESULT_AVX_VMULPS_VADDPS_IR8,	AVX_M_IR8_vmulps_vaddps_ir8( Loop, 7.0f		) );
		Progress++;


		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

			SetResult( RESULT_AVX_VFMADDPS_IR8,	AVX_S_FMA_IR8_vfmaddps_ir8( Loop, 10.0f		) );
			Progress++;

			SetResult( RESULT_AVX_VFMADDPS_IR12,	AVX_S_FMA_IR12_vfmaddps_ir12( Loop, 10.0f		) );
			Progress++;

			SetResult( RESULT_AVX_FMA_MUL_IR12,		AVX_M_FMA_IR12_vfmaddps_vmulps_ir12( Loop, 10.0f		) );
			Progress++;

			SetResult( RESULT_AVX_FMA_ADD_IR12,		AVX_M_FMA_IR12_vfmaddps_vaddps_ir12( Loop, 10.0f		) );
			Progress++;

		}else{
			Progress++;
			Progress++;
			Progress++;
			Progress++;
		}

		//------------------------------------------------------
		// IR9
		//------------------------------------------------------

		SetResult( RESULT_AVX_VML_AD_VADDPS_IR9,	AVX_M_IR9_vml_ad_vaddps_ir9( Loop, 7.0f		) );
		Progress++;

#if 0
		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){
			SetResult( RESULT_AVX_VFMA_ML_VADDPS_IR9,	AVX_M_IR9_vfma_ml_vaddps_ir9( Loop, 7.0f		) );
			Progress++;
		}else{
			Progress++;
		}
#endif

	}else{
		Progress+= 8;
	}

	//------------------------------------------------------
	// AVX512
	//------------------------------------------------------

#if ENABLE_AVX512
	if( Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ){

		SetResult( RESULT_AVX512_VMULPS_IR12,	AVX512_S_IR12_vmulps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_VADDPS_IR12,	AVX512_S_IR12_vaddps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_VFMADDPS_IR12,	AVX512_M_IR12_vfmaddps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_FMA_MUL_IR12,	AVX512_M_IR12_vfmaddps_vmulps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_FMA_ADD_IR12,	AVX512_M_IR12_vfmaddps_vaddps_ir12( Loop, 0.0f		) );
		Progress++;



		SetResult( RESULT_AVX512_YMM_VMULPS_IR12,	AVX512_YMM_S_IR12_vmulps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_YMM_VADDPS_IR12,	AVX512_YMM_S_IR12_vaddps_ir12( Loop, 0.0f		) );
		Progress++;

		SetResult( RESULT_AVX512_YMM_VFMADDPS_IR12,	AVX512_YMM_M_IR12_vfmaddps_ir12( Loop, 0.0f		) );
		Progress++;

	}else
#endif
	{
		Progress+= 8;
	}

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
		return	"SSE/AVX (SP fp)";
	}else{
		return	"SSE/AVX (SP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // x64
//-----------------------------------------------------------------------------

