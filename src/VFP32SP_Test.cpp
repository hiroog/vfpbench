// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP32SP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_ARM7 || flCPU_ARM6
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace VFP32SP {
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

static void check_result( float ret, float ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	flASSERT( ret == ans );
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
// Single Interleave 1
//-----------------------------------------------------------------------------

#define VFP_S_IR1_8(op) \
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"		\
			op " s0, s8, s9 \n"



#define VFP_S_IR1_0(op,name) \
static uint64_t VFP_S_IR1_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d5, r2, r3\n"	\
		"fsitos	s8, s10\n"		\
		"fsitos	s9, s11\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fsts	s0, %[o0]\n"	\
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

#define VFP_S_IR1(op,name) VFP_S_IR1_0(#op,name)







//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define VFP_S_IR8_8(op) \
			op " s0, s8, s9 \n"		\
			op " s1, s8, s9 \n"		\
			op " s2, s8, s9 \n"		\
			op " s3, s8, s9 \n"		\
			op " s4, s8, s9 \n"		\
			op " s5, s8, s9 \n"		\
			op " s6, s8, s9 \n"		\
			op " s7, s8, s9 \n"



#define VFP_S_IR8_0(op,name) \
static uint64_t VFP_S_IR8_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d5, r2, r3\n"	\
		"fsitos	s8, s10\n"		\
		"fsitos	s9, s11\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fsts	s0, %[o0]\n"	\
		"fsts	s1, %[o1]\n"	\
		"fsts	s2, %[o2]\n"	\
		"fsts	s3, %[o3]\n"	\
		"fsts	s4, %[o4]\n"	\
		"fsts	s5, %[o5]\n"	\
		"fsts	s6, %[o6]\n"	\
		"fsts	s7, %[o7]\n"	\
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

#define VFP_S_IR8(op,name) VFP_S_IR8_0(#op,name)




//-----------------------------------------------------------------------------
// Single Interleave 8d
//-----------------------------------------------------------------------------

#define VFP_S_IR8D_8(op) \
			op " s0,  s16, s18 \n"		\
			op " s2,  s16, s18 \n"		\
			op " s4,  s16, s18 \n"		\
			op " s6,  s16, s18 \n"		\
			op " s8,  s16, s18 \n"		\
			op " s10, s16, s18 \n"		\
			op " s12, s16, s18 \n"		\
			op " s14, s16, s18 \n"



#define VFP_S_IR8D_0(op,name) \
static uint64_t VFP_S_IR8D_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d10, r2, r3\n"	\
		"fsitos	s16, s20\n"		\
		"fsitos	s18, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_S_IR8D_8( op )	\
		VFP_S_IR8D_8( op )	\
		VFP_S_IR8D_8( op )	\
		VFP_S_IR8D_8( op )	\
		VFP_S_IR8D_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fsts	s0, %[o0]\n"	\
		"fsts	s2, %[o1]\n"	\
		"fsts	s4, %[o2]\n"	\
		"fsts	s6, %[o3]\n"	\
		"fsts	s8, %[o4]\n"	\
		"fsts	s10, %[o5]\n"	\
		"fsts	s12, %[o6]\n"	\
		"fsts	s14, %[o7]\n"	\
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

#define VFP_S_IR8D(op,name) VFP_S_IR8D_0(#op,name)






//-----------------------------------------------------------------------------
// Single Interleave src 4
//-----------------------------------------------------------------------------

#define VFP_S_IRS4_8(op) \
			op " s0, s8, s4 \n"		\
			op " s1, s8, s5 \n"		\
			op " s2, s8, s6 \n"		\
			op " s3, s8, s7 \n"		\
			op " s4, s8, s0 \n"		\
			op " s5, s8, s1 \n"		\
			op " s6, s8, s2 \n"		\
			op " s7, s8, s3 \n"



#define VFP_S_IRS4_0(op,name) \
static uint64_t VFP_S_IRS4_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"fmdrr  d5, r2, r3\n"	\
		"fsitos	s8, s10\n"		\
		"fsitos	s9, s11\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"fsts	s0, %[o0]\n"	\
		"fsts	s1, %[o1]\n"	\
		"fsts	s2, %[o2]\n"	\
		"fsts	s3, %[o3]\n"	\
		"fsts	s4, %[o4]\n"	\
		"fsts	s5, %[o5]\n"	\
		"fsts	s6, %[o6]\n"	\
		"fsts	s7, %[o7]\n"	\
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

#define VFP_S_IRS4(op,name) VFP_S_IRS4_0(#op,name)






//=============================================================================
// NEON
//=============================================================================

//-----------------------------------------------------------------------------
#if flCPU_NEON
//-----------------------------------------------------------------------------


#define NEON_REG_CLEAR() \
		"mov	r0, #0\n"		\
		"vmov	d0, r0, r0\n"	\
		"vmov	d1, r0, r0\n"	\
		"vmov	q1, q0\n"	\
		"vmov	q2, q0\n"	\
		"vmov	q3, q0\n"	\
		"vmov	q4, q0\n"	\
		"vmov	q5, q0\n"	\
		"vmov	q6, q0\n"	\
		"vmov	q7, q0\n"	\
		"vmov	q8, q0\n"	\
		"vmov	q9, q0\n"	\
		"vmov	q10, q0\n"	\
		"vmov	q11, q0\n"	\
		"vmov	q12, q0\n"	\
		"vmov	q13, q0\n"	\
		"vmov	q14, q0\n"	\
		"vmov	q15, q0\n"	\



//-----------------------------------------------------------------------------
// Double 32x2 Interleave 1
//-----------------------------------------------------------------------------

#define NEON_D_IR1_8(op) \
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"



#define NEON_D_IR1_0(op,name) \
static uint64_t NEON_D_IR1_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d10, r2, r3\n"	\
		"vcvt.f32.s32	s16, s20\n"		\
		"vcvt.f32.s32	s17, s20\n"		\
		"vcvt.f32.s32	s18, s21\n"		\
		"vcvt.f32.s32	s19, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_D_IR1_8( op )	\
		NEON_D_IR1_8( op )	\
		NEON_D_IR1_8( op )	\
		NEON_D_IR1_8( op )	\
		NEON_D_IR1_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","r0","r2","r3",	\
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	return	timer.Result();	\
}

#define NEON_D_IR1(op,name) NEON_D_IR1_0(#op,name)




//-----------------------------------------------------------------------------
// Double 32x2 Interleave 8
//-----------------------------------------------------------------------------

#define NEON_D_IR8_8(op) \
			op " d0, d8, d9 \n"		\
			op " d1, d8, d9 \n"		\
			op " d2, d8, d9 \n"		\
			op " d3, d8, d9 \n"		\
			op " d4, d8, d9 \n"		\
			op " d5, d8, d9 \n"		\
			op " d6, d8, d9 \n"		\
			op " d7, d8, d9 \n"



#define NEON_D_IR8_0(op,name) \
static uint64_t NEON_D_IR8_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d10, r2, r3\n"	\
		"vcvt.f32.s32	s16, s20\n"		\
		"vcvt.f32.s32	s17, s20\n"		\
		"vcvt.f32.s32	s18, s21\n"		\
		"vcvt.f32.s32	s19, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_D_IR8_8( op )	\
		NEON_D_IR8_8( op )	\
		NEON_D_IR8_8( op )	\
		NEON_D_IR8_8( op )	\
		NEON_D_IR8_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
		"vstr.32	s1, %[o1]\n"	\
		"vstr.32	s2, %[o2]\n"	\
		"vstr.32	s6, %[o3]\n"	\
		"vstr.32	s8, %[o4]\n"	\
		"vstr.32	s10, %[o5]\n"	\
		"vstr.32	s14, %[o6]\n"	\
		"vstr.32	s15, %[o7]\n"	\
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
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
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

#define NEON_D_IR8(op,name) NEON_D_IR8_0(#op,name)






//-----------------------------------------------------------------------------
// Double 32x2 Interleave src 4
//-----------------------------------------------------------------------------

#define NEON_D_IRS4_8(op) \
			op " d0, d8, d4 \n"		\
			op " d1, d8, d5 \n"		\
			op " d2, d8, d6 \n"		\
			op " d3, d8, d7 \n"		\
			op " d4, d8, d0 \n"		\
			op " d5, d8, d1 \n"		\
			op " d6, d8, d2 \n"		\
			op " d7, d8, d3 \n"



#define NEON_D_IRS4_0(op,name) \
static uint64_t NEON_D_IRS4_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d10, r2, r3\n"	\
		"vcvt.f32.s32	s16, s20\n"		\
		"vcvt.f32.s32	s17, s20\n"		\
		"vcvt.f32.s32	s18, s21\n"		\
		"vcvt.f32.s32	s19, s21\n"		\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_D_IRS4_8( op )	\
		NEON_D_IRS4_8( op )	\
		NEON_D_IRS4_8( op )	\
		NEON_D_IRS4_8( op )	\
		NEON_D_IRS4_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
		"vstr.32	s1, %[o1]\n"	\
		"vstr.32	s2, %[o2]\n"	\
		"vstr.32	s6, %[o3]\n"	\
		"vstr.32	s8, %[o4]\n"	\
		"vstr.32	s10, %[o5]\n"	\
		"vstr.32	s14, %[o6]\n"	\
		"vstr.32	s15, %[o7]\n"	\
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
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
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

#define NEON_D_IRS4(op,name) NEON_D_IRS4_0(#op,name)








//-----------------------------------------------------------------------------
// Quad 32x4 Interleave 1
//-----------------------------------------------------------------------------

#define NEON_Q_IR1_8(op) \
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"		\
			op " q0, q8, q9 \n"



#define NEON_Q_IR1_0(op,name) \
static uint64_t NEON_Q_IR1_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d1, r2, r3\n"	\
		"vcvt.f32.s32	s0, s2\n"	\
		"vcvt.f32.s32	s1, s3\n"	\
		"vdup.32	q8, d0[0]\n"	\
		"vdup.32	q9, d0[1]\n"	\
		"vmov	q0, q2\n"	\
		"vmov	q1, q2\n"	\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_Q_IR1_8( op )	\
		NEON_Q_IR1_8( op )	\
		NEON_Q_IR1_8( op )	\
		NEON_Q_IR1_8( op )	\
		NEON_Q_IR1_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","r0","r2","r3",	\
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	return	timer.Result();	\
}

#define NEON_Q_IR1(op,name) NEON_Q_IR1_0(#op,name)









//-----------------------------------------------------------------------------
// Quad 32x4 Interleave 8
//-----------------------------------------------------------------------------

#define NEON_Q_IR8_8(op) \
			op " q0, q8, q9 \n"		\
			op " q1, q8, q9 \n"		\
			op " q2, q8, q9 \n"		\
			op " q3, q8, q9 \n"		\
			op " q4, q8, q9 \n"		\
			op " q5, q8, q9 \n"		\
			op " q6, q8, q9 \n"		\
			op " q7, q8, q9 \n"



#define NEON_Q_IR8_0(op,name) \
static uint64_t NEON_Q_IR8_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d1, r2, r3\n"	\
		"vcvt.f32.s32	s0, s2\n"	\
		"vcvt.f32.s32	s1, s3\n"	\
		"vdup.32	q8, d0[0]\n"	\
		"vdup.32	q9, d0[1]\n"	\
		"vmov	q0, q2\n"	\
		"vmov	q1, q2\n"	\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_Q_IR8_8( op )	\
		NEON_Q_IR8_8( op )	\
		NEON_Q_IR8_8( op )	\
		NEON_Q_IR8_8( op )	\
		NEON_Q_IR8_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
		"vstr.32	s1, %[o1]\n"	\
		"vstr.32	s2, %[o2]\n"	\
		"vstr.32	s6, %[o3]\n"	\
		"vstr.32	s8, %[o4]\n"	\
		"vstr.32	s10, %[o5]\n"	\
		"vstr.32	s14, %[o6]\n"	\
		"vstr.32	s15, %[o7]\n"	\
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
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
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

#define NEON_Q_IR8(op,name) NEON_Q_IR8_0(#op,name)





//-----------------------------------------------------------------------------
// Quad 32x4 Interleave src 4
//-----------------------------------------------------------------------------

#define NEON_Q_IRS4_8(op) \
			op " q0, q8, q4 \n"		\
			op " q1, q8, q5 \n"		\
			op " q2, q8, q6 \n"		\
			op " q3, q8, q7 \n"		\
			op " q4, q8, q0 \n"		\
			op " q5, q8, q1 \n"		\
			op " q6, q8, q2 \n"		\
			op " q7, q8, q3 \n"



#define NEON_Q_IRS4_0(op,name) \
static uint64_t NEON_Q_IRS4_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d1, r2, r3\n"	\
		"vcvt.f32.s32	s0, s2\n"	\
		"vcvt.f32.s32	s1, s3\n"	\
		"vdup.32	q8, d0[0]\n"	\
		"vdup.32	q9, d0[1]\n"	\
		"vmov	q0, q2\n"	\
		"vmov	q1, q2\n"	\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_Q_IRS4_8( op )	\
		NEON_Q_IRS4_8( op )	\
		NEON_Q_IRS4_8( op )	\
		NEON_Q_IRS4_8( op )	\
		NEON_Q_IRS4_8( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
		"vstr.32	s1, %[o1]\n"	\
		"vstr.32	s2, %[o2]\n"	\
		"vstr.32	s6, %[o3]\n"	\
		"vstr.32	s8, %[o4]\n"	\
		"vstr.32	s10, %[o5]\n"	\
		"vstr.32	s14, %[o6]\n"	\
		"vstr.32	s15, %[o7]\n"	\
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
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
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

#define NEON_Q_IRS4(op,name) NEON_Q_IRS4_0(#op,name)



//-----------------------------------------------------------------------------
// Quad 32x4 Interleave 12
//-----------------------------------------------------------------------------

#define NEON_Q_IR12_12(op) \
			op " q0, q12, q13 \n"	\
			op " q1, q12, q13 \n"	\
			op " q2, q12, q13 \n"	\
			op " q3, q12, q13 \n"	\
			op " q4, q12, q13 \n"	\
			op " q5, q12, q13 \n"	\
			op " q6, q12, q13 \n"	\
			op " q7, q12, q13 \n"	\
			op " q8, q12, q13 \n"	\
			op " q9, q12, q13 \n"	\
			op " q10, q12, q13 \n"	\
			op " q11, q12, q13 \n"	\



#define NEON_Q_IR12_0(op,name) \
static uint64_t NEON_Q_IR12_##name( unsigned int LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	r0, #0  \n"		\
		"mov	r2, #2  \n"		\
		"mov	r3, #5  \n"		\
		"vmov	d1, r2, r3\n"	\
		"vcvt.f32.s32	s0, s2\n"	\
		"vcvt.f32.s32	s1, s3\n"	\
		"vdup.32	q12, d0[0]\n"	\
		"vdup.32	q13, d0[1]\n"	\
		"vmov	q0, q2\n"	\
		"vmov	q1, q2\n"	\
		"mov	r0, %[loop]\n"	\
	"1:\n"	\
		NEON_Q_IR12_12( op )	\
		NEON_Q_IR12_12( op )	\
		NEON_Q_IR12_12( op )	\
		NEON_Q_IR12_12( op )	\
		NEON_Q_IR12_12( op )	\
		"subs	r0, r0, #1\n"	\
		"bne	1b\n"	 	\
		"vstr.32	s0, %[o0]\n"	\
		"vstr.32	s1, %[o1]\n"	\
		"vstr.32	s2, %[o2]\n"	\
		"vstr.32	s6, %[o3]\n"	\
		"vstr.32	s8, %[o4]\n"	\
		"vstr.32	s10, %[o5]\n"	\
		"vstr.32	s14, %[o6]\n"	\
		"vstr.32	s15, %[o7]\n"	\
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
	"q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15" ); \
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

