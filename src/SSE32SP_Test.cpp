// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE32SP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_X86
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace SSE32SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	xmm0-xmm7
*/



typedef	uint32_t	CounterType;


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
static uint64_t SSE_S_IR8_##name( CounterType LoopCount, float answer ) \
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
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		"dec	%%eax\n"	\
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
	: "cc","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7" ); \
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
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		"dec	%%eax\n"	\
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
	: "cc","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7" ); \
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
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		"dec	%%eax\n"	\
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
	: "cc","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7" ); \
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
// Single Interleave 6 MAD 3
//-----------------------------------------------------------------------------

#define SSE_M_IR6_6(op1,op2,op3) \
			op1 " %%xmm0, %%xmm0\n"		\
			op2 " %%xmm1, %%xmm1\n"		\
			op3 " %%xmm2, %%xmm2\n"		\
			op1 " %%xmm3, %%xmm3\n"		\
			op2 " %%xmm4, %%xmm4\n"		\
			op3 " %%xmm5, %%xmm5\n"		\



#define SSE_M_IR6_0(op1,op2,op3,name) \
static uint64_t SSE_M_IR6_##name( CounterType LoopCount, float answer ) \
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
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		SSE_M_IR6_6( op1,op2,op3 )	\
		"dec	%%eax\n"	\
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
	: "cc","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7" ); \
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

#define SSE_M_IR6(op1,op2,op3,name) SSE_M_IR6_0(#op1,#op2,#op3,name)





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
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		"dec	%%eax\n"	\
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
	: "cc","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7" ); \
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



//-----------------------------------------------------------------------------
// Single Interleave 4
//-----------------------------------------------------------------------------

#define AVX_S_IR4_8(op) \
			op " %%ymm5, %%ymm4, %%ymm0\n"		\
			op " %%ymm5, %%ymm4, %%ymm1\n"		\
			op " %%ymm5, %%ymm4, %%ymm2\n"		\
			op " %%ymm5, %%ymm4, %%ymm3\n"		\
			op " %%ymm5, %%ymm4, %%ymm0\n"		\
			op " %%ymm5, %%ymm4, %%ymm1\n"		\
			op " %%ymm5, %%ymm4, %%ymm2\n"		\
			op " %%ymm5, %%ymm4, %%ymm3\n"



#define AVX_S_IR4_0(op,name) \
static uint64_t AVX_S_IR4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm4, %%xmm4\n"	\
		"vshufps	$0, %%xmm4, %%xmm4, %%xmm4\n"	\
		"vperm2f128	$0, %%ymm4, %%ymm4, %%ymm4\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm5, %%xmm5\n"	\
		"vshufps	$0, %%xmm5, %%xmm5, %%xmm5\n"	\
		"vperm2f128	$0, %%ymm5, %%ymm5, %%ymm5\n"	\
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		AVX_S_IR4_8( op )	\
		AVX_S_IR4_8( op )	\
		AVX_S_IR4_8( op )	\
		AVX_S_IR4_8( op )	\
		AVX_S_IR4_8( op )	\
		"dec	%%eax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%eax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7"\
		); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );	\
	return	timer.Result();	\
}

#define AVX_S_IR4(op,name) AVX_S_IR4_0(#op,name)



//-----------------------------------------------------------------------------
// Single Interleave 4 MAD
//-----------------------------------------------------------------------------

#define AVX_M_IR4_8(op1,op2) \
			op1 " %%ymm5, %%ymm4, %%ymm0\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm1\n"		\
			op1 " %%ymm5, %%ymm4, %%ymm2\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm3\n"		\
			op1 " %%ymm5, %%ymm4, %%ymm0\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm1\n"		\
			op1 " %%ymm5, %%ymm4, %%ymm2\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm3\n"



#define AVX_M_IR4_0(op1,op2,name) \
static uint64_t AVX_M_IR4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm4, %%xmm4\n"	\
		"vshufps	$0, %%xmm4, %%xmm4, %%xmm4\n"	\
		"vperm2f128	$0, %%ymm4, %%ymm4, %%ymm4\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm5, %%xmm5\n"	\
		"vshufps	$0, %%xmm5, %%xmm5, %%xmm5\n"	\
		"vperm2f128	$0, %%ymm5, %%ymm5, %%ymm5\n"	\
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		AVX_M_IR4_8( op1, op2 )	\
		AVX_M_IR4_8( op1, op2 )	\
		AVX_M_IR4_8( op1, op2 )	\
		AVX_M_IR4_8( op1, op2 )	\
		AVX_M_IR4_8( op1, op2 )	\
		"dec	%%eax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%eax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
	/*check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );*/	\
	return	timer.Result();	\
}

