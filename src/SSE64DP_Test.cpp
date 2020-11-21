// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE64DP_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_X64
//-----------------------------------------------------------------------------
#if defined(__AVX512F__) && __AVX512F__
# define	ENABLE_AVX512	1
#else
# define	ENABLE_AVX512	0
#endif


namespace SSE64DP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

using namespace flatlib;
using system::CPUFeature;

/*
	xmm0-xmm7
*/



typedef	uint64_t	CounterType;


//=============================================================================
// SSE
//=============================================================================

static void check_result( double ret, double ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	FL_ASSERT( ret == ans );
}


#define SSE_REG_CLEAR() \
		"xorpd	%%xmm0, %%xmm0\n"	\
		"xorpd	%%xmm1, %%xmm1\n"	\
		"xorpd	%%xmm2, %%xmm2\n"	\
		"xorpd	%%xmm3, %%xmm3\n"	\
		"xorpd	%%xmm4, %%xmm4\n"	\
		"xorpd	%%xmm5, %%xmm5\n"	\
		"xorpd	%%xmm6, %%xmm6\n"	\
		"xorpd	%%xmm7, %%xmm7\n"	\
		"xorpd	%%xmm8, %%xmm8\n"	\
		"xorpd	%%xmm9, %%xmm9\n"	\
		"xorpd	%%xmm10, %%xmm10\n"	\
		"xorpd	%%xmm11, %%xmm11\n"	\
		"xorpd	%%xmm12, %%xmm12\n"	\
		"xorpd	%%xmm13, %%xmm13\n"	\
		"xorpd	%%xmm14, %%xmm14\n"	\
		"xorpd	%%xmm15, %%xmm15\n"	\





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
static uint64_t SSE_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm0\n"	\
		"shufpd		$0, %%xmm0, %%xmm0\n"	\
		"movapd		%%xmm0, %%xmm2\n"	\
		"movapd		%%xmm0, %%xmm4\n"	\
		"movapd		%%xmm0, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm1\n"	\
		"shufpd		$0, %%xmm1, %%xmm1\n"	\
		"movapd		%%xmm1, %%xmm3\n"	\
		"movapd		%%xmm1, %%xmm5\n"	\
		"movapd		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		SSE_S_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
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
static uint64_t SSE_S_IRS4_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm0\n"	\
		"shufpd		$0, %%xmm0, %%xmm0\n"	\
		"movapd		%%xmm0, %%xmm2\n"	\
		"movapd		%%xmm0, %%xmm4\n"	\
		"movapd		%%xmm0, %%xmm6\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm1\n"	\
		"shufpd		$0, %%xmm1, %%xmm1\n"	\
		"movapd		%%xmm1, %%xmm3\n"	\
		"movapd		%%xmm1, %%xmm5\n"	\
		"movapd		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		SSE_S_IRS4_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
	);\
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
static uint64_t SSE_M_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm0\n"	\
		"shufpd		$0, %%xmm0, %%xmm0\n"	\
		"movapd		%%xmm0, %%xmm2\n"	\
		"movapd		%%xmm0, %%xmm4\n"	\
		"movapd		%%xmm0, %%xmm6\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm1\n"	\
		"shufpd		$0, %%xmm1, %%xmm1\n"	\
		"movapd		%%xmm1, %%xmm3\n"	\
		"movapd		%%xmm1, %%xmm5\n"	\
		"movapd		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		SSE_M_IR8_8( op1,op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
	);\
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
// Single Interleave 9 MAD op3
//-----------------------------------------------------------------------------

#define SSE_M_IR9_9(op1,op2,op3) \
			op1 " %%xmm0, %%xmm0\n"		\
			op2 " %%xmm1, %%xmm1\n"		\
			op3 " %%xmm2, %%xmm2\n"		\
			op1 " %%xmm3, %%xmm3\n"		\
			op2 " %%xmm4, %%xmm4\n"		\
			op3 " %%xmm5, %%xmm5\n"		\
			op1 " %%xmm6, %%xmm6\n"		\
			op2 " %%xmm7, %%xmm7\n"		\
			op3 " %%xmm8, %%xmm8\n"