#define NEON_Q_IR12(op,name) NEON_Q_IR12_0(#op,name)



//-----------------------------------------------------------------------------
#endif // flCPU_NEON
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


static const char*	Instruction_Title[]= {

	"VFP fmuls (32bit x1) n8",
	"VFP fadds (32bit x1) n8",
	"VFP fmacs (32bit x1) n8",
	"VFP vfma.f32 (32bit x1) n8",

	"NEON vmul.f32 (32bit x2) n8",
	"NEON vadd.f32 (32bit x2) n8",
	"NEON vmla.f32 (32bit x2) n8",
	"NEON vfma.f32 (32bit x2) n8",

	"NEON vmul.f32 (32bit x4) n8",
	"NEON vadd.f32 (32bit x4) n8",
	"NEON vmla.f32 (32bit x4) n8",
	"NEON vfma.f32 (32bit x4) n8",


	"VFP fmuls (32bit x1) ns4",
	"VFP fadds (32bit x1) ns4",
	"VFP fmacs (32bit x1) ns4",
	"VFP vfma.f32 (32bit x1) ns4",

	"NEON vmul.f32 (32bit x2) ns4",
	"NEON vadd.f32 (32bit x2) ns4",
	"NEON vmla.f32 (32bit x2) ns4",
	"NEON vfma.f32 (32bit x2) ns4",

	"NEON vmul.f32 (32bit x4) ns4",
	"NEON vadd.f32 (32bit x4) ns4",
	"NEON vmla.f32 (32bit x4) ns4",
	"NEON vfma.f32 (32bit x4) ns4",

	"VFP fmuls (32bit x1) n1",
	"VFP fadds (32bit x1) n1",
	"VFP fmacs (32bit x1) n1",
	"VFP vfma.f32 (32bit x1) n1",

	"NEON vmul.f32 (32bit x2) n1",
	"NEON vadd.f32 (32bit x2) n1",
	"NEON vmla.f32 (32bit x2) n1",
	"NEON vfma.f32 (32bit x2) n1",

	"NEON vmul.f32 (32bit x4) n1",
	"NEON vadd.f32 (32bit x4) n1",
	"NEON vmla.f32 (32bit x4) n1",
	"NEON vfma.f32 (32bit x4) n1",


	"NEON vmul.f32 (32bit x4) n12",
	"NEON vadd.f32 (32bit x4) n12",
	"NEON vmla.f32 (32bit x4) n12",
	"NEON vfma.f32 (32bit x4) n12",

};




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_VFP_FMULS_IR8,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDS_IR8,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACS_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F32_IR8,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_D_VMUL_F32_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VADD_F32_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VMLA_F32_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_D_VFMA_F32_IR8,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_Q_VMUL_F32_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VADD_F32_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VMLA_F32_IR8,	PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_NEON_Q_VFMA_F32_IR8,	PER_LOOP_INST * 8, 8	);



	SetOp( RESULT_VFP_FMULS_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDS_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACS_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F32_IRS4,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_D_VMUL_F32_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VADD_F32_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VMLA_F32_IRS4,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_D_VFMA_F32_IRS4,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_Q_VMUL_F32_IRS4,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VADD_F32_IRS4,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VMLA_F32_IRS4,	PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_NEON_Q_VFMA_F32_IRS4,	PER_LOOP_INST * 8, 8	);


	SetOp( RESULT_VFP_FMULS_IR1,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADDS_IR1,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMACS_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_VFP_VFMA_F32_IR1,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_D_VMUL_F32_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VADD_F32_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_D_VMLA_F32_IR1,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_D_VFMA_F32_IR1,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_Q_VMUL_F32_IR1,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VADD_F32_IR1,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_Q_VMLA_F32_IR1,	PER_LOOP_INST * 8, 8	);
	SetOp( RESULT_NEON_Q_VFMA_F32_IR1,	PER_LOOP_INST * 8, 8	);



	SetOp( RESULT_NEON_Q_VMUL_F32_IR12,	PER_LOOP_INST_12 * 4, 4	);
	SetOp( RESULT_NEON_Q_VADD_F32_IR12,	PER_LOOP_INST_12 * 4, 4	);
	SetOp( RESULT_NEON_Q_VMLA_F32_IR12,	PER_LOOP_INST_12 * 8, 8	);
	SetOp( RESULT_NEON_Q_VFMA_F32_IR12,	PER_LOOP_INST_12 * 8, 8	);
}