#define AVX_M_IR4(op1,op2,name) AVX_M_IR4_0(#op1,#op2,name)



//-----------------------------------------------------------------------------
// Single Interleave 6 MAD OP3
//-----------------------------------------------------------------------------

#define AVX_M_IR6_6(op1,op2,op3) \
			op1 " %%ymm5, %%ymm4, %%ymm0\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm1\n"		\
			op3 " %%ymm5, %%ymm4, %%ymm2\n"		\
			op1 " %%ymm5, %%ymm4, %%ymm3\n"		\
			op2 " %%ymm5, %%ymm4, %%ymm6\n"		\
			op3 " %%ymm5, %%ymm4, %%ymm7\n"		\



#define AVX_M_IR6_0(op1,op2,op3,name) \
static uint64_t AVX_M_IR6_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm4, %%xmm4\n"	\
		"vshufps	$0, %%xmm4, %%xmm4, %%xmm4\n"	\
		"vperm2f128	$0, %%ymm4, %%ymm4, %%ymm4\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm5, %%xmm5\n"	\
		"vshufps	$0, %%xmm5, %%xmm5, %%xmm5\n"	\
		"vperm2f128	$0, %%ymm5, %%ymm5, %%ymm5\n"	\
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		AVX_M_IR6_6( op1, op2, op3 )	\
		"dec	%%eax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%eax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7"\
		); \
\
	timer.End();		\
	timer.Dump( op1 );	\
	/*check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );*/	\
	return	timer.Result();	\
}

#define AVX_M_IR6(op1,op2,op3,name) AVX_M_IR6_0(#op1,#op2,#op3,name)



//-----------------------------------------------------------------------------
// Single Interleave 4 FMA
//-----------------------------------------------------------------------------

#define AVX_S_FMA_IR4_8(op) \
			op " %%ymm5, %%ymm4, %%ymm0\n"		\
			op " %%ymm5, %%ymm4, %%ymm1\n"		\
			op " %%ymm5, %%ymm4, %%ymm2\n"		\
			op " %%ymm5, %%ymm4, %%ymm3\n"		\
			op " %%ymm5, %%ymm4, %%ymm0\n"		\
			op " %%ymm5, %%ymm4, %%ymm1\n"		\
			op " %%ymm5, %%ymm4, %%ymm2\n"		\
			op " %%ymm5, %%ymm4, %%ymm3\n"



#define AVX_S_FMA_IR4_0(op,name) \
static uint64_t AVX_S_FMA_IR4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm4, %%xmm4\n"	\
		"vshufps	$0, %%xmm4, %%xmm4, %%xmm4\n"	\
		"vperm2f128	$0, %%ymm4, %%ymm4, %%ymm4\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2ss	%%eax, %%xmm5, %%xmm5\n"	\
		"vshufps	$0, %%xmm5, %%xmm5, %%xmm5\n"	\
		"vperm2f128	$0, %%ymm5, %%ymm5, %%ymm5\n"	\
		"mov	%[loop], %%eax\n"	\
	"1:\n"	\
		AVX_S_FMA_IR4_8( op )	\
		AVX_S_FMA_IR4_8( op )	\
		AVX_S_FMA_IR4_8( op )	\
		AVX_S_FMA_IR4_8( op )	\
		AVX_S_FMA_IR4_8( op )	\
		"dec	%%eax\n"	\
		"jne	1b\n"	 	\
		"vmovss	%%xmm0, %[o0]\n"	\
		"vmovss	%%xmm1, %[o1]\n"	\
		"vmovss	%%xmm2, %[o2]\n"	\
		"vmovss	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
	: [loop]"r"(LoopCount)	\
	: "cc","%eax",	\
		"%ymm0","%ymm1","%ymm2","%ymm3","%ymm4","%ymm5","%ymm6","%ymm7"\
		); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );	\
	check_result( ret1, answer );	\
	check_result( ret2, answer );	\
	check_result( ret3, answer );*/	\
	return	timer.Result();	\
}

