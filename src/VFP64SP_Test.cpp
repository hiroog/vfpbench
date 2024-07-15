// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP64SP_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_ARM64
//-----------------------------------------------------------------------------

#if defined(__ARM_FEATURE_SVE2) && __ARM_FEATURE_SVE2
# include	<arm_sve.h>
# define	USE_ARM_SVE2	1
#endif

using namespace flatlib;
using system::CPUFeature;

namespace VFP64SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_OS_ANDROID
# define	USE_ANDROID_GCC		1
#else
# define	USE_ANDROID_GCC		0
#endif


/*
	w0-w31	32bit
	x0-x31	64bit

	v0-v31	128bit
	q0-q31	128bit
	d0-d31	64bit
	s0-s31	32bit
	h0-h31	16bit
	b0-b31	8bit
*/



typedef	unsigned long	CounterType;


//=============================================================================
// VFP
//=============================================================================

static void check_result( float ret, float ans, const char* name )
{
	FL_LOG( "check: %s : %f %f\n", name, ret, ans );
	FL_ASSERT( ret == ans );
}

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define VFP_REG_CLEAR() \
		"eor	v0.16B, v0.16B, v0.16B\n"		\
		"orr	v1.16b, v0.16b, v0.16b\n"	\
		"orr	v2.16b, v0.16b, v0.16b\n"	\
		"orr	v3.16b, v0.16b, v0.16b\n"	\
		"orr	v4.16b, v0.16b, v0.16b\n"	\
		"orr	v5.16b, v0.16b, v0.16b\n"	\
		"orr	v6.16b, v0.16b, v0.16b\n"	\
		"orr	v7.16b, v0.16b, v0.16b\n"	\
		"orr	v8.16b, v0.16b, v0.16b\n"	\
		"orr	v9.16b, v0.16b, v0.16b\n"	\
		"orr	v10.16b, v0.16b, v0.16b\n"	\
		"orr	v11.16b, v0.16b, v0.16b\n"	\
		"orr	v12.16b, v0.16b, v0.16b\n"	\
		"orr	v13.16b, v0.16b, v0.16b\n"	\
		"orr	v14.16b, v0.16b, v0.16b\n"	\
		"orr	v15.16b, v0.16b, v0.16b\n"	\

//-----------------
#else
//-----------------

#define VFP_REG_CLEAR() \
		"eor		v0.16B, v0.16B, v0.16B\n"		\
		"orr.16b	v1, v0, v0\n"	\
		"orr.16b	v2, v0, v0\n"	\
		"orr.16b	v3, v0, v0\n"	\
		"orr.16b	v4, v0, v0\n"	\
		"orr.16b	v5, v0, v0\n"	\
		"orr.16b	v6, v0, v0\n"	\
		"orr.16b	v7, v0, v0\n"	\
		"orr.16b	v8, v0, v0\n"	\
		"orr.16b	v9, v0, v0\n"	\
		"orr.16b	v10, v0, v0\n"	\
		"orr.16b	v11, v0, v0\n"	\
		"orr.16b	v12, v0, v0\n"	\
		"orr.16b	v13, v0, v0\n"	\
		"orr.16b	v14, v0, v0\n"	\
		"orr.16b	v15, v0, v0\n"	\

//-----------------
#endif
//-----------------



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
static uint64_t VFP_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		VFP_S_IR1_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
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
static uint64_t VFP_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		VFP_S_IR8_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define VFP_S_IR8(op,name) VFP_S_IR8_0(#op,name)












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
static uint64_t VFP_S_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		VFP_S_IRS4_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );*/	\
	return	timer.Result();	\
}

#define VFP_S_IRS4(op,name) VFP_S_IRS4_0(#op,name)












//=============================================================================
// MAD
//=============================================================================


//-----------------
#if USE_ANDROID_GCC
//-----------------

#define MAD_REG_CLEAR() \
		"eor	v0.16B, v0.16B, v0.16B\n"		\
		"orr	v1.16b, v0.16b, v0.16b\n"	\
		"orr	v2.16b, v0.16b, v0.16b\n"	\
		"orr	v3.16b, v0.16b, v0.16b\n"	\
		"orr	v4.16b, v0.16b, v0.16b\n"	\
		"orr	v5.16b, v0.16b, v0.16b\n"	\
		"orr	v6.16b, v0.16b, v0.16b\n"	\
		"orr	v7.16b, v0.16b, v0.16b\n"	\
		"orr	v8.16b, v0.16b, v0.16b\n"	\
		"orr	v9.16b, v0.16b, v0.16b\n"	\
		"orr	v10.16b, v0.16b, v0.16b\n"	\
		"orr	v11.16b, v0.16b, v0.16b\n"	\
		"orr	v12.16b, v0.16b, v0.16b\n"	\
		"orr	v13.16b, v0.16b, v0.16b\n"	\
		"orr	v14.16b, v0.16b, v0.16b\n"	\
		"orr	v15.16b, v0.16b, v0.16b\n"	\