#define SSE_M_IR9_0(op1,op2,op3,name) \
static uint64_t SSE_M_IR9_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm0\n"	\
		"shufpd		$0, %%xmm0, %%xmm0\n"	\
		"movapd		%%xmm0, %%xmm2\n"	\
		"movapd		%%xmm0, %%xmm4\n"	\
		"movapd		%%xmm0, %%xmm6\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm1\n"	\
		"shufpd		$0, %%xmm1, %%xmm1\n"	\
		"movapd		%%xmm1, %%xmm3\n"	\
		"movapd		%%xmm1, %%xmm5\n"	\
		"movapd		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		SSE_M_IR9_9( op1,op2,op3 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
	);\
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
static uint64_t SSE_S_FMA_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm0\n"	\
		"shufpd		$0, %%xmm0, %%xmm0\n"	\
		"movapd		%%xmm0, %%xmm2\n"	\
		"movapd		%%xmm0, %%xmm4\n"	\
		"movapd		%%xmm0, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm1\n"	\
		"shufpd		$0, %%xmm1, %%xmm1\n"	\
		"movapd		%%xmm1, %%xmm3\n"	\
		"movapd		%%xmm1, %%xmm5\n"	\
		"movapd		%%xmm1, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		SSE_S_FMA_IR8_8( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
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

#define SSE_S_FMA_IR8(op,name) SSE_S_FMA_IR8_0(#op,name)






//-----------------------------------------------------------------------------
// Single Interleave 12 FMA
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
static uint64_t SSE_S_FMA_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"movapd		%%xmm8, %%xmm2\n"	\
		"movapd		%%xmm8, %%xmm4\n"	\
		"movapd		%%xmm8, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
		"movapd		%%xmm9, %%xmm3\n"	\
		"movapd		%%xmm9, %%xmm5\n"	\
		"movapd		%%xmm9, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		SSE_S_FMA_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
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

#define SSE_S_FMA_IR12(op,name) SSE_S_FMA_IR12_0(#op,name)




//-----------------------------------------------------------------------------
// Single Interleave 12 FMA+SSE
//-----------------------------------------------------------------------------

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



#define SSE_M_FMA_IR12_0(op1,op2,name) \
static uint64_t SSE_M_FMA_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SSE_REG_CLEAR()			\
		"mov		$1, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"movapd		%%xmm8, %%xmm2\n"	\
		"movapd		%%xmm8, %%xmm4\n"	\
		"movapd		%%xmm8, %%xmm6\n"	\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
		"movapd		%%xmm9, %%xmm3\n"	\
		"movapd		%%xmm9, %%xmm5\n"	\
		"movapd		%%xmm9, %%xmm7\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		SSE_M_FMA_IR12_12( op1, op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
		"movsd	%%xmm4, %[o4]\n"	\
		"movsd	%%xmm5, %[o5]\n"	\
		"movsd	%%xmm6, %[o6]\n"	\
		"movsd	%%xmm7, %[o7]\n"	\
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
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"\
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

#define SSE_M_FMA_IR12(op1,op2,name) SSE_M_FMA_IR12_0(#op1,#op2,name)


















//=============================================================================
// AVX
//=============================================================================


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define AVX_REG_CLEAR() \
		"vxorpd	%%ymm0, %%ymm0, %%ymm0\n"	\
		"vxorpd	%%ymm1, %%ymm1, %%ymm1\n"	\
		"vxorpd	%%ymm2, %%ymm2, %%ymm2\n"	\
		"vxorpd	%%ymm3, %%ymm3, %%ymm3\n"	\
		"vxorpd	%%ymm4, %%ymm4, %%ymm4\n"	\
		"vxorpd	%%ymm5, %%ymm5, %%ymm5\n"	\
		"vxorpd	%%ymm6, %%ymm6, %%ymm6\n"	\
		"vxorpd	%%ymm7, %%ymm7, %%ymm7\n"	\
		"vxorpd	%%ymm8, %%ymm8, %%ymm8\n"	\
		"vxorpd	%%ymm9, %%ymm9, %%ymm9\n"	\
		"vxorpd	%%ymm10, %%ymm10, %%ymm10\n"	\
		"vxorpd	%%ymm11, %%ymm11, %%ymm11\n"	\
		"vxorpd	%%ymm12, %%ymm12, %%ymm12\n"	\
		"vxorpd	%%ymm13, %%ymm13, %%ymm13\n"	\
		"vxorpd	%%ymm14, %%ymm14, %%ymm14\n"	\
		"vxorpd	%%ymm15, %%ymm15, %%ymm15\n"	\



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
static uint64_t AVX_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
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
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	return	timer.Result();	\
}