#define AVX_S_FMA_IR4(op,name) AVX_S_FMA_IR4_0(#op,name)


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
	"FMA vfmaddps (32bit x4) n8",

	"SSE vml+ad+adps (32bit x4) n6",



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

	SetOp( RESULT_SSE_ML_AD_ADDPS_IR6,	PER_LOOP_INST_6 * 4, 4	);


	SetOp( RESULT_SSE_MULSS_IRS4,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_ADDSS_IRS4,		PER_LOOP_INST * 1, 1	);

	SetOp( RESULT_SSE_MULPS_IRS4,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_ADDPS_IRS4,		PER_LOOP_INST * 4, 4	);



	SetOp( RESULT_AVX_VMULPS_IR4,		PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VADDPS_IR4,		PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VMULPS_VADDPS_IR4,PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VFMADDPS_IR8,		PER_LOOP_INST * 16, 16	);
	SetOp( RESULT_AVX_VFMADDPS_IR12,	PER_LOOP_INST * 16, 16	);

	SetOp( RESULT_AVX_VML_AD_VADDPS_IR6,PER_LOOP_INST_6 * 8, 8	);

}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



SSE_S_IR8( mulss,  mulss_ir8 );
SSE_S_IR8( addss,  addss_ir8 );
SSE_S_FMA_IR8( vfmadd231ss,  fmaddss_ir8 );	// FMA3

SSE_S_IR8( mulps,  mulps_ir8 );
SSE_S_IR8( addps,  addps_ir8 );
SSE_M_IR8( mulps, addps,  mulps_addps_ir8 );
SSE_S_FMA_IR8( vfmadd231ps,  fmaddps_ir8 );	// FMA3

SSE_M_IR6( mulps, addps, addps, ml_ad_addps_ir6 );



SSE_S_IRS4( mulss,  mulss_irs4 );
SSE_S_IRS4( addss,  addss_irs4 );

SSE_S_IRS4( mulps,  mulps_irs4 );
SSE_S_IRS4( addps,  addps_irs4 );



AVX_S_IR4( vmulps,  vmulps_ir4 );
AVX_S_IR4( vaddps,  vaddps_ir4 );
AVX_M_IR4( vmulps, vaddps,  vmulps_vaddps_ir4 );
AVX_S_FMA_IR4( vfmadd231ps,  vfmaddps_ir4 );	// FMA3
//AVX_S_FMA_IR12( vfmadd231ps,  vfmaddps_ir12 );	// FMA3

AVX_M_IR6( vmulps, vaddps, vaddps, vml_ad_vaddps_ir6 );





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
	SetResult( RESULT_SSE_MULSS_IR8,	SSE_S_IR8_mulss_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_SSE_ADDSS_IR8,	SSE_S_IR8_addss_ir8( Loop, 10.0f		) );
	Progress++;

	if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

		SetResult( RESULT_SSE_FMADDSS_IR8,	SSE_S_FMA_IR8_fmaddss_ir8( Loop, 10.0f		) );
		Progress++;

	}else{
		Progress++;
	}


	SetResult( RESULT_SSE_MULPS_IR8,	SSE_S_IR8_mulps_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_SSE_ADDPS_IR8,	SSE_S_IR8_addps_ir8( Loop, 10.0f		) );
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
	// IR6
	//------------------------------------------------------

	SetResult( RESULT_SSE_ML_AD_ADDPS_IR6,	SSE_M_IR6_ml_ad_addps_ir6( Loop, 10.0f		) );
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
		// IR4
		//------------------------------------------------------
		SetResult( RESULT_AVX_VMULPS_IR4,	AVX_S_IR4_vmulps_ir4( Loop, 10.0f		) );
		Progress++;

		SetResult( RESULT_AVX_VADDPS_IR4,	AVX_S_IR4_vaddps_ir4( Loop, 7.0f		) );
		Progress++;


		SetResult( RESULT_AVX_VMULPS_VADDPS_IR4,	AVX_M_IR4_vmulps_vaddps_ir4( Loop, 7.0f		) );
		Progress++;


		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

			SetResult( RESULT_AVX_VFMADDPS_IR8,	AVX_S_FMA_IR4_vfmaddps_ir4( Loop, 10.0f		) );
			Progress++;

			//SetResult( RESULT_AVX_VFMADDPS_IR12,	AVX_S_FMA_IR12_vfmaddps_ir12( Loop, 10.0f		) );
			SetResult( RESULT_AVX_VFMADDPS_IR12,	AVX_S_FMA_IR4_vfmaddps_ir4( Loop, 10.0f		) );
			Progress++;

		}else{
			Progress++;
			Progress++;
		}


		//------------------------------------------------------
		// IR6
		//------------------------------------------------------

		SetResult( RESULT_AVX_VML_AD_VADDPS_IR6,	AVX_M_IR6_vml_ad_vaddps_ir6( Loop, 7.0f		) );
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
#endif // x86
//-----------------------------------------------------------------------------