//-----------------
#else
//-----------------

#define MAD_REG_CLEAR() \
		"eor		v0.16B, v0.16B, v0.16B\n"		\
		"orr.16b	v1, v0, v0\n"	\
		"orr.16b	v2, v0, v0\n"	\
		"orr.16b	v3, v0, v0\n"	\
		"orr.16b	v4, v0, v0\n"	\
		"orr.16b	v5, v0, v0\n"	\
		"orr.16b	v6, v0, v0\n"	\
		"orr.16b	v7, v0, v0\n"	\
		"orr.16b	v8, v0, v0\n"	\
		"orr.16b	v9, v0, v0\n"	\
		"orr.16b	v10, v0, v0\n"	\
		"orr.16b	v11, v0, v0\n"	\
		"orr.16b	v12, v0, v0\n"	\
		"orr.16b	v13, v0, v0\n"	\
		"orr.16b	v14, v0, v0\n"	\
		"orr.16b	v15, v0, v0\n"	\

//-----------------
#endif
//-----------------



//-----------------------------------------------------------------------------
// Single Interleave 1 MAD
//-----------------------------------------------------------------------------

#define MAD_S_IR1_8(op) \
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"		\
			op " s0, s8, s9, s0 \n"



#define MAD_S_IR1_0(op,name) \
static uint64_t MAD_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		MAD_S_IR1_8( op )	\
		MAD_S_IR1_8( op )	\
		MAD_S_IR1_8( op )	\
		MAD_S_IR1_8( op )	\
		MAD_S_IR1_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	return	timer.Result();	\
}

#define MAD_S_IR1(op,name) MAD_S_IR1_0(#op,name)










//----------------------------
// Single Interleave 8 MAD
//----------------------------

#define MAD_S_IR8_8(op) \
			op " s0, s8, s9, s0 \n"		\
			op " s1, s8, s9, s1 \n"		\
			op " s2, s8, s9, s2 \n"		\
			op " s3, s8, s9, s3 \n"		\
			op " s4, s8, s9, s4 \n"		\
			op " s5, s8, s9, s5 \n"		\
			op " s6, s8, s9, s6 \n"		\
			op " s7, s8, s9, s7 \n"



#define MAD_S_IR8_0(op,name) \
static uint64_t MAD_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		MAD_S_IR8_8( op )	\
		MAD_S_IR8_8( op )	\
		MAD_S_IR8_8( op )	\
		MAD_S_IR8_8( op )	\
		MAD_S_IR8_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define MAD_S_IR8(op,name) MAD_S_IR8_0(#op,name)






//-----------------------------------------------------------------------------
// Single Interleave src 4 MAD
//-----------------------------------------------------------------------------

#define MAD_S_IRS4_8(op) \
			op " s0, s8, s4, s0 \n"		\
			op " s1, s8, s5, s1 \n"		\
			op " s2, s8, s6, s2 \n"		\
			op " s3, s8, s7, s3 \n"		\
			op " s4, s8, s0, s4 \n"		\
			op " s5, s8, s1, s5 \n"		\
			op " s6, s8, s2, s6 \n"		\
			op " s7, s8, s3, s7 \n"



#define MAD_S_IRS4_0(op,name) \
static uint64_t MAD_S_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	s8, w2\n"		\
		"scvtf	s9, w3\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		MAD_S_IRS4_8( op )	\
		MAD_S_IRS4_8( op )	\
		MAD_S_IRS4_8( op )	\
		MAD_S_IRS4_8( op )	\
		MAD_S_IRS4_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );*/	\
	return	timer.Result();	\
}

#define MAD_S_IRS4(op,name) MAD_S_IRS4_0(#op,name)

















//=============================================================================
// NEON
//=============================================================================

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define NEON_REG_CLEAR() \
		"eor	v0.16B, v0.16B, v0.16B\n"		\
		"orr	v1.16b, v0.16b, v0.16b\n"	\
		"orr	v2.16b, v0.16b, v0.16b\n"	\
		"orr	v3.16b, v0.16b, v0.16b\n"	\
		"orr	v4.16b, v0.16b, v0.16b\n"	\
		"orr	v5.16b, v0.16b, v0.16b\n"	\
		"orr	v6.16b, v0.16b, v0.16b\n"	\
		"orr	v7.16b, v0.16b, v0.16b\n"	\
		"orr	v8.16b, v0.16b, v0.16b\n"	\
		"orr	v9.16b, v0.16b, v0.16b\n"	\
		"orr	v10.16b, v0.16b, v0.16b\n"	\
		"orr	v11.16b, v0.16b, v0.16b\n"	\
		"orr	v12.16b, v0.16b, v0.16b\n"	\
		"orr	v13.16b, v0.16b, v0.16b\n"	\
		"orr	v14.16b, v0.16b, v0.16b\n"	\
		"orr	v15.16b, v0.16b, v0.16b\n"	\