#define AVX_S_IR8(op,name) AVX_S_IR8_0(#op,name)







//-----------------------------------------------------------------------------
// Single Interleave MAD
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
static uint64_t AVX_M_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_M_IR8_8( op1, op2 )	\
		AVX_M_IR8_8( op1, op2 )	\
		AVX_M_IR8_8( op1, op2 )	\
		AVX_M_IR8_8( op1, op2 )	\
		AVX_M_IR8_8( op1, op2 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	check_result( ret3, answer );*/	\
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
			op3 " %%ymm9, %%ymm8, %%ymm10\n"



#define AVX_M_IR9_0(op1,op2,op3,name) \
static uint64_t AVX_M_IR9_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
		"vperm2f128	$0, %%ymm9, %%ymm9, %%ymm9\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX_M_IR9_9( op1, op2, op3 )	\
		AVX_M_IR9_9( op1, op2, op3 )	\
		AVX_M_IR9_9( op1, op2, op3 )	\
		AVX_M_IR9_9( op1, op2, op3 )	\
		AVX_M_IR9_9( op1, op2, op3 )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	check_result( ret3, answer );*/	\
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
static uint64_t AVX_S_FMA_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
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
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	check_result( ret3, answer );*/	\
	return	timer.Result();	\
}

#define AVX_S_FMA_IR8(op,name) AVX_S_FMA_IR8_0(#op,name)



//-----------------------------------------------------------------------------
// Single Interleave Register 12 FMA
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
static uint64_t AVX_S_FMA_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
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
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	check_result( ret3, answer );*/	\
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
static uint64_t AVX_M_FMA_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm8\n"	\
		"shufpd		$0, %%xmm8, %%xmm8\n"	\
		"vperm2f128	$0, %%ymm8, %%ymm8, %%ymm8\n"	\
		"mov		$5, %%eax\n"		\
		"cvtsi2sd	%%eax, %%xmm9\n"	\
		"shufpd		$0, %%xmm9, %%xmm9\n"	\
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
		"movsd	%%xmm0, %[o0]\n"	\
		"movsd	%%xmm1, %[o1]\n"	\
		"movsd	%%xmm2, %[o2]\n"	\
		"movsd	%%xmm3, %[o3]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3)	\
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
	check_result( ret3, answer );*/	\
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
static uint64_t AVX512_S_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2sd	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufpd	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastsd	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2sd	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufpd	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastsd	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		AVX512_S_IR12_12( op )	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovsd	%%xmm0, %[o0]\n"	\
		"vmovsd	%%xmm1, %[o1]\n"	\
		"vmovsd	%%xmm2, %[o2]\n"	\
		"vmovsd	%%xmm3, %[o3]\n"	\
		"vmovsd	%%xmm4, %[o4]\n"	\
		"vmovsd	%%xmm5, %[o5]\n"	\
		"vmovsd	%%xmm6, %[o6]\n"	\
		"vmovsd	%%xmm7, %[o7]\n"	\
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
static uint64_t AVX512_M_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		AVX512_REG_CLEAR()			\
		"mov		$2, %%eax\n"		\
		"vcvtsi2sd	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufpd	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastsd	%%xmm30, %%zmm12\n"	\
		"mov		$5, %%eax\n"		\
		"vcvtsi2sd	%%eax, %%xmm30, %%xmm30\n"	\
		"vshufpd	$0, %%xmm30, %%xmm30, %%xmm30\n"	\
		"vbroadcastsd	%%xmm30, %%zmm13\n"	\
		"mov	%[loop], %%rax\n"	\
	"1:\n"	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		AVX512_M_IR12_12( op1, op2)	\
		"dec	%%rax\n"	\
		"jne	1b\n"	 	\
		"vmovsd	%%xmm0, %[o0]\n"	\
		"vmovsd	%%xmm1, %[o1]\n"	\
		"vmovsd	%%xmm2, %[o2]\n"	\
		"vmovsd	%%xmm3, %[o3]\n"	\
		"vmovsd	%%xmm4, %[o4]\n"	\
		"vmovsd	%%xmm5, %[o5]\n"	\
		"vmovsd	%%xmm6, %[o6]\n"	\
		"vmovsd	%%xmm7, %[o7]\n"	\
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