unsigned int FloatTest::GetProgressStep() const
{
	return	RESULT_MAX;
}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



VFP_S_IR8( fmuls, fmuls_ir8 );
VFP_S_IR8( fadds, fadds_ir8 );
VFP_S_IR8( fmacs, fmacs_ir8 );
#if flCPU_VFPV4
VFP_S_IR8( vfma.f32, vfma_f32_ir8 );
#endif


//----------------------------------
#if flCPU_NEON
//----------------------------------
NEON_D_IR8( vmul.f32, vmul_f32_ir8 );
NEON_D_IR8( vadd.f32, vadd_f32_ir8 );
NEON_D_IR8( vmla.f32, vmla_f32_ir8 );
#if flCPU_VFPV4
NEON_D_IR8( vfma.f32, vfma_f32_ir8 );
#endif


NEON_Q_IR8( vmul.f32, vmul_f32_ir8 );
NEON_Q_IR8( vadd.f32, vadd_f32_ir8 );
NEON_Q_IR8( vmla.f32, vmla_f32_ir8 );
#if flCPU_VFPV4
NEON_Q_IR8( vfma.f32, vfma_f32_ir8 );
#endif
//----------------------------------
#endif
//----------------------------------





VFP_S_IRS4( fmuls, fmuls_irs4 );
VFP_S_IRS4( fadds, fadds_irs4 );
VFP_S_IRS4( fmacs, fmacs_irs4 );
#if flCPU_VFPV4
VFP_S_IRS4( vfma.f32, vfma_f32_irs4 );
#endif


