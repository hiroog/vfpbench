// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE64SP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_X64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace SSE64SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	xmm0-xmm7
*/



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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



static const char*	Instruction_Title[]= {

	"SSE mulss (32bit x1) n8",
	"SSE addss (32bit x1) n8",
	"FMA vfmaddss (32bit x1) n8",

	"SSE mulps (32bit x4) n8",
	"SSE addps (32bit x4) n8",
	"SSE mul+addps (32bit x4) n8",
	"FMA vfmaddss (32bit x4) n8",

	"SSE ml+ad+addps (32bit x4) n6",



	"SSE mulss (32bit x1) ns4",
	"SSE addss (32bit x1) ns4",

	"SSE mulps (32bit x4) ns4",
	"SSE addps (32bit x4) ns4",


	"AVX vmulps (32bit x8) n8",
	"AVX vaddps (32bit x8) n8",
	"AVX vmul+addps (32bit x8) n8",
	"FMA vfmaddps (32bit x8) n8",
	"FMA vfmaddps (32bit x8) n12",

	"AVX vml+ad+adps (32bit x8) n6",

};




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

	SetOp( RESULT_SSE_MULPS_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_ADDPS_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_MULPS_ADDPS_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_FMADDPS_IR8,		PER_LOOP_INST * 8, 8	);

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

	SetOp( RESULT_AVX_VML_AD_VADDPS_IR9,	PER_LOOP_INST_9 * 8, 8	);



}








//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



SSE_S_IR8( mulss,  mulss_ir8 );
SSE_S_IR8( addss,  addss_ir8 );
SSE_S_FMA_IR8( vfmadd213ss,  fmaddss_ir8 );			// FMA3

SSE_S_IR8( mulps,  mulps_ir8 );
SSE_S_IR8( addps,  addps_ir8 );
SSE_M_IR8( mulps, addps,  mulps_addps_ir8 );
SSE_S_FMA_IR8( vfmadd213ps,  fmaddps_ir8 );			// FMA3

SSE_M_IR9( mulps, addps, addps,  ml_ad_addps_ir9 );




SSE_S_IRS4( mulss,  mulss_irs4 );
SSE_S_IRS4( addss,  addss_irs4 );

SSE_S_IRS4( mulps,  mulps_irs4 );
SSE_S_IRS4( addps,  addps_irs4 );



AVX_S_IR8( vmulps,  vmulps_ir8 );
AVX_S_IR8( vaddps,  vaddps_ir8 );
AVX_M_IR8( vmulps, vaddps,  vmulps_vaddps_ir8 );
AVX_S_FMA_IR8( vfmadd213ps,  vfmaddps_ir8 );			// FMA3
AVX_S_FMA_IR12( vfmadd213ps,  vfmaddps_ir12 );			// FMA3

AVX_M_IR9( vmulps, vaddps, vaddps, vml_ad_vaddps_ir9 );




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

	}else{
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

	}else{
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

		}else{
			Progress++;
			Progress++;
		}

		//------------------------------------------------------
		// IR9
		//------------------------------------------------------

		SetResult( RESULT_AVX_VML_AD_VADDPS_IR9,	AVX_M_IR9_vml_ad_vaddps_ir9( Loop, 7.0f		) );
		Progress++;

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