#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



static const char*	Instruction_Title[]= {

	"SSE2 mulsd (64bit x1) n8",
	"SSE2 addsd (64bit x1) n8",
	"FMA  vfmaddsd (64bit x1) n8",
	"FMA  vfmaddsd (64bit x1) n12",
	"FMA  vfma+mlsd (64bit x1) n12",
	"FMA  vfma+adsd (64bit x1) n12",

	"SSE2 mulpd (64bit x2) n8",
	"SSE2 addpd (64bit x2) n8",
	"SSE2 mul+addpd (64bit x2) n8",
	"FMA  vfmaddpd (64bit x2) n8",
	"FMA  vfmaddpd (64bit x2) n12",
	"FMA  vfma+mlpd (64bit x2) n12",
	"FMA  vfma+adpd (64bit x2) n12",


	"SSE2 ml+ad+dpd (64bit x2) n9",


	"SSE2 mulsd (64bit x1) ns4",
	"SSE2 addsd (64bit x1) ns4",

	"SSE2 mulpd (64bit x2) ns4",
	"SSE2 addpd (64bit x2) ns4",


	"AVX vmulpd (64bit x4) n8",
	"AVX vaddpd (64bit x4) n8",
	"AVX vmul+addpd (64bit x4) n8",
	"FMA vfmaddpd (64bit x4) n8",
	"FMA vfmaddpd (64bit x4) n12",

	"FMA vfma+mlpd (64bit x4) n12",
	"FMA vfma+adpd (64bit x4) n12",

	"AVX vml_ad_adpd (64bit x4) n9",


	"AVX512 vmulpd (64bit x8) n12",
	"AVX512 vaddpd (64bit x8) n12",
	"AVX512 vfmaddpd (64bit x8) n12",
	"AVX512 vfma+mpd (64bit x8) n12",
	"AVX512 vfma+apd (64bit x8) n12",
};