//----------------------------------
#if flCPU_NEON
//----------------------------------
NEON_D_IRS4( vmul.f32, vmul_f32_irs4 );
NEON_D_IRS4( vadd.f32, vadd_f32_irs4 );
NEON_D_IRS4( vmla.f32, vmla_f32_irs4 );
#if flCPU_VFPV4
NEON_D_IRS4( vfma.f32, vfma_f32_irs4 );
#endif


NEON_Q_IRS4( vmul.f32, vmul_f32_irs4 );
NEON_Q_IRS4( vadd.f32, vadd_f32_irs4 );
NEON_Q_IRS4( vmla.f32, vmla_f32_irs4 );
#if flCPU_VFPV4
NEON_Q_IRS4( vfma.f32, vfma_f32_irs4 );
#endif
//----------------------------------
#endif
//----------------------------------




VFP_S_IR1( fmuls, fmuls_ir1 );
VFP_S_IR1( fadds, fadds_ir1 );
VFP_S_IR1( fmacs, fmacs_ir1 );
#if flCPU_VFPV4
VFP_S_IR1( vfma.f32, vfma_f32_ir1 );
#endif


//----------------------------------
#if flCPU_NEON
//----------------------------------
NEON_D_IR1( vmul.f32, vmul_f32_ir1 );
NEON_D_IR1( vadd.f32, vadd_f32_ir1 );
NEON_D_IR1( vmla.f32, vmla_f32_ir1 );
#if flCPU_VFPV4
NEON_D_IR1( vfma.f32, vfma_f32_ir1 );
#endif


