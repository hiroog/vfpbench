// 2019 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/SystemInfo.h>
#include	<minilib/Math.h>
#include	"TimerClass.h"
#include	"VFP64HP_Test.h"


//-----------------------------------------------------------------------------
#if flCPU_ARM64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace VFP64HP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if flCPU_FP16


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

static void check_result( uint32_t iret, float ans )
{
	float	ret= math::HalfToFloat( iret );
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


#if 0
		"orr	v16.16b, v0.16b, v0.16b\n"	\
		"orr	v17.16b, v0.16b, v0.16b\n"	\
		"orr	v18.16b, v0.16b, v0.16b\n"	\
		"orr	v19.16b, v0.16b, v0.16b\n"	\
		"orr	v20.16b, v0.16b, v0.16b\n"	\
		"orr	v21.16b, v0.16b, v0.16b\n"	\
		"orr	v22.16b, v0.16b, v0.16b\n"	\
		"orr	v23.16b, v0.16b, v0.16b\n"	\

#endif

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

#if 0
		"orr.16b	v16, v0, v0\n"	\
		"orr.16b	v17, v0, v0\n"	\
		"orr.16b	v18, v0, v0\n"	\
		"orr.16b	v19, v0, v0\n"	\
		"orr.16b	v20, v0, v0\n"	\
		"orr.16b	v21, v0, v0\n"	\
		"orr.16b	v22, v0, v0\n"	\
		"orr.16b	v23, v0, v0\n"	\

#endif

//-----------------
#endif
//-----------------



//-----------------------------------------------------------------------------
// Single Interleave 1
//-----------------------------------------------------------------------------

#define VFP_S_IR1_8(op) \
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"		\
			op " h0, h8, h9 \n"



#define VFP_S_IR1_0(op,name) \
static uint64_t VFP_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
	/*check_result( ret0, answer );*/	\
	return	timer.Result();	\
}

#define VFP_S_IR1(op,name) VFP_S_IR1_0(#op,name)













//-----------------------------------------------------------------------------
// Single Interleave 8
//-----------------------------------------------------------------------------

#define VFP_S_IR8_8(op) \
			op " h0, h8, h9 \n"		\
			op " h1, h8, h9 \n"		\
			op " h2, h8, h9 \n"		\
			op " h3, h8, h9 \n"		\
			op " h4, h8, h9 \n"		\
			op " h5, h8, h9 \n"		\
			op " h6, h8, h9 \n"		\
			op " h7, h8, h9 \n"


//		"udot   v0.4s, v1.16b, v1.16b\n"	\

#define VFP_S_IR8_0(op,name) \
static uint64_t VFP_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
			op " h0, h8, h4 \n"		\
			op " h1, h8, h5 \n"		\
			op " h2, h8, h6 \n"		\
			op " h3, h8, h7 \n"		\
			op " h4, h8, h0 \n"		\
			op " h5, h8, h1 \n"		\
			op " h6, h8, h2 \n"		\
			op " h7, h8, h3 \n"



#define VFP_S_IRS4_0(op,name) \
static uint64_t VFP_S_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		VFP_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"		\
			op " h0, h0, h8, h9 \n"



#define MAD_S_IR1_0(op,name) \
static uint64_t MAD_S_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
		"str	h0, %[o0]\n"	\
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
			op " h0, h0, h8, h9 \n"		\
			op " h1, h1, h8, h9 \n"		\
			op " h2, h2, h8, h9 \n"		\
			op " h3, h3, h8, h9 \n"		\
			op " h4, h4, h8, h9 \n"		\
			op " h5, h5, h8, h9 \n"		\
			op " h6, h6, h8, h9 \n"		\
			op " h7, h7, h8, h9 \n"



#define MAD_S_IR8_0(op,name) \
static uint64_t MAD_S_IR8_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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

#define MAD_S_IR8(op,name) MAD_S_IR8_0(#op,name)






//-----------------------------------------------------------------------------
// Single Interleave src 4 MAD
//-----------------------------------------------------------------------------

#define MAD_S_IRS4_8(op) \
			op " h0, h0, h8, h4 \n"		\
			op " h1, h1, h8, h5 \n"		\
			op " h2, h2, h8, h6 \n"		\
			op " h3, h3, h8, h7 \n"		\
			op " h4, h4, h8, h0 \n"		\
			op " h5, h5, h8, h1 \n"		\
			op " h6, h6, h8, h2 \n"		\
			op " h7, h7, h8, h3 \n"