//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	FL_ASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_SSE_MULSD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_ADDSD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_FMADDSD_IR8,		PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_FMADDSD_IR12,		PER_LOOP_INST_12 * 2, 2	);
	SetOp2f( RESULT_SSE_FMA_MULSD_IR12,	PER_LOOP_INST_12, (2+1)/2.0f	);
	SetOp2f( RESULT_SSE_FMA_ADDSD_IR12,	PER_LOOP_INST_12, (2+1)/2.0f	);

	SetOp( RESULT_SSE_MULPD_IR8,		PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_ADDPD_IR8,		PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_MULPD_ADDPD_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_FMADDPD_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_SSE_FMADDPD_IR12,		PER_LOOP_INST_12 * 4, 4	);
	SetOp2f( RESULT_SSE_FMA_MULPD_IR12,	PER_LOOP_INST_12, (4+2)/2.0f	);
	SetOp2f( RESULT_SSE_FMA_ADDPD_IR12,	PER_LOOP_INST_12, (4+2)/2.0f	);


	SetOp( RESULT_SSE_ML_AD_ADDPD_IR9,	PER_LOOP_INST_9 * 2, 2	);


	SetOp( RESULT_SSE_MULSD_IRS4,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_SSE_ADDSD_IRS4,		PER_LOOP_INST * 1, 1	);

	SetOp( RESULT_SSE_MULPD_IRS4,		PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_SSE_ADDPD_IRS4,		PER_LOOP_INST * 2, 2	);


	SetOp( RESULT_AVX_VMULPD_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_AVX_VADDPD_IR8,		PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_AVX_VMULPD_VADDPD_IR8,PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_AVX_VFMADDPD_IR8,		PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_AVX_VFMADDPD_IR12,	PER_LOOP_INST_12 * 8, 8	);

	SetOp2f( RESULT_AVX_FMA_MUL_IR12,	PER_LOOP_INST_12, (8+4)/2.0f	);
	SetOp2f( RESULT_AVX_FMA_ADD_IR12,	PER_LOOP_INST_12, (8+4)/2.0f	);


	SetOp( RESULT_AVX_VML_AD_VADDPD_IR9,PER_LOOP_INST_9 * 4, 4	);


	SetOp2f( RESULT_AVX512_VMULPD_IR12,		PER_LOOP_INST_12, 8	);
	SetOp2f( RESULT_AVX512_VADDPD_IR12,		PER_LOOP_INST_12, 8	);
	SetOp2f( RESULT_AVX512_VFMADDPD_IR12,	PER_LOOP_INST_12, 16	);
	SetOp2f( RESULT_AVX512_FMA_MUL_IR12,	PER_LOOP_INST_12, (16+8)/2.0f	);
	SetOp2f( RESULT_AVX512_FMA_ADD_IR12,	PER_LOOP_INST_12, (16+8)/2.0f	);
}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



SSE_S_IR8( mulsd,  mulsd_ir8 );
SSE_S_IR8( addsd,  addsd_ir8 );
SSE_S_FMA_IR8( vfmadd231sd,  fmaddsd_ir8 );
SSE_S_FMA_IR12( vfmadd231sd,  fmaddsd_ir12 );
SSE_M_FMA_IR12( vfmadd231sd,  mulsd,	fmaddsd_mulsd_ir12 );
SSE_M_FMA_IR12( vfmadd231sd,  addsd,	fmaddsd_addsd_ir12 );

SSE_S_IR8( mulpd,  mulpd_ir8 );
SSE_S_IR8( addpd,  addpd_ir8 );
SSE_M_IR8( mulpd, addpd,  mulpd_addpd_ir8 );
SSE_S_FMA_IR8( vfmadd231pd,  fmaddpd_ir8 );
SSE_S_FMA_IR12( vfmadd231pd,  fmaddpd_ir12 );
SSE_M_FMA_IR12( vfmadd231pd,  mulpd,	fmaddpd_mulpd_ir12 );
SSE_M_FMA_IR12( vfmadd231pd,  addpd,	fmaddpd_addpd_ir12 );


SSE_M_IR9( mulpd, addpd, addpd, ml_ad_addpd_ir9 );


SSE_S_IRS4( mulsd,  mulsd_irs4 );
SSE_S_IRS4( addsd,  addsd_irs4 );

SSE_S_IRS4( mulpd,  mulpd_irs4 );
SSE_S_IRS4( addpd,  addpd_irs4 );


AVX_S_IR8( vmulpd,  vmulpd_ir8 );
AVX_S_IR8( vaddpd,  vaddpd_ir8 );
AVX_M_IR8( vmulpd, vaddpd,  vmulpd_vaddpd_ir8 );
AVX_S_FMA_IR8( vfmadd231pd,  vfmaddpd_ir8 );
AVX_S_FMA_IR12( vfmadd231pd,  vfmaddpd_ir12 );

