// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP64DP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_ARM64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace VFP64DP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if flOS_ANDROID
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

static void check_result( double ret, double ans )
{
	FL_LOG( "check: %f %f\n", ret, ans );
	flASSERT( ret == ans );
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
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"		\
			op " d0, d8, d9 \n"



#define VFP_S_IR1_0(op,name) \
static uint64_t VFP_S_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
			op " d0, d8, d9 \n"		\
			op " d1, d8, d9 \n"		\
			op " d2, d8, d9 \n"		\
			op " d3, d8, d9 \n"		\
			op " d4, d8, d9 \n"		\
			op " d5, d8, d9 \n"		\
			op " d6, d8, d9 \n"		\
			op " d7, d8, d9 \n"



#define VFP_S_IR8_0(op,name) \
static uint64_t VFP_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
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
// Single Interleave src 4
//-----------------------------------------------------------------------------

#define VFP_S_IRS4_8(op) \
			op " d0, d8, d4 \n"		\
			op " d1, d8, d5 \n"		\
			op " d2, d8, d6 \n"		\
			op " d3, d8, d7 \n"		\
			op " d4, d8, d0 \n"		\
			op " d5, d8, d1 \n"		\
			op " d6, d8, d2 \n"		\
			op " d7, d8, d3 \n"



#define VFP_S_IRS4_0(op,name) \
static uint64_t VFP_S_IRS4_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
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
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"		\
			op " d0, d0, d8, d9 \n"



#define MAD_S_IR1_0(op,name) \
static uint64_t MAD_S_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","w2","w3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
	return	timer.Result();	\
}

#define MAD_S_IR1(op,name) MAD_S_IR1_0(#op,name)










//----------------------------
// Single Interleave 8 MAD
//----------------------------


#define MAD_S_IR8_8(op) \
			op " d0, d0, d8, d9 \n"		\
			op " d1, d1, d8, d9 \n"		\
			op " d2, d2, d8, d9 \n"		\
			op " d3, d3, d8, d9 \n"		\
			op " d4, d4, d8, d9 \n"		\
			op " d5, d5, d8, d9 \n"		\
			op " d6, d6, d8, d9 \n"		\
			op " d7, d7, d8, d9 \n"



#define MAD_S_IR8_0(op,name) \
static uint64_t MAD_S_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
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

#define MAD_S_IR8(op,name) MAD_S_IR8_0(#op,name)






//-----------------------------------------------------------------------------
// Single Interleave src 4 MAD
//-----------------------------------------------------------------------------

#define MAD_S_IRS4_8(op) \
			op " d0, d0, d8, d4 \n"		\
			op " d1, d1, d8, d5 \n"		\
			op " d2, d2, d8, d6 \n"		\
			op " d3, d3, d8, d7 \n"		\
			op " d4, d4, d8, d0 \n"		\
			op " d5, d5, d8, d1 \n"		\
			op " d6, d6, d8, d2 \n"		\
			op " d7, d7, d8, d3 \n"



#define MAD_S_IRS4_0(op,name) \
static uint64_t MAD_S_IRS4_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	d8, w2\n"		\
		"scvtf	d9, w3\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
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
static uint64_t NEON_V_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()		\
		"mov	x2, #2\n"		\
		"mov	x3, #5\n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
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
static uint64_t NEON_V_IR1_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()		\
		"mov	x2, #2\n"		\
		"mov	x3, #5\n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
	:	[o0]"=m"(ret0)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
\
	timer.End();		\
	timer.Dump( op );	\
	/*check_result( ret0, answer );*/	\
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
static uint64_t NEON_V_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2\n"		\
		"mov	x3, #5\n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
static uint64_t NEON_V_IR8_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2\n"		\
		"mov	x3, #5\n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
static uint64_t NEON_V_IRS4_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2  \n"		\
		"mov	x3, #5  \n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
static uint64_t NEON_V_IRS4_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2  \n"		\
		"mov	x3, #5  \n"		\
		"dup	v8.2d, x2\n"	\
		"scvtf	v8.2d, v8.2d\n"		\
		"dup	v9.2d, x3\n"	\
		"scvtf	v9.2d, v9.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
static uint64_t NEON_V_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2  \n"		\
		"mov	x3, #5  \n"		\
		"dup	v12.2d, x2\n"	\
		"scvtf	v12.2d, v12.2d\n"		\
		"dup	v13.2d, x3\n"	\
		"scvtf	v13.2d, v13.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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