//-----------------
#else
//-----------------

#define NEON_REG_CLEAR() \
		"eor		v0.16B, v0.16B, v0.16B\n"		\
		"orr.16b	v1, v0, v0\n"	\
		"orr.16b	v2, v0, v0\n"	\
		"orr.16b	v3, v0, v0\n"	\
		"orr.16b	v4, v0, v0\n"	\
		"orr.16b	v5, v0, v0\n"	\
		"orr.16b	v6, v0, v0\n"	\
		"orr.16b	v7, v0, v0\n"	\
		"orr.16b	v8, v0, v0\n"	\
		"orr.16b	v9, v0, v0\n"	\
		"orr.16b	v10, v0, v0\n"	\
		"orr.16b	v11, v0, v0\n"	\
		"orr.16b	v12, v0, v0\n"	\
		"orr.16b	v13, v0, v0\n"	\
		"orr.16b	v14, v0, v0\n"	\
		"orr.16b	v15, v0, v0\n"	\

//-----------------
#endif
//-----------------





//-----------------------------------------------------------------------------
// Vector 32x2 32x4 Interleave 1
//-----------------------------------------------------------------------------

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define NEON_V_IR1_8(op,size) \
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v0" size ", v8" size ", v9" size " \n"



#define NEON_V_IR1_0(op,size,name) \
static uint64_t NEON_V_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()		\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR1_8( op, size )	\
		NEON_V_IR1_8( op, size )	\
		NEON_V_IR1_8( op, size )	\
		NEON_V_IR1_8( op, size )	\
		NEON_V_IR1_8( op, size )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR1(op,size,name) NEON_V_IR1_0(#op,#size,name)


//-----------------
#else
//-----------------

#define NEON_V_IR1_8(op) \
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"		\
			op " v0, v8, v9 \n"



#define NEON_V_IR1_0(op,name) \
static uint64_t NEON_V_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()		\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR1_8( op )	\
		NEON_V_IR1_8( op )	\
		NEON_V_IR1_8( op )	\
		NEON_V_IR1_8( op )	\
		NEON_V_IR1_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR1(op,name) NEON_V_IR1_0(#op,name)

//-----------------
#endif
//-----------------



//-----------------------------------------------------------------------------
// Vector 32x2 32x4 Interleave 8
//-----------------------------------------------------------------------------

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define NEON_V_IR8_8(op,size) \
			op " v0" size ", v8" size ", v9" size " \n"		\
			op " v1" size ", v8" size ", v9" size " \n"		\
			op " v2" size ", v8" size ", v9" size " \n"		\
			op " v3" size ", v8" size ", v9" size " \n"		\
			op " v4" size ", v8" size ", v9" size " \n"		\
			op " v5" size ", v8" size ", v9" size " \n"		\
			op " v6" size ", v8" size ", v9" size " \n"		\
			op " v7" size ", v8" size ", v9" size " \n"


#define NEON_V_IR8_0(op,size,name) \
static uint64_t NEON_V_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR8_8( op, size )	\
		NEON_V_IR8_8( op, size )	\
		NEON_V_IR8_8( op, size )	\
		NEON_V_IR8_8( op, size )	\
		NEON_V_IR8_8( op, size )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR8(op,size,name) NEON_V_IR8_0(#op,#size,name)


//-----------------
#else
//-----------------

#define NEON_V_IR8_8(op) \
			op " v0, v8, v9 \n"		\
			op " v1, v8, v9 \n"		\
			op " v2, v8, v9 \n"		\
			op " v3, v8, v9 \n"		\
			op " v4, v8, v9 \n"		\
			op " v5, v8, v9 \n"		\
			op " v6, v8, v9 \n"		\
			op " v7, v8, v9 \n"




#define NEON_V_IR8_0(op,name) \
static uint64_t NEON_V_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR8_8( op )	\
		NEON_V_IR8_8( op )	\
		NEON_V_IR8_8( op )	\
		NEON_V_IR8_8( op )	\
		NEON_V_IR8_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR8(op,name) NEON_V_IR8_0(#op,name)

//-----------------
#endif
//-----------------




//-----------------------------------------------------------------------------
// Vector 32x2 32x4 Interleave src 4
//-----------------------------------------------------------------------------

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define NEON_V_IRS4_8(op,size) \
			op " v0" size ", v8" size ", v4" size " \n"		\
			op " v1" size ", v8" size ", v5" size " \n"		\
			op " v2" size ", v8" size ", v6" size " \n"		\
			op " v3" size ", v8" size ", v7" size " \n"		\
			op " v4" size ", v8" size ", v0" size " \n"		\
			op " v5" size ", v8" size ", v1" size " \n"		\
			op " v6" size ", v8" size ", v2" size " \n"		\
			op " v7" size ", v8" size ", v3" size " \n"



#define NEON_V_IRS4_0(op,size,name) \
static uint64_t NEON_V_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IRS4_8( op, size )	\
		NEON_V_IRS4_8( op, size )	\
		NEON_V_IRS4_8( op, size )	\
		NEON_V_IRS4_8( op, size )	\
		NEON_V_IRS4_8( op, size )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );*/	\
	return	timer.Result();	\
}

#define NEON_V_IRS4(op,size,name) NEON_V_IRS4_0(#op,#size,name)

//-----------------
#else
//-----------------

#define NEON_V_IRS4_8(op) \
			op " v0, v8, v4 \n"		\
			op " v1, v8, v5 \n"		\
			op " v2, v8, v6 \n"		\
			op " v3, v8, v7 \n"		\
			op " v4, v8, v0 \n"		\
			op " v5, v8, v1 \n"		\
			op " v6, v8, v2 \n"		\
			op " v7, v8, v3 \n"



#define NEON_V_IRS4_0(op,name) \
static uint64_t NEON_V_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v8.4s, w2\n"	\
		"scvtf	v8.4s, v8.4s\n"		\
		"dup	v9.4s, w3\n"	\
		"scvtf	v9.4s, v9.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IRS4_8( op )	\
		NEON_V_IRS4_8( op )	\
		NEON_V_IRS4_8( op )	\
		NEON_V_IRS4_8( op )	\
		NEON_V_IRS4_8( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );*/	\
	return	timer.Result();	\
}

#define NEON_V_IRS4(op,name) NEON_V_IRS4_0(#op,name)


//-----------------
#endif
//-----------------




//-----------------------------------------------------------------------------
// Vector 32x2 32x4 Interleave 12
//-----------------------------------------------------------------------------

//-----------------
#if USE_ANDROID_GCC
//-----------------

#define NEON_V_IR12_12(op,size) \
			op " v0" size ", v12" size ", v13" size " \n"	\
			op " v1" size ", v12" size ", v13" size " \n"	\
			op " v2" size ", v12" size ", v13" size " \n"	\
			op " v3" size ", v12" size ", v13" size " \n"	\
			op " v4" size ", v12" size ", v13" size " \n"	\
			op " v5" size ", v12" size ", v13" size " \n"	\
			op " v6" size ", v12" size ", v13" size " \n"	\
			op " v7" size ", v12" size ", v13" size " \n"	\
			op " v8" size ", v12" size ", v13" size " \n"	\
			op " v9" size ", v12" size ", v13" size " \n"	\
			op " v10" size ", v12" size ", v13" size " \n"	\
			op " v11" size ", v12" size ", v13" size " \n"	\



#define NEON_V_IR12_0(op,size,name) \
static uint64_t NEON_V_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v12.4s, w2\n"	\
		"scvtf	v12.4s, v12.4s\n"		\
		"dup	v13.4s, w3\n"	\
		"scvtf	v13.4s, v13.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR12_12( op, size )	\
		NEON_V_IR12_12( op, size )	\
		NEON_V_IR12_12( op, size )	\
		NEON_V_IR12_12( op, size )	\
		NEON_V_IR12_12( op, size )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR12(op,size,name) NEON_V_IR12_0(#op,#size,name)

//-----------------
#else
//-----------------

#define NEON_V_IR12_12(op) \
			op " v0, v12, v13 \n"	\
			op " v1, v12, v13 \n"	\
			op " v2, v12, v13 \n"	\
			op " v3, v12, v13 \n"	\
			op " v4, v12, v13 \n"	\
			op " v5, v12, v13 \n"	\
			op " v6, v12, v13 \n"	\
			op " v7, v12, v13 \n"	\
			op " v8, v12, v13 \n"	\
			op " v9, v12, v13 \n"	\
			op " v10, v12, v13 \n"	\
			op " v11, v12, v13 \n"	\



#define NEON_V_IR12_0(op,name) \
static uint64_t NEON_V_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v12.4s, w2\n"	\
		"scvtf	v12.4s, v12.4s\n"		\
		"dup	v13.4s, w3\n"	\
		"scvtf	v13.4s, v13.4s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		NEON_V_IR12_12( op )	\
		NEON_V_IR12_12( op )	\
		NEON_V_IR12_12( op )	\
		NEON_V_IR12_12( op )	\
		NEON_V_IR12_12( op )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s0, %[o0]\n"	\
		"str	s1, %[o1]\n"	\
		"str	s2, %[o2]\n"	\
		"str	s3, %[o3]\n"	\
		"str	s4, %[o4]\n"	\
		"str	s5, %[o5]\n"	\
		"str	s6, %[o6]\n"	\
		"str	s7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define NEON_V_IR12(op,name) NEON_V_IR12_0(#op,name)

//-----------------
#endif
//-----------------




//=============================================================================
// SVE
//=============================================================================

//-----------------
#if USE_ARM_SVE2
//-----------------

#define SVE_REG_CLEAR() \
		"dup	z0.b, #0\n"		\
		"dup	z1.b, #0\n"		\
		"dup	z2.b, #0\n"		\
		"dup	z3.b, #0\n"		\
		"dup	z4.b, #0\n"		\
		"dup	z5.b, #0\n"		\
		"dup	z6.b, #0\n"		\
		"dup	z7.b, #0\n"		\
		"dup	z8.b, #0\n"		\
		"dup	z9.b, #0\n"		\
		"dup	z10.b, #0\n"		\
		"dup	z11.b, #0\n"		\
		"dup	z12.b, #0\n"		\
		"dup	z13.b, #0\n"		\
		"dup	z14.b, #0\n"		\
		"dup	z15.b, #0\n"		\
		"ptrue	p0.s\n"		\


//-----------------------------------------------------------------------------
// Vector 32bit Interleave 12
//-----------------------------------------------------------------------------

#define SVE_V_IR12_12(op,size,pred) \
			op " z4"  size pred ", z2" size ", z3" size " \n"	\
			op " z5"  size pred ", z2" size ", z3" size " \n"	\
			op " z6"  size pred ", z2" size ", z3" size " \n"	\
			op " z7"  size pred ", z2" size ", z3" size " \n"	\
			op " z8"  size pred ", z2" size ", z3" size " \n"	\
			op " z9"  size pred ", z2" size ", z3" size " \n"	\
			op " z10" size pred ", z2" size ", z3" size " \n"	\
			op " z11" size pred ", z2" size ", z3" size " \n"	\
			op " z12" size pred ", z2" size ", z3" size " \n"	\
			op " z13" size pred ", z2" size ", z3" size " \n"	\
			op " z14" size pred ", z2" size ", z3" size " \n"	\
			op " z15" size pred ", z2" size ", z3" size " \n"	\


#define SVE_V_IR12_0(op,size,name,pred) \
static uint64_t SVE_V_IR12_##name( CounterType LoopCount, float answer ) \
{ \
	float	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		SVE_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	z2.s, w2\n"	\
		"scvtf	z2.s, p0/m, z2.s\n"		\
		"dup	z3.s, w3\n"	\
		"scvtf	z3.s, p0/m, z3.s\n"		\
		"mov	x0, %[loop]\n"	\
		"mov	w0, w0\n"	\
	"1:\n"	\
		SVE_V_IR12_12( op, size, pred )	\
		SVE_V_IR12_12( op, size, pred )	\
		SVE_V_IR12_12( op, size, pred )	\
		SVE_V_IR12_12( op, size, pred )	\
		SVE_V_IR12_12( op, size, pred )	\
		"subs	w0, w0, #1\n"	\
		"b.ne	1b\n"	 	\
		"str	s4, %[o0]\n"	\
		"str	s5, %[o1]\n"	\
		"str	s6, %[o2]\n"	\
		"str	s7, %[o3]\n"	\
		"str	s8, %[o4]\n"	\
		"str	s9, %[o5]\n"	\
		"str	s10, %[o6]\n"	\
		"str	s11, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3","p0",	\
	"z0","z1","z2","z3","z4","z5","z6","z7","z8","z9","z10","z11","z12","z13","z14","z15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	check_result( ret0, answer, #name );	\
	check_result( ret1, answer, #name );	\
	check_result( ret2, answer, #name );	\
	check_result( ret3, answer, #name );	\
	check_result( ret4, answer, #name );	\
	check_result( ret5, answer, #name );	\
	check_result( ret6, answer, #name );	\
	check_result( ret7, answer, #name );	\
	return	timer.Result();	\
}

#define SVE_V_IR12(op,size,name,pred) SVE_V_IR12_0(#op,#size,name,pred)




//-----------------
#endif
//-----------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


static const char*	Instruction_Title[]= {

	"FPU fmul (32bit x1) n8",
	"FPU fadd (32bit x1) n8",
	"FPU fmadd (32bit x1) n8",

	"NEON fmul.2s (32bit x2) n8",
	"NEON fadd.2s (32bit x2) n8",
	"NEON fmla.2s (32bit x2) n8",

	"NEON fmul.4s (32bit x4) n8",
	"NEON fadd.4s (32bit x4) n8",
	"NEON fmla.4s (32bit x4) n8",


	"FPU fmul (32bit x1) ns4",
	"FPU fadd (32bit x1) ns4",
	"FPU fmadd (32bit x1) ns4",

	"NEON fmul.2s (32bit x2) ns4",
	"NEON fadd.2s (32bit x2) ns4",
	"NEON fmla.2s (32bit x2) ns4",

	"NEON fmul.4s (32bit x4) ns4",
	"NEON fadd.4s (32bit x4) ns4",
	"NEON fmla.4s (32bit x4) ns4",


	"FPU fmul (32bit x1) n1",
	"FPU fadd (32bit x1) n1",
	"FPU fmadd (32bit x1) n1",

	"NEON fmul.2s (32bit x2) n1",
	"NEON fadd.2s (32bit x2) n1",
	"NEON fmla.2s (32bit x2) n1",

	"NEON fmul.4s (32bit x4) n1",
	"NEON fadd.4s (32bit x4) n1",
	"NEON fmla.4s (32bit x4) n1",


	"NEON fmul.4s (32bit x4) n12",
	"NEON fadd.4s (32bit x4) n12",
	"NEON fmla.4s (32bit x4) n12",


	"SVE fmul.s (32bit xN) n12",
	"SVE fadd.s (32bit xN) n12",
	"SVE fmla.s (32bit xN) n12",
};



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	FL_ASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_VFP_FMUL_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IR8,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_S2_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_S2_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_S2_IR8,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_FMUL_S4_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FADD_S4_IR8,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FMLA_S4_IR8,	PER_LOOP_INST * 8, 8	);



	SetOp( RESULT_VFP_FMUL_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IRS4,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_S2_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_S2_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_S2_IRS4,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_FMUL_S4_IRS4,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FADD_S4_IRS4,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FMLA_S4_IRS4,	PER_LOOP_INST * 8, 8	);



	SetOp( RESULT_VFP_FMUL_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IR1,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_S2_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_S2_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_S2_IR1,	PER_LOOP_INST * 4, 4	);

	SetOp( RESULT_NEON_FMUL_S4_IR1,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FADD_S4_IR1,	PER_LOOP_INST * 4, 4	);
	SetOp( RESULT_NEON_FMLA_S4_IR1,	PER_LOOP_INST * 8, 8	);



	SetOp( RESULT_NEON_FMUL_S4_IR12,	PER_LOOP_INST_12 * 4, 4	);
	SetOp( RESULT_NEON_FADD_S4_IR12,	PER_LOOP_INST_12 * 4, 4	);
	SetOp( RESULT_NEON_FMLA_S4_IR12,	PER_LOOP_INST_12 * 8, 8	);



#if USE_ARM_SVE2
	if( Info.HasInstructionSet( CPUFeature::ARM_SVE2 ) ){
		uint32_t	vector_length= (uint32_t)( svcntw() );
		FL_LOG( "single vlen=%d\n", vector_length );
		SetOp( RESULT_SVE_FMUL_S_IR12,	PER_LOOP_INST_12 * vector_length, vector_length	);
		SetOp( RESULT_SVE_FADD_S_IR12,	PER_LOOP_INST_12 * vector_length, vector_length	);
		SetOp( RESULT_SVE_FMLA_S_IR12,	PER_LOOP_INST_12 * vector_length*2, vector_length*2	);
	}else
#endif

	{
		SetOp( RESULT_SVE_FMUL_S_IR12,	PER_LOOP_INST_12 * 4, 4	);
		SetOp( RESULT_SVE_FADD_S_IR12,	PER_LOOP_INST_12 * 4, 4	);
		SetOp( RESULT_SVE_FMLA_S_IR12,	PER_LOOP_INST_12 * 8, 8	);
	}
}




unsigned int FloatTest::GetProgressStep() const
{
	return	RESULT_MAX;
}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



VFP_S_IR8( fmul,  fmul_ir8 );
VFP_S_IR8( fadd,  fadd_ir8 );
MAD_S_IR8( fmadd, fmadd_ir8 );


//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IR8( fmul, .2s, fmul_2s_ir8 );
NEON_V_IR8( fadd, .2s, fadd_2s_ir8 );
NEON_V_IR8( fmla, .2s, fmla_2s_ir8 );


NEON_V_IR8( fmul, .4s, fmul_4s_ir8 );
NEON_V_IR8( fadd, .4s, fadd_4s_ir8 );
NEON_V_IR8( fmla, .4s, fmla_4s_ir8 );

//-----------------
#else
//-----------------

NEON_V_IR8( fmul.2s, fmul_2s_ir8 );
NEON_V_IR8( fadd.2s, fadd_2s_ir8 );
NEON_V_IR8( fmla.2s, fmla_2s_ir8 );


NEON_V_IR8( fmul.4s, fmul_4s_ir8 );
NEON_V_IR8( fadd.4s, fadd_4s_ir8 );
NEON_V_IR8( fmla.4s, fmla_4s_ir8 );

//-----------------
#endif
//-----------------



VFP_S_IRS4( fmul,  fmul_irs4 );
VFP_S_IRS4( fadd,  fadd_irs4 );
MAD_S_IRS4( fmadd, fmadd_irs4 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IRS4( fmul, .2s, fmul_2s_irs4 );
NEON_V_IRS4( fadd, .2s, fadd_2s_irs4 );
NEON_V_IRS4( fmla, .2s, fmla_2s_irs4 );


NEON_V_IRS4( fmul, .4s, fmul_4s_irs4 );
NEON_V_IRS4( fadd, .4s, fadd_4s_irs4 );
NEON_V_IRS4( fmla, .4s, fmla_4s_irs4 );

//-----------------
#else
//-----------------

NEON_V_IRS4( fmul.2s, fmul_2s_irs4 );
NEON_V_IRS4( fadd.2s, fadd_2s_irs4 );
NEON_V_IRS4( fmla.2s, fmla_2s_irs4 );


NEON_V_IRS4( fmul.4s, fmul_4s_irs4 );
NEON_V_IRS4( fadd.4s, fadd_4s_irs4 );
NEON_V_IRS4( fmla.4s, fmla_4s_irs4 );

//-----------------
#endif
//-----------------



VFP_S_IR1( fmul,  fmul_ir1 );
VFP_S_IR1( fadd,  fadd_ir1 );
MAD_S_IR1( fmadd, fmadd_ir1 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IR1( fmul, .2s, fmul_2s_ir1 );
NEON_V_IR1( fadd, .2s, fadd_2s_ir1 );
NEON_V_IR1( fmla, .2s, fmla_2s_ir1 );


NEON_V_IR1( fmul, .4s, fmul_4s_ir1 );
NEON_V_IR1( fadd, .4s, fadd_4s_ir1 );
NEON_V_IR1( fmla, .4s, fmla_4s_ir1 );



NEON_V_IR12( fmul, .4s, fmul_4s_ir12 );
NEON_V_IR12( fadd, .4s, fadd_4s_ir12 );
NEON_V_IR12( fmla, .4s, fmla_4s_ir12 );

//-----------------
#else
//-----------------

NEON_V_IR1( fmul.2s, fmul_2s_ir1 );
NEON_V_IR1( fadd.2s, fadd_2s_ir1 );
NEON_V_IR1( fmla.2s, fmla_2s_ir1 );


NEON_V_IR1( fmul.4s, fmul_4s_ir1 );
NEON_V_IR1( fadd.4s, fadd_4s_ir1 );
NEON_V_IR1( fmla.4s, fmla_4s_ir1 );



NEON_V_IR12( fmul.4s, fmul_4s_ir12 );
NEON_V_IR12( fadd.4s, fadd_4s_ir12 );
NEON_V_IR12( fmla.4s, fmla_4s_ir12 );

//-----------------
#endif
//-----------------


//-----------------
#if USE_ARM_SVE2
//-----------------

SVE_V_IR12( fmul, .s, fmul_s_ir12, " " );
SVE_V_IR12( fadd, .s, fadd_s_ir12, " " );
SVE_V_IR12( fmla, .s, fmla_s_ir12, ", p0/m" );

//-----------------
#endif
//-----------------



void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP64 loop=%d\n", Loop );


#if 0
{
struct fpack {
	float	x, y, z, w;
};
svuint32x4_t	ret1;
svuint32x4_t	ret2;
float	ret0= 0.0f;
fpack	src0;
fpack	src1;
src0.x= 1.0f;
src0.y= 2.0f;
src0.z= 3.0f;
src0.w= 4.0f;
src1.x= 2.0f;
src1.y= 3.0f;
src1.z= 4.0f;
src1.w= 5.0f;
__asm__ __volatile__(

"dup  z0.b,#0\n"
"dup  z1.b,#0\n"
"dup  z2.b,#0\n"
"mov	w2, #99  \n"	
"mov	w3, #5  \n"	
"dup	z4.s, w2\n"
"ptrue p0.s\n"
"scvtf	z4.s, p0/m, z4.s\n"	
"ld1w {z1.s}, p0/z, %[i0]\n"
"ld1w {z2.s}, p0/z, %[i1]\n"
"fmla z0.s, p0/m, z1.s, z2.s\n"
"str  z0,%[o1]\n"
"str  z4,%[o2]\n"
: [o0]"=m"(ret0), [o1]"=m"(ret1), [o2]"=m"(ret2)
: [loop]"r"(LoopCount), [i0]"m"(src0), [i1]"m"(src1)
: "cc","x0","w2","w3","p0",
  "z0",
  "z1",
  "z2",
  "z3",
  "z4",
  "z5",
  "z6",
  "z7",
  "z8",
  "z9",
  "z10",
  "z11",
  "z12",
  "z13",
  "z14",
  "z15" );


for( int a= 0 ; a< 16 ; a++ ){
	uint8_t	data= ((const uint8_t*)(&ret1))[a];
FL_PRINT( "%d: %02x\n", a, (int)data );
}
for( int a= 0 ; a< 4 ; a++ ){
	float	data= ((const float*)(&ret1))[a];
FL_PRINT( "%d: %f\n", a, data );
}
for( int a= 0 ; a< 4 ; a++ ){
	float	data= ((const float*)(&ret2))[a];
FL_PRINT( "%d: %f\n", a, data );
}

}
#endif


	float	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum_2+= 10.0f;
		__asm__ __volatile__( "nop\n": : : "cc", "x0" );
	}

	//------------------------------------------------------
	// IR8
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IR8,		VFP_S_IR8_fmul_ir8( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADD_IR8,		VFP_S_IR8_fadd_ir8( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FMADD_IR8,	MAD_S_IR8_fmadd_ir8( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S2_IR8,	NEON_V_IR8_fmul_2s_ir8( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S2_IR8,	NEON_V_IR8_fadd_2s_ir8( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S2_IR8,	NEON_V_IR8_fmla_2s_ir8( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR8,	NEON_V_IR8_fmul_4s_ir8( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S4_IR8,	NEON_V_IR8_fadd_4s_ir8( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S4_IR8,	NEON_V_IR8_fmla_4s_ir8( Loop, sum_2 ) );
	Progress.Increment();


	//------------------------------------------------------



	//------------------------------------------------------
	// IRS4
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IRS4,	VFP_S_IRS4_fmul_irs4( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADD_IRS4,	VFP_S_IRS4_fadd_irs4( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FMADD_IRS4,	MAD_S_IRS4_fmadd_irs4( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S2_IRS4,		NEON_V_IRS4_fmul_2s_irs4( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S2_IRS4,		NEON_V_IRS4_fadd_2s_irs4( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S2_IRS4,		NEON_V_IRS4_fmla_2s_irs4( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IRS4,		NEON_V_IRS4_fmul_4s_irs4( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S4_IRS4,		NEON_V_IRS4_fadd_4s_irs4( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S4_IRS4,		NEON_V_IRS4_fmla_4s_irs4( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------
	// IR1
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IR1,		VFP_S_IR1_fmul_ir1( Loop, 10.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FADD_IR1,		VFP_S_IR1_fadd_ir1( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_VFP_FMADD_IR1,	MAD_S_IR1_fmadd_ir1( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S2_IR1,	NEON_V_IR1_fmul_2s_ir1( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S2_IR1,	NEON_V_IR1_fadd_2s_ir1( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S2_IR1,	NEON_V_IR1_fmla_2s_ir1( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR1,	NEON_V_IR1_fmul_4s_ir1( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S4_IR1,	NEON_V_IR1_fadd_4s_ir1( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S4_IR1,	NEON_V_IR1_fmla_4s_ir1( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------




	//------------------------------------------------------
	// IR12
	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR12,		NEON_V_IR12_fmul_4s_ir12( Loop, 10.0f	) );
	Progress.Increment();

	SetResult( RESULT_NEON_FADD_S4_IR12,		NEON_V_IR12_fadd_4s_ir12( Loop, 7.0f		) );
	Progress.Increment();

	SetResult( RESULT_NEON_FMLA_S4_IR12,		NEON_V_IR12_fmla_4s_ir12( Loop, sum_2 ) );
	Progress.Increment();

	//------------------------------------------------------



	//------------------------------------------------------
	// SVE
	//------------------------------------------------------

#if USE_ARM_SVE2
	if( Info.HasInstructionSet( CPUFeature::ARM_SVE2 ) ){

		SetResult( RESULT_SVE_FMUL_S_IR12,		SVE_V_IR12_fmul_s_ir12( Loop, 10.0f	) );
		Progress.Increment();

		SetResult( RESULT_SVE_FADD_S_IR12,		SVE_V_IR12_fadd_s_ir12( Loop, 7.0f		) );
		Progress.Increment();

		SetResult( RESULT_SVE_FMLA_S_IR12,		SVE_V_IR12_fmla_s_ir12( Loop, sum_2 ) );
		Progress.Increment();

	}else
#endif
	{
		Progress.Increment();
		Progress.Increment();
		Progress.Increment();
	}

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
		return	"FPU/NEON (SP fp)";
	}else{
		return	"FPU/NEON (SP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // ARM64
//-----------------------------------------------------------------------------