AVX_M_FMA_IR12( vfmadd231pd,  vmulpd,	vfmaddpd_vmulpd_ir12 );
AVX_M_FMA_IR12( vfmadd231pd,  vaddpd,	vfmaddpd_vaddpd_ir12 );

AVX_M_IR9( vmulpd, vaddpd, vaddpd, vml_ad_vaddpd_ir9 );


#if ENABLE_AVX512
AVX512_S_IR12( vmulpd,	vmulpd_ir12 );
AVX512_S_IR12( vaddpd,	vaddpd_ir12 );
AVX512_M_IR12( vfmadd231pd,	vfmadd231pd,	vfmaddpd_ir12 );
AVX512_M_IR12( vfmadd231pd,	vmulpd,			vfmaddpd_vmulpd_ir12 );
AVX512_M_IR12( vfmadd231pd,	vaddpd,			vfmaddpd_vaddpd_ir12 );
#endif



void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "SSE loop=%d\n", Loop );

	double	sum= 0;
	double	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 2.0f;
		sum_2+= 10.0f;
	}




	//------------------------------------------------------
	// IR8
	//------------------------------------------------------
	SetResult( RESULT_SSE_MULSD_IR8,	SSE_S_IR8_mulsd_ir8( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_SSE_ADDSD_IR8,	SSE_S_IR8_addsd_ir8( Loop, 10.0f		) );
	Progress.Increment();

	if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

		SetResult( RESULT_SSE_FMADDSD_IR8,	SSE_S_FMA_IR8_fmaddsd_ir8( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMADDSD_IR12,	SSE_S_FMA_IR12_fmaddsd_ir12( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMA_MULSD_IR12,	SSE_M_FMA_IR12_fmaddsd_mulsd_ir12( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMA_ADDSD_IR12,	SSE_M_FMA_IR12_fmaddsd_addsd_ir12( Loop, 10.0f		) );
		Progress.Increment();

	}else{
		Progress.Increment();
		Progress.Increment();
		Progress.Increment();
		Progress.Increment();
	}




	SetResult( RESULT_SSE_MULPD_IR8,	SSE_S_IR8_mulpd_ir8( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_SSE_ADDPD_IR8,	SSE_S_IR8_addpd_ir8( Loop, 10.0f		) );
	Progress.Increment();


	SetResult( RESULT_SSE_MULPD_ADDPD_IR8,	SSE_M_IR8_mulpd_addpd_ir8( Loop, 10.0f		) );
	Progress.Increment();


	if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

		SetResult( RESULT_SSE_FMADDPD_IR8,	SSE_S_FMA_IR8_fmaddpd_ir8( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMADDPD_IR12,	SSE_S_FMA_IR12_fmaddpd_ir12( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMA_MULPD_IR12,	SSE_M_FMA_IR12_fmaddpd_mulpd_ir12( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_SSE_FMA_ADDPD_IR12,	SSE_M_FMA_IR12_fmaddpd_addpd_ir12( Loop, 10.0f		) );
		Progress.Increment();

	}else{
		Progress.Increment();
		Progress.Increment();
		Progress.Increment();
		Progress.Increment();
	}



	//------------------------------------------------------
	// IR9
	//------------------------------------------------------

	SetResult( RESULT_SSE_ML_AD_ADDPD_IR9,	SSE_M_IR9_ml_ad_addpd_ir9( Loop, 10.0f		) );
	Progress.Increment();



	//------------------------------------------------------
	// IRS4
	//------------------------------------------------------
	SetResult( RESULT_SSE_MULSD_IRS4,	SSE_S_IRS4_mulsd_irs4( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_SSE_ADDSD_IRS4,	SSE_S_IRS4_addsd_irs4( Loop, 10.0f		) );
	Progress.Increment();



	SetResult( RESULT_SSE_MULPD_IRS4,	SSE_S_IRS4_mulpd_irs4( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_SSE_ADDPD_IRS4,	SSE_S_IRS4_addpd_irs4( Loop, 10.0f		) );
	Progress.Increment();






	//------------------------------------------------------
	//------------------------------------------------------


	//------------------------------------------------------
	// AVX
	//------------------------------------------------------

	if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){

		//------------------------------------------------------
		// IR8
		//------------------------------------------------------
		SetResult( RESULT_AVX_VMULPD_IR8,	AVX_S_IR8_vmulpd_ir8( Loop, 10.0f		) );
		Progress.Increment();

		SetResult( RESULT_AVX_VADDPD_IR8,	AVX_S_IR8_vaddpd_ir8( Loop, 7.0f		) );
		Progress.Increment();


		SetResult( RESULT_AVX_VMULPD_VADDPD_IR8,	AVX_M_IR8_vmulpd_vaddpd_ir8( Loop, 7.0f		) );
		Progress.Increment();


		if( Info.HasInstructionSet( CPUFeature::IA_FMA3 ) ){

			SetResult( RESULT_AVX_VFMADDPD_IR8,	AVX_S_FMA_IR8_vfmaddpd_ir8( Loop, 10.0f		) );	// **!!!** 15
			Progress.Increment();

			SetResult( RESULT_AVX_VFMADDPD_IR12,	AVX_S_FMA_IR12_vfmaddpd_ir12( Loop, 10.0f		) );
			Progress.Increment();

			SetResult( RESULT_AVX_FMA_MUL_IR12,	AVX_M_FMA_IR12_vfmaddpd_vmulpd_ir12( Loop, 10.0f		) );
			Progress.Increment();

			SetResult( RESULT_AVX_FMA_ADD_IR12,	AVX_M_FMA_IR12_vfmaddpd_vaddpd_ir12( Loop, 10.0f		) );
			Progress.Increment();

		}else{
			Progress.Increment();
			Progress.Increment();
			Progress.Increment();
			Progress.Increment();
		}


		//------------------------------------------------------
		// IR9
		//------------------------------------------------------
		SetResult( RESULT_AVX_VML_AD_VADDPD_IR9,	AVX_M_IR9_vml_ad_vaddpd_ir9( Loop, 7.0f		) );
		Progress.Increment();

	}else{
		Progress.LoadAdd( 8 );
	}


	//------------------------------------------------------
	// AVX512
	//------------------------------------------------------

#if ENABLE_AVX512
	if( Info.HasInstructionSet( CPUFeature::IA_AVX512F ) ){

		SetResult( RESULT_AVX512_VMULPD_IR12,	AVX512_S_IR12_vmulpd_ir12( Loop, 0.0f		) );
		Progress.Increment();

		SetResult( RESULT_AVX512_VADDPD_IR12,	AVX512_S_IR12_vaddpd_ir12( Loop, 0.0f		) );
		Progress.Increment();

		SetResult( RESULT_AVX512_VFMADDPD_IR12,	AVX512_M_IR12_vfmaddpd_ir12( Loop, 0.0f		) );
		Progress.Increment();

		SetResult( RESULT_AVX512_FMA_MUL_IR12,	AVX512_M_IR12_vfmaddpd_vmulpd_ir12( Loop, 0.0f		) );
		Progress.Increment();

		SetResult( RESULT_AVX512_FMA_ADD_IR12,	AVX512_M_IR12_vfmaddpd_vaddpd_ir12( Loop, 0.0f		) );
		Progress.Increment();

	}else
#endif
	{
		Progress.LoadAdd( 5 );
	}


	//------------------------------------------------------
	//------------------------------------------------------

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
		return	"SSE/AVX (DP fp)";
	}else{
		return	"SSE/AVX (DP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // x64
//-----------------------------------------------------------------------------