static uint64_t NEON_V_IR12_##name( CounterType LoopCount, double answer ) \
{ \
	double	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	x2, #2  \n"		\
		"mov	x3, #5  \n"		\
		"dup	v12.2d, x2\n"	\
		"scvtf	v12.2d, v12.2d\n"		\
		"dup	v13.2d, x3\n"	\
		"scvtf	v13.2d, v13.2d\n"		\
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
		"str	d0, %[o0]\n"	\
		"str	d1, %[o1]\n"	\
		"str	d2, %[o2]\n"	\
		"str	d3, %[o3]\n"	\
		"str	d4, %[o4]\n"	\
		"str	d5, %[o5]\n"	\
		"str	d6, %[o6]\n"	\
		"str	d7, %[o7]\n"	\
	:	[o0]"=m"(ret0),	\
		[o1]"=m"(ret1),	\
		[o2]"=m"(ret2),	\
		[o3]"=m"(ret3),	\
		[o4]"=m"(ret4),	\
		[o5]"=m"(ret5),	\
		[o6]"=m"(ret6),	\
		[o7]"=m"(ret7)	\
	: [loop]"r"(LoopCount)	\
	: "cc","x0","x2","x3",	\
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" ); \
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

#define NEON_V_IR12(op,name) NEON_V_IR12_0(#op,name)


//-----------------
#endif
//-----------------






//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


static const char*	Instruction_Title[]= {


	"FPU fmul (64bit x1) n8",
	"FPU fadd (64bit x1) n8",
	"FPU fmadd (64bit x1) n8",

	"NEON fmul.2d (64bit x2) n8",
	"NEON fadd.2d (64bit x2) n8",
	"NEON fmla.2d (64bit x2) n8",




	"FPU fmul (64bit x1) ns4",
	"FPU fadd (64bit x1) ns4",
	"FPU fmadd (64bit x1) ns4",

	"NEON fmul.2d (64bit x2) ns4",
	"NEON fadd.2d (64bit x2) ns4",
	"NEON fmla.2d (64bit x2) ns4",



	"FPU fmul (64bit x1) n1",
	"FPU fadd (64bit x1) n1",
	"FPU fmadd (64bit x1) n1",

	"NEON fmul.2d (64bit x2) n1",
	"NEON fadd.2d (64bit x2) n1",
	"NEON fmla.2d (64bit x2) n1",



	"NEON fmul.2d (64bit x2) n12",
	"NEON fadd.2d (64bit x2) n12",
	"NEON fmla.2d (64bit x2) n12",

};




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp( RESULT_VFP_FMUL_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IR8,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IR8,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_D2_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_D2_IR8,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_D2_IR8,	PER_LOOP_INST * 4, 4	);



	SetOp( RESULT_VFP_FMUL_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IRS4,	PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IRS4,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_D2_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_D2_IRS4,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_D2_IRS4,	PER_LOOP_INST * 4, 4	);



	SetOp( RESULT_VFP_FMUL_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FADD_IR1,		PER_LOOP_INST * 1, 1	);
	SetOp( RESULT_VFP_FMADD_IR1,	PER_LOOP_INST * 2, 2	);

	SetOp( RESULT_NEON_FMUL_D2_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FADD_D2_IR1,	PER_LOOP_INST * 2, 2	);
	SetOp( RESULT_NEON_FMLA_D2_IR1,	PER_LOOP_INST * 4, 4	);




	SetOp( RESULT_NEON_FMUL_D2_IR12,	PER_LOOP_INST_12 * 2, 2	);
	SetOp( RESULT_NEON_FADD_D2_IR12,	PER_LOOP_INST_12 * 2, 2	);
	SetOp( RESULT_NEON_FMLA_D2_IR12,	PER_LOOP_INST_12 * 4, 4	);

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

NEON_V_IR8( fmul, .2d, fmul_2d_ir8 );
NEON_V_IR8( fadd, .2d, fadd_2d_ir8 );
NEON_V_IR8( fmla, .2d, fmla_2d_ir8 );

//-----------------
#else
//-----------------

NEON_V_IR8( fmul.2d, fmul_2d_ir8 );
NEON_V_IR8( fadd.2d, fadd_2d_ir8 );
NEON_V_IR8( fmla.2d, fmla_2d_ir8 );

//-----------------
#endif
//-----------------


VFP_S_IRS4( fmul,  fmul_irs4 );
VFP_S_IRS4( fadd,  fadd_irs4 );
MAD_S_IRS4( fmadd, fmadd_irs4 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IRS4( fmul, .2d, fmul_2d_irs4 );
NEON_V_IRS4( fadd, .2d, fadd_2d_irs4 );
NEON_V_IRS4( fmla, .2d, fmla_2d_irs4 );

//-----------------
#else
//-----------------

NEON_V_IRS4( fmul.2d, fmul_2d_irs4 );
NEON_V_IRS4( fadd.2d, fadd_2d_irs4 );
NEON_V_IRS4( fmla.2d, fmla_2d_irs4 );

//-----------------
#endif
//-----------------


VFP_S_IR1( fmul,  fmul_ir1 );
VFP_S_IR1( fadd,  fadd_ir1 );
MAD_S_IR1( fmadd, fmadd_ir1 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IR1( fmul, .2d, fmul_2d_ir1 );
NEON_V_IR1( fadd, .2d, fadd_2d_ir1 );
NEON_V_IR1( fmla, .2d, fmla_2d_ir1 );



NEON_V_IR12( fmul, .2d, fmul_2d_ir12 );
NEON_V_IR12( fadd, .2d, fadd_2d_ir12 );
NEON_V_IR12( fmla, .2d, fmla_2d_ir12 );

//-----------------
#else
//-----------------

NEON_V_IR1( fmul.2d, fmul_2d_ir1 );
NEON_V_IR1( fadd.2d, fadd_2d_ir1 );
NEON_V_IR1( fmla.2d, fmla_2d_ir1 );



NEON_V_IR12( fmul.2d, fmul_2d_ir12 );
NEON_V_IR12( fadd.2d, fadd_2d_ir12 );
NEON_V_IR12( fmla.2d, fmla_2d_ir12 );

//-----------------
#endif
//-----------------






void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP64DP loop=%d\n", Loop );

	double	sum= 0;
	double	sum_2= 0;
	for( unsigned int i= 0 ; i< Loop * 5 ; i++ ){
		sum+= 2.0f;
		sum_2+= 10.0f;
	}

	//------------------------------------------------------
	// IR8
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IR8,		VFP_S_IR8_fmul_ir8( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADD_IR8,		VFP_S_IR8_fadd_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FMADD_IR8,	MAD_S_IR8_fmadd_ir8( Loop, sum ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_D2_IR8,	NEON_V_IR8_fmul_2d_ir8( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_D2_IR8,	NEON_V_IR8_fadd_2d_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_D2_IR8,	NEON_V_IR8_fmla_2d_ir8( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------





	//------------------------------------------------------
	// IRS4
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IRS4,	VFP_S_IRS4_fmul_irs4( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADD_IRS4,	VFP_S_IRS4_fadd_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FMADD_IRS4,	MAD_S_IRS4_fmadd_irs4( Loop, sum ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_D2_IRS4,		NEON_V_IRS4_fmul_2d_irs4( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_D2_IRS4,		NEON_V_IRS4_fadd_2d_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_D2_IRS4,		NEON_V_IRS4_fmla_2d_irs4( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------




	//------------------------------------------------------
	// IR1
	//------------------------------------------------------

	SetResult( RESULT_VFP_FMUL_IR1,		VFP_S_IR1_fmul_ir1( Loop, 10.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FADD_IR1,		VFP_S_IR1_fadd_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_VFP_FMADD_IR1,	MAD_S_IR1_fmadd_ir1( Loop, sum ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_D2_IR1,	NEON_V_IR1_fmul_2d_ir1( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_D2_IR1,	NEON_V_IR1_fadd_2d_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_D2_IR1,	NEON_V_IR1_fmla_2d_ir1( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------




	//------------------------------------------------------
	// IR12
	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_D2_IR12,		NEON_V_IR12_fmul_2d_ir12( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_D2_IR12,		NEON_V_IR12_fadd_2d_ir12( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_D2_IR12,		NEON_V_IR12_fmla_2d_ir12( Loop, sum_2 ) );
	Progress++;

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
		return	"FPU/NEON (DP fp)";
	}else{
		return	"FPU/NEON (DP fp) multi-thread";
	}
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // ARM64
//-----------------------------------------------------------------------------