#define MAD_S_IRS4_0(op,name) \
static uint64_t MAD_S_IRS4_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		MAD_REG_CLEAR()			\
		"mov	w2, #1\n"		\
		"mov	w3, #2\n"		\
		"scvtf	h8, w2\n"		\
		"scvtf	h9, w3\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
static uint64_t NEON_V_IR1_##name( CounterType LoopCount, float answer ) \
{ \
	uint32_t	ret0;	\
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
		"str	h0, %[o0]\n"	\
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
	uint32_t	ret0;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()		\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.8h, w2\n"	\
		"scvtf	v8.8h, v8.8h\n"		\
		"dup	v9.8h, w3\n"	\
		"scvtf	v9.8h, v9.8h\n"		\
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
		"str	h0, %[o0]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.8h, w2\n"	\
		"scvtf	v8.8h, v8.8h\n"		\
		"dup	v9.8h, w3\n"	\
		"scvtf	v9.8h, v9.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2\n"		\
		"mov	w3, #5\n"		\
		"dup	v8.8h, w2\n"	\
		"scvtf	v8.8h, v8.8h\n"		\
		"dup	v9.8h, w3\n"	\
		"scvtf	v9.8h, v9.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v8.8h, w2\n"	\
		"scvtf	v8.8h, v8.8h\n"		\
		"dup	v9.8h, w3\n"	\
		"scvtf	v9.8h, v9.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v8.8h, w2\n"	\
		"scvtf	v8.8h, v8.8h\n"		\
		"dup	v9.8h, w3\n"	\
		"scvtf	v9.8h, v9.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v12.8h, w2\n"	\
		"scvtf	v12.8h, v12.8h\n"		\
		"dup	v13.8h, w3\n"	\
		"scvtf	v13.8h, v13.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;	\
	TimerClass	timer;			\
	timer.Begin();				\
	__asm__ __volatile__(		\
		NEON_REG_CLEAR()			\
		"mov	w2, #2  \n"		\
		"mov	w3, #5  \n"		\
		"dup	v12.8h, w2\n"	\
		"scvtf	v12.8h, v12.8h\n"		\
		"dup	v13.8h, w3\n"	\
		"scvtf	v13.8h, v13.8h\n"		\
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
		"str	h0, %[o0]\n"	\
		"str	h1, %[o1]\n"	\
		"str	h2, %[o2]\n"	\
		"str	h3, %[o3]\n"	\
		"str	h4, %[o4]\n"	\
		"str	h5, %[o5]\n"	\
		"str	h6, %[o6]\n"	\
		"str	h7, %[o7]\n"	\
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

#define NEON_V_IR12(op,name) NEON_V_IR12_0(#op,name)

//-----------------
#endif
//-----------------




#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


static const char*	Instruction_Title[]= {

	"FPU fmul (16bit x1) n8",
	"FPU fadd (16bit x1) n8",
	"FPU fmadd (16bit x1) n8",

	"NEON fmul.4h (16bit x4) n8",
	"NEON fadd.4h (16bit x4) n8",
	"NEON fmla.4h (16bit x4) n8",

	"NEON fmul.8h (16bit x8) n8",
	"NEON fadd.8h (16bit x8) n8",
	"NEON fmla.8h (16bit x8) n8",


	"FPU fmul (16bit x1) ns4",
	"FPU fadd (16bit x1) ns4",
	"FPU fmadd (16bit x1) ns4",

	"NEON fmul.4h (16bit x4) ns4",
	"NEON fadd.4h (16bit x4) ns4",
	"NEON fmla.4h (16bit x4) ns4",

	"NEON fmul.8h (16bit x8) ns4",
	"NEON fadd.8h (16bit x8) ns4",
	"NEON fmla.8h (16bit x8) ns4",


	"FPU fmul (16bit x1) n1",
	"FPU fadd (16bit x1) n1",
	"FPU fmadd (16bit x1) n1",

	"NEON fmul.4h (16bit x4) n1",
	"NEON fadd.4h (16bit x4) n1",
	"NEON fmla.4h (16bit x4) n1",

	"NEON fmul.8h (16bit x8) n1",
	"NEON fadd.8h (16bit x8) n1",
	"NEON fmla.8h (16bit x8) n1",


	"NEON fmul.8h (16bit x8) n12",
	"NEON fadd.8h (16bit x8) n12",
	"NEON fmla.8h (16bit x8) n12",

};



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


FloatTest::FloatTest()
{
	flASSERT( RESULT_MAX <= RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();


	SetOp2( RESULT_VFP_FMUL_IR8,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FADD_IR8,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FMADD_IR8,		PER_LOOP_INST, 2	);

	SetOp2( RESULT_NEON_FMUL_S2_IR8,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FADD_S2_IR8,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FMLA_S2_IR8,	PER_LOOP_INST, 8	);

	SetOp2( RESULT_NEON_FMUL_S4_IR8,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FADD_S4_IR8,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FMLA_S4_IR8,	PER_LOOP_INST, 16	);



	SetOp2( RESULT_VFP_FMUL_IRS4,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FADD_IRS4,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FMADD_IRS4,		PER_LOOP_INST, 2	);

	SetOp2( RESULT_NEON_FMUL_S2_IRS4,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FADD_S2_IRS4,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FMLA_S2_IRS4,	PER_LOOP_INST, 8	);

	SetOp2( RESULT_NEON_FMUL_S4_IRS4,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FADD_S4_IRS4,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FMLA_S4_IRS4,	PER_LOOP_INST, 16	);



	SetOp2( RESULT_VFP_FMUL_IR1,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FADD_IR1,		PER_LOOP_INST, 1	);
	SetOp2( RESULT_VFP_FMADD_IR1,		PER_LOOP_INST, 2	);

	SetOp2( RESULT_NEON_FMUL_S2_IR1,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FADD_S2_IR1,	PER_LOOP_INST, 4	);
	SetOp2( RESULT_NEON_FMLA_S2_IR1,	PER_LOOP_INST, 8	);

	SetOp2( RESULT_NEON_FMUL_S4_IR1,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FADD_S4_IR1,	PER_LOOP_INST, 8	);
	SetOp2( RESULT_NEON_FMLA_S4_IR1,	PER_LOOP_INST, 16	);



	SetOp2( RESULT_NEON_FMUL_S4_IR12,	PER_LOOP_INST_12, 8	);
	SetOp2( RESULT_NEON_FADD_S4_IR12,	PER_LOOP_INST_12, 8	);
	SetOp2( RESULT_NEON_FMLA_S4_IR12,	PER_LOOP_INST_12, 16	);

}




unsigned int FloatTest::GetProgressStep() const
{
	return	RESULT_MAX;
}






//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#if flCPU_FP16



VFP_S_IR8( fmul,  fmul_ir8 );
VFP_S_IR8( fadd,  fadd_ir8 );
MAD_S_IR8( fmadd, fmadd_ir8 );


//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IR8( fmul, .4h, fmul_2s_ir8 );
NEON_V_IR8( fadd, .4h, fadd_2s_ir8 );
NEON_V_IR8( fmla, .4h, fmla_2s_ir8 );


NEON_V_IR8( fmul, .8h, fmul_4s_ir8 );
NEON_V_IR8( fadd, .8h, fadd_4s_ir8 );
NEON_V_IR8( fmla, .8h, fmla_4s_ir8 );

//-----------------
#else
//-----------------

NEON_V_IR8( fmul.4h, fmul_2s_ir8 );
NEON_V_IR8( fadd.4h, fadd_2s_ir8 );
NEON_V_IR8( fmla.4h, fmla_2s_ir8 );


NEON_V_IR8( fmul.8h, fmul_4s_ir8 );
NEON_V_IR8( fadd.8h, fadd_4s_ir8 );
NEON_V_IR8( fmla.8h, fmla_4s_ir8 );

//-----------------
#endif
//-----------------



VFP_S_IRS4( fmul,  fmul_irs4 );
VFP_S_IRS4( fadd,  fadd_irs4 );
MAD_S_IRS4( fmadd, fmadd_irs4 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IRS4( fmul, .4h, fmul_2s_irs4 );
NEON_V_IRS4( fadd, .4h, fadd_2s_irs4 );
NEON_V_IRS4( fmla, .4h, fmla_2s_irs4 );


NEON_V_IRS4( fmul, .8h, fmul_4s_irs4 );
NEON_V_IRS4( fadd, .8h, fadd_4s_irs4 );
NEON_V_IRS4( fmla, .8h, fmla_4s_irs4 );

//-----------------
#else
//-----------------

NEON_V_IRS4( fmul.4h, fmul_2s_irs4 );
NEON_V_IRS4( fadd.4h, fadd_2s_irs4 );
NEON_V_IRS4( fmla.4h, fmla_2s_irs4 );


NEON_V_IRS4( fmul.8h, fmul_4s_irs4 );
NEON_V_IRS4( fadd.8h, fadd_4s_irs4 );
NEON_V_IRS4( fmla.8h, fmla_4s_irs4 );

//-----------------
#endif
//-----------------



VFP_S_IR1( fmul,  fmul_ir1 );
VFP_S_IR1( fadd,  fadd_ir1 );
MAD_S_IR1( fmadd, fmadd_ir1 );

//-----------------
#if USE_ANDROID_GCC
//-----------------

NEON_V_IR1( fmul, .4h, fmul_2s_ir1 );
NEON_V_IR1( fadd, .4h, fadd_2s_ir1 );
NEON_V_IR1( fmla, .4h, fmla_2s_ir1 );


NEON_V_IR1( fmul, .8h, fmul_4s_ir1 );
NEON_V_IR1( fadd, .8h, fadd_4s_ir1 );
NEON_V_IR1( fmla, .8h, fmla_4s_ir1 );



NEON_V_IR12( fmul, .8h, fmul_4s_ir12 );
NEON_V_IR12( fadd, .8h, fadd_4s_ir12 );
NEON_V_IR12( fmla, .8h, fmla_4s_ir12 );

//-----------------
#else
//-----------------

NEON_V_IR1( fmul.4h, fmul_2s_ir1 );
NEON_V_IR1( fadd.4h, fadd_2s_ir1 );
NEON_V_IR1( fmla.4h, fmla_2s_ir1 );


NEON_V_IR1( fmul.8h, fmul_4s_ir1 );
NEON_V_IR1( fadd.8h, fadd_4s_ir1 );
NEON_V_IR1( fmla.8h, fmla_4s_ir1 );



NEON_V_IR12( fmul.8h, fmul_4s_ir12 );
NEON_V_IR12( fadd.8h, fadd_4s_ir12 );
NEON_V_IR12( fmla.8h, fmla_4s_ir12 );

//-----------------
#endif
//-----------------





#endif


void FloatTest::Run()
{
#if flCPU_FP16
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP64 loop=%d\n", Loop );

	float	sum= 0;
	float	sum_2= 0;
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

	SetResult( RESULT_NEON_FMUL_S2_IR8,	NEON_V_IR8_fmul_2s_ir8( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S2_IR8,	NEON_V_IR8_fadd_2s_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S2_IR8,	NEON_V_IR8_fmla_2s_ir8( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR8,	NEON_V_IR8_fmul_4s_ir8( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S4_IR8,	NEON_V_IR8_fadd_4s_ir8( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S4_IR8,	NEON_V_IR8_fmla_4s_ir8( Loop, sum_2 ) );
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

	SetResult( RESULT_NEON_FMUL_S2_IRS4,		NEON_V_IRS4_fmul_2s_irs4( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S2_IRS4,		NEON_V_IRS4_fadd_2s_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S2_IRS4,		NEON_V_IRS4_fmla_2s_irs4( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IRS4,		NEON_V_IRS4_fmul_4s_irs4( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S4_IRS4,		NEON_V_IRS4_fadd_4s_irs4( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S4_IRS4,		NEON_V_IRS4_fmla_4s_irs4( Loop, sum_2 ) );
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

	SetResult( RESULT_NEON_FMUL_S2_IR1,	NEON_V_IR1_fmul_2s_ir1( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S2_IR1,	NEON_V_IR1_fadd_2s_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S2_IR1,	NEON_V_IR1_fmla_2s_ir1( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------


	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR1,	NEON_V_IR1_fmul_4s_ir1( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S4_IR1,	NEON_V_IR1_fadd_4s_ir1( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S4_IR1,	NEON_V_IR1_fmla_4s_ir1( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------




	//------------------------------------------------------
	// IR12
	//------------------------------------------------------

	SetResult( RESULT_NEON_FMUL_S4_IR12,		NEON_V_IR12_fmul_4s_ir12( Loop, 10.0f	) );
	Progress++;

	SetResult( RESULT_NEON_FADD_S4_IR12,		NEON_V_IR12_fadd_4s_ir12( Loop, 7.0f		) );
	Progress++;

	SetResult( RESULT_NEON_FMLA_S4_IR12,		NEON_V_IR12_fmla_4s_ir12( Loop, sum_2 ) );
	Progress++;

	//------------------------------------------------------


	DoneFlag= true;
#endif
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
		return	"FPU/NEON (HP fp)";
	}else{
		return	"FPU/NEON (HP fp) multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // ARM64
//-----------------------------------------------------------------------------