NEON_Q_IR1( vmul.f32, vmul_f32_ir1 );
NEON_Q_IR1( vadd.f32, vadd_f32_ir1 );
NEON_Q_IR1( vmla.f32, vmla_f32_ir1 );
#if flCPU_VFPV4
NEON_Q_IR1( vfma.f32, vfma_f32_ir1 );
#endif
//----------------------------------
#endif
//----------------------------------





//----------------------------------
#if flCPU_NEON
//----------------------------------
NEON_Q_IR12( vmul.f32, vmul_f32_ir12 );
NEON_Q_IR12( vadd.f32, vadd_f32_ir12 );
NEON_Q_IR12( vmla.f32, vmla_f32_ir12 );
#if flCPU_VFPV4
NEON_Q_IR12( vfma.f32, vfma_f32_ir12 );
#endif
//----------------------------------
#endif
//----------------------------------



void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP loop=%d\n", Loop );

	//------------------------------------------------------
	// IR8
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMULS_IR8,	VFP_S_IR8_fmuls_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADDS_IR8,	VFP_S_IR8_fadds_ir8( Loop, 7.0f		) );
	Progress++;

	float	sum= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 10.0f;
	}

	SetResult( RESULT_VFP_FMACS_IR8,	VFP_S_IR8_fmacs_ir8( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F32_IR8,	VFP_S_IR8_vfma_f32_ir8( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------
//----------------------------------
#if flCPU_NEON
//----------------------------------

	SetResult( RESULT_NEON_D_VMUL_F32_IR8,	NEON_D_IR8_vmul_f32_ir8( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_D_VADD_F32_IR8,	NEON_D_IR8_vadd_f32_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_D_VMLA_F32_IR8,	NEON_D_IR8_vmla_f32_ir8( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_D_VFMA_F32_IR8,	NEON_D_IR8_vfma_f32_ir8( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_Q_VMUL_F32_IR8,	NEON_Q_IR8_vmul_f32_ir8( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_Q_VADD_F32_IR8,	NEON_Q_IR8_vadd_f32_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_Q_VMLA_F32_IR8,	NEON_Q_IR8_vmla_f32_ir8( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_Q_VFMA_F32_IR8,	NEON_Q_IR8_vfma_f32_ir8( Loop, sum ) );
#endif
	Progress++;

//----------------------------------
#endif
//----------------------------------
	//------------------------------------------------------




	//------------------------------------------------------
	// IRS4
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMULS_IRS4,	VFP_S_IRS4_fmuls_irs4( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADDS_IRS4,	VFP_S_IRS4_fadds_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FMACS_IRS4,	VFP_S_IRS4_fmacs_irs4( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F32_IRS4,	VFP_S_IRS4_vfma_f32_irs4( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------
//----------------------------------
#if flCPU_NEON
//----------------------------------

	SetResult( RESULT_NEON_D_VMUL_F32_IRS4,	NEON_D_IRS4_vmul_f32_irs4( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_D_VADD_F32_IRS4,	NEON_D_IRS4_vadd_f32_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_D_VMLA_F32_IRS4,	NEON_D_IRS4_vmla_f32_irs4( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_D_VFMA_F32_IRS4,	NEON_D_IRS4_vfma_f32_irs4( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_Q_VMUL_F32_IRS4,	NEON_Q_IRS4_vmul_f32_irs4( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_Q_VADD_F32_IRS4,	NEON_Q_IRS4_vadd_f32_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_Q_VMLA_F32_IRS4,	NEON_Q_IRS4_vmla_f32_irs4( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_Q_VFMA_F32_IRS4,	NEON_Q_IRS4_vfma_f32_irs4( Loop, sum ) );
#endif
	Progress++;

//----------------------------------
#endif
//----------------------------------
	//------------------------------------------------------


	//------------------------------------------------------
	// IR1
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMULS_IR1,	VFP_S_IR1_fmuls_ir1( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADDS_IR1,	VFP_S_IR1_fadds_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FMACS_IR1,	VFP_S_IR1_fmacs_ir1( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_VFP_VFMA_F32_IR1,	VFP_S_IR1_vfma_f32_ir1( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------
//----------------------------------
#if flCPU_NEON
//----------------------------------

	SetResult( RESULT_NEON_D_VMUL_F32_IR1,	NEON_D_IR1_vmul_f32_ir1( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_D_VADD_F32_IR1,	NEON_D_IR1_vadd_f32_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_D_VMLA_F32_IR1,	NEON_D_IR1_vmla_f32_ir1( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_D_VFMA_F32_IR1,	NEON_D_IR1_vfma_f32_ir1( Loop, sum ) );
#endif
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_Q_VMUL_F32_IR1,	NEON_Q_IR1_vmul_f32_ir1( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_Q_VADD_F32_IR1,	NEON_Q_IR1_vadd_f32_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_Q_VMLA_F32_IR1,	NEON_Q_IR1_vmla_f32_ir1( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_Q_VFMA_F32_IR1,	NEON_Q_IR1_vfma_f32_ir1( Loop, sum ) );
#endif
	Progress++;

//----------------------------------
#endif
//----------------------------------
	//------------------------------------------------------




	//------------------------------------------------------
	// IR12
	//------------------------------------------------------
//----------------------------------
#if flCPU_NEON
//----------------------------------

	SetResult( RESULT_NEON_Q_VMUL_F32_IR12,	NEON_Q_IR12_vmul_f32_ir12( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_Q_VADD_F32_IR12,	NEON_Q_IR12_vadd_f32_ir12( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_Q_VMLA_F32_IR12,	NEON_Q_IR12_vmla_f32_ir12( Loop, sum ) );
	Progress++;

#if flCPU_VFPV4
	SetResult( RESULT_NEON_Q_VFMA_F32_IR12,	NEON_Q_IR12_vfma_f32_ir12( Loop, sum ) );
#endif
	Progress++;

//----------------------------------
#endif
//----------------------------------
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
		return	"VFP/NEON (SP fp)";
	}else{
		return	"VFP/NEON (SP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // ARM
//-----------------------------------------------------------------------------

