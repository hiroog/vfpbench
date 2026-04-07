// 2024 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP64INT8_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_ARM64
//-----------------------------------------------------------------------------

#if defined(__ARM_FEATURE_MATMUL_INT8) && __ARM_FEATURE_MATMUL_INT8
# define	USE_ARM_I8MM		1
#endif

#if defined(__ARM_FEATURE_DOTPROD) && __ARM_FEATURE_DOTPROD
# define	USE_ARM_DOTPROD		1
#endif

#if defined(__ARM_FEATURE_SVE) && __ARM_FEATURE_SVE
# define	USE_ARM_SVE			1
#endif

#ifndef VB_MINIMUM_ARM_INT8
# define	VB_MINIMUM_ARM_INT8		1
#endif

using namespace flatlib;
using system::CPUFeature;

namespace VFP64INT8 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


typedef	unsigned long	CounterType;


/*
	smmla  Vd.4s, Vn.16b, Vm.16b  signed int8 MMLA:  2x8 * 8x2 -> 2x2 int32 MA
	ummla  Vd.4s, Vn.16b, Vm.16b  unsigned int8 MMLA
	usmmla Vd.4s, Vn.16b, Vm.16b  mixed (unsigned*signed) int8 MMLA
	  OPS per instruction: 4 outputs x (8 mul + 8 add) = 64

	sdot   Vd.4s, Vn.16b, Vm.16b  signed   int8 dot product (128-bit)
	udot   Vd.4s, Vn.16b, Vm.16b  unsigned int8 dot product (128-bit)
	  OPS per instruction: 4 outputs x (4 mul + 4 add) = 32

	sdot   Vd.2s, Vn.8b,  Vm.8b   signed   int8 dot product (64-bit)
	udot   Vd.2s, Vn.8b,  Vm.8b   unsigned int8 dot product (64-bit)
	  OPS per instruction: 2 outputs x (4 mul + 4 add) = 16

	Note: columns labelled "MFLOPS" in the result table show MOPS for integer tests.
*/


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
		"orr.16b	v15, v0, v0\n"


//=============================================================================
// FEAT_I8MM: smmla / ummla / usmmla  (n12 only)
//=============================================================================

#if USE_ARM_I8MM

//-----------------------------------------------------------------------------
// smmla Interleave 12
//-----------------------------------------------------------------------------

#define SMMLA_IR12_12() \
			"smmla v0.4s,  v12.16b, v13.16b\n"	\
			"smmla v1.4s,  v12.16b, v13.16b\n"	\
			"smmla v2.4s,  v12.16b, v13.16b\n"	\
			"smmla v3.4s,  v12.16b, v13.16b\n"	\
			"smmla v4.4s,  v12.16b, v13.16b\n"	\
			"smmla v5.4s,  v12.16b, v13.16b\n"	\
			"smmla v6.4s,  v12.16b, v13.16b\n"	\
			"smmla v7.4s,  v12.16b, v13.16b\n"	\
			"smmla v8.4s,  v12.16b, v13.16b\n"	\
			"smmla v9.4s,  v12.16b, v13.16b\n"	\
			"smmla v10.4s, v12.16b, v13.16b\n"	\
			"smmla v11.4s, v12.16b, v13.16b\n"


static uint64_t NEON_SMMLA_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.16b, #1\n"
		"movi	v13.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SMMLA_IR12_12()
		SMMLA_IR12_12()
		SMMLA_IR12_12()
		SMMLA_IR12_12()
		SMMLA_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "smmla" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// ummla Interleave 12
//-----------------------------------------------------------------------------

#define UMMLA_IR12_12() \
			"ummla v0.4s,  v12.16b, v13.16b\n"	\
			"ummla v1.4s,  v12.16b, v13.16b\n"	\
			"ummla v2.4s,  v12.16b, v13.16b\n"	\
			"ummla v3.4s,  v12.16b, v13.16b\n"	\
			"ummla v4.4s,  v12.16b, v13.16b\n"	\
			"ummla v5.4s,  v12.16b, v13.16b\n"	\
			"ummla v6.4s,  v12.16b, v13.16b\n"	\
			"ummla v7.4s,  v12.16b, v13.16b\n"	\
			"ummla v8.4s,  v12.16b, v13.16b\n"	\
			"ummla v9.4s,  v12.16b, v13.16b\n"	\
			"ummla v10.4s, v12.16b, v13.16b\n"	\
			"ummla v11.4s, v12.16b, v13.16b\n"


static uint64_t NEON_UMMLA_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.16b, #1\n"
		"movi	v13.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UMMLA_IR12_12()
		UMMLA_IR12_12()
		UMMLA_IR12_12()
		UMMLA_IR12_12()
		UMMLA_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "ummla" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// usmmla Interleave 12
//-----------------------------------------------------------------------------

#define USMMLA_IR12_12() \
			"usmmla v0.4s,  v12.16b, v13.16b\n"	\
			"usmmla v1.4s,  v12.16b, v13.16b\n"	\
			"usmmla v2.4s,  v12.16b, v13.16b\n"	\
			"usmmla v3.4s,  v12.16b, v13.16b\n"	\
			"usmmla v4.4s,  v12.16b, v13.16b\n"	\
			"usmmla v5.4s,  v12.16b, v13.16b\n"	\
			"usmmla v6.4s,  v12.16b, v13.16b\n"	\
			"usmmla v7.4s,  v12.16b, v13.16b\n"	\
			"usmmla v8.4s,  v12.16b, v13.16b\n"	\
			"usmmla v9.4s,  v12.16b, v13.16b\n"	\
			"usmmla v10.4s, v12.16b, v13.16b\n"	\
			"usmmla v11.4s, v12.16b, v13.16b\n"


static uint64_t NEON_USMMLA_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.16b, #1\n"
		"movi	v13.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		USMMLA_IR12_12()
		USMMLA_IR12_12()
		USMMLA_IR12_12()
		USMMLA_IR12_12()
		USMMLA_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "usmmla" );
	return	timer.Result();
}

#endif // USE_ARM_I8MM


//=============================================================================
// FEAT_DOTPROD: sdot / udot  .4s (128-bit) and .2s (64-bit)
//=============================================================================

#if USE_ARM_DOTPROD

//-----------------------------------------------------------------------------
// sdot .4s  Interleave 8
//-----------------------------------------------------------------------------

#define SDOT_S4_IR8_8() \
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v1.4s, v8.16b, v9.16b\n"		\
			"sdot v2.4s, v8.16b, v9.16b\n"		\
			"sdot v3.4s, v8.16b, v9.16b\n"		\
			"sdot v4.4s, v8.16b, v9.16b\n"		\
			"sdot v5.4s, v8.16b, v9.16b\n"		\
			"sdot v6.4s, v8.16b, v9.16b\n"		\
			"sdot v7.4s, v8.16b, v9.16b\n"

#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_SDOT_S4_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.16b, #1\n"
		"movi	v9.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S4_IR8_8()
		SDOT_S4_IR8_8()
		SDOT_S4_IR8_8()
		SDOT_S4_IR8_8()
		SDOT_S4_IR8_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// sdot .4s  Interleave 12
//-----------------------------------------------------------------------------

#define SDOT_S4_IR12_12() \
			"sdot v0.4s,  v12.16b, v13.16b\n"	\
			"sdot v1.4s,  v12.16b, v13.16b\n"	\
			"sdot v2.4s,  v12.16b, v13.16b\n"	\
			"sdot v3.4s,  v12.16b, v13.16b\n"	\
			"sdot v4.4s,  v12.16b, v13.16b\n"	\
			"sdot v5.4s,  v12.16b, v13.16b\n"	\
			"sdot v6.4s,  v12.16b, v13.16b\n"	\
			"sdot v7.4s,  v12.16b, v13.16b\n"	\
			"sdot v8.4s,  v12.16b, v13.16b\n"	\
			"sdot v9.4s,  v12.16b, v13.16b\n"	\
			"sdot v10.4s, v12.16b, v13.16b\n"	\
			"sdot v11.4s, v12.16b, v13.16b\n"


static uint64_t NEON_SDOT_S4_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.16b, #1\n"
		"movi	v13.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S4_IR12_12()
		SDOT_S4_IR12_12()
		SDOT_S4_IR12_12()
		SDOT_S4_IR12_12()
		SDOT_S4_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// sdot .4s  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define SDOT_S4_IR1_8() \
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"		\
			"sdot v0.4s, v8.16b, v9.16b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_SDOT_S4_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.16b, #1\n"
		"movi	v9.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S4_IR1_8()
		SDOT_S4_IR1_8()
		SDOT_S4_IR1_8()
		SDOT_S4_IR1_8()
		SDOT_S4_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// sdot .2s  Interleave 8
//-----------------------------------------------------------------------------

#define SDOT_S2_IR8_8() \
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v1.2s, v8.8b, v9.8b\n"		\
			"sdot v2.2s, v8.8b, v9.8b\n"		\
			"sdot v3.2s, v8.8b, v9.8b\n"		\
			"sdot v4.2s, v8.8b, v9.8b\n"		\
			"sdot v5.2s, v8.8b, v9.8b\n"		\
			"sdot v6.2s, v8.8b, v9.8b\n"		\
			"sdot v7.2s, v8.8b, v9.8b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_SDOT_S2_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.8b, #1\n"
		"movi	v9.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S2_IR8_8()
		SDOT_S2_IR8_8()
		SDOT_S2_IR8_8()
		SDOT_S2_IR8_8()
		SDOT_S2_IR8_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// sdot .2s  Interleave 12
//-----------------------------------------------------------------------------

#define SDOT_S2_IR12_12() \
			"sdot v0.2s,  v12.8b, v13.8b\n"	\
			"sdot v1.2s,  v12.8b, v13.8b\n"	\
			"sdot v2.2s,  v12.8b, v13.8b\n"	\
			"sdot v3.2s,  v12.8b, v13.8b\n"	\
			"sdot v4.2s,  v12.8b, v13.8b\n"	\
			"sdot v5.2s,  v12.8b, v13.8b\n"	\
			"sdot v6.2s,  v12.8b, v13.8b\n"	\
			"sdot v7.2s,  v12.8b, v13.8b\n"	\
			"sdot v8.2s,  v12.8b, v13.8b\n"	\
			"sdot v9.2s,  v12.8b, v13.8b\n"	\
			"sdot v10.2s, v12.8b, v13.8b\n"	\
			"sdot v11.2s, v12.8b, v13.8b\n"


static uint64_t NEON_SDOT_S2_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.8b, #1\n"
		"movi	v13.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S2_IR12_12()
		SDOT_S2_IR12_12()
		SDOT_S2_IR12_12()
		SDOT_S2_IR12_12()
		SDOT_S2_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// sdot .2s  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define SDOT_S2_IR1_8() \
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"		\
			"sdot v0.2s, v8.8b, v9.8b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_SDOT_S2_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.8b, #1\n"
		"movi	v9.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		SDOT_S2_IR1_8()
		SDOT_S2_IR1_8()
		SDOT_S2_IR1_8()
		SDOT_S2_IR1_8()
		SDOT_S2_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "sdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// udot .4s  Interleave 8
//-----------------------------------------------------------------------------

#define UDOT_S4_IR8_8() \
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v1.4s, v8.16b, v9.16b\n"		\
			"udot v2.4s, v8.16b, v9.16b\n"		\
			"udot v3.4s, v8.16b, v9.16b\n"		\
			"udot v4.4s, v8.16b, v9.16b\n"		\
			"udot v5.4s, v8.16b, v9.16b\n"		\
			"udot v6.4s, v8.16b, v9.16b\n"		\
			"udot v7.4s, v8.16b, v9.16b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_UDOT_S4_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.16b, #1\n"
		"movi	v9.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S4_IR8_8()
		UDOT_S4_IR8_8()
		UDOT_S4_IR8_8()
		UDOT_S4_IR8_8()
		UDOT_S4_IR8_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// udot .4s  Interleave 12
//-----------------------------------------------------------------------------

#define UDOT_S4_IR12_12() \
			"udot v0.4s,  v12.16b, v13.16b\n"	\
			"udot v1.4s,  v12.16b, v13.16b\n"	\
			"udot v2.4s,  v12.16b, v13.16b\n"	\
			"udot v3.4s,  v12.16b, v13.16b\n"	\
			"udot v4.4s,  v12.16b, v13.16b\n"	\
			"udot v5.4s,  v12.16b, v13.16b\n"	\
			"udot v6.4s,  v12.16b, v13.16b\n"	\
			"udot v7.4s,  v12.16b, v13.16b\n"	\
			"udot v8.4s,  v12.16b, v13.16b\n"	\
			"udot v9.4s,  v12.16b, v13.16b\n"	\
			"udot v10.4s, v12.16b, v13.16b\n"	\
			"udot v11.4s, v12.16b, v13.16b\n"


static uint64_t NEON_UDOT_S4_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.16b, #1\n"
		"movi	v13.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S4_IR12_12()
		UDOT_S4_IR12_12()
		UDOT_S4_IR12_12()
		UDOT_S4_IR12_12()
		UDOT_S4_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// udot .4s  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define UDOT_S4_IR1_8() \
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"		\
			"udot v0.4s, v8.16b, v9.16b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_UDOT_S4_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.16b, #1\n"
		"movi	v9.16b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S4_IR1_8()
		UDOT_S4_IR1_8()
		UDOT_S4_IR1_8()
		UDOT_S4_IR1_8()
		UDOT_S4_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// udot .2s  Interleave 8
//-----------------------------------------------------------------------------

#define UDOT_S2_IR8_8() \
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v1.2s, v8.8b, v9.8b\n"		\
			"udot v2.2s, v8.8b, v9.8b\n"		\
			"udot v3.2s, v8.8b, v9.8b\n"		\
			"udot v4.2s, v8.8b, v9.8b\n"		\
			"udot v5.2s, v8.8b, v9.8b\n"		\
			"udot v6.2s, v8.8b, v9.8b\n"		\
			"udot v7.2s, v8.8b, v9.8b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_UDOT_S2_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.8b, #1\n"
		"movi	v9.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S2_IR8_8()
		UDOT_S2_IR8_8()
		UDOT_S2_IR8_8()
		UDOT_S2_IR8_8()
		UDOT_S2_IR8_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// udot .2s  Interleave 12
//-----------------------------------------------------------------------------

#define UDOT_S2_IR12_12() \
			"udot v0.2s,  v12.8b, v13.8b\n"	\
			"udot v1.2s,  v12.8b, v13.8b\n"	\
			"udot v2.2s,  v12.8b, v13.8b\n"	\
			"udot v3.2s,  v12.8b, v13.8b\n"	\
			"udot v4.2s,  v12.8b, v13.8b\n"	\
			"udot v5.2s,  v12.8b, v13.8b\n"	\
			"udot v6.2s,  v12.8b, v13.8b\n"	\
			"udot v7.2s,  v12.8b, v13.8b\n"	\
			"udot v8.2s,  v12.8b, v13.8b\n"	\
			"udot v9.2s,  v12.8b, v13.8b\n"	\
			"udot v10.2s, v12.8b, v13.8b\n"	\
			"udot v11.2s, v12.8b, v13.8b\n"


static uint64_t NEON_UDOT_S2_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v12.8b, #1\n"
		"movi	v13.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S2_IR12_12()
		UDOT_S2_IR12_12()
		UDOT_S2_IR12_12()
		UDOT_S2_IR12_12()
		UDOT_S2_IR12_12()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
		"str	s1, %[o1]\n"
		"str	s2, %[o2]\n"
		"str	s3, %[o3]\n"
		"str	s4, %[o4]\n"
		"str	s5, %[o5]\n"
		"str	s6, %[o6]\n"
		"str	s7, %[o7]\n"
	:	[o0]"=m"(ret0),
		[o1]"=m"(ret1),
		[o2]"=m"(ret2),
		[o3]"=m"(ret3),
		[o4]"=m"(ret4),
		[o5]"=m"(ret5),
		[o6]"=m"(ret6),
		[o7]"=m"(ret7)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// udot .2s  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define UDOT_S2_IR1_8() \
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"		\
			"udot v0.2s, v8.8b, v9.8b\n"


#if !VB_MINIMUM_ARM_INT8
static uint64_t NEON_UDOT_S2_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"movi	v8.8b, #1\n"
		"movi	v9.8b, #2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		UDOT_S2_IR1_8()
		UDOT_S2_IR1_8()
		UDOT_S2_IR1_8()
		UDOT_S2_IR1_8()
		UDOT_S2_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "udot" );
	return	timer.Result();
}
#endif

#endif // USE_ARM_DOTPROD


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static const char*	Instruction_Title[]= {
	// smmla/ummla/usmmla  FOP=64
	"NEON smmla.4s  (int8 x16) n12",
	"NEON ummla.4s  (int8 x16) n12",
	"NEON usmmla.4s (int8 x16) n12",

	// sdot .2s  FOP=16
#if !VB_MINIMUM_ARM_INT8
	"NEON sdot.2s   (int8 x8)  n8",
#endif
	"NEON sdot.2s   (int8 x8)  n12",
#if !VB_MINIMUM_ARM_INT8
	"NEON sdot.2s   (int8 x8)  n1",
#endif

	// udot .2s  FOP=16
#if !VB_MINIMUM_ARM_INT8
	"NEON udot.2s   (int8 x8)  n8",
#endif
	"NEON udot.2s   (int8 x8)  n12",
#if !VB_MINIMUM_ARM_INT8
	"NEON udot.2s   (int8 x8)  n1",
#endif

	// sdot .4s  FOP=32
#if !VB_MINIMUM_ARM_INT8
	"NEON sdot.4s   (int8 x16) n8",
#endif
	"NEON sdot.4s   (int8 x16) n12",
#if !VB_MINIMUM_ARM_INT8
	"NEON sdot.4s   (int8 x16) n1",
#endif

	// udot .4s  FOP=32
#if !VB_MINIMUM_ARM_INT8
	"NEON udot.4s   (int8 x16) n8",
#endif
	"NEON udot.4s   (int8 x16) n12",
#if !VB_MINIMUM_ARM_INT8
	"NEON udot.4s   (int8 x16) n1",
#endif
};

static_assert( sizeof(Instruction_Title)/sizeof(Instruction_Title[0]) == FloatTest::RESULT_MAX,
               "Instruction_Title size mismatch" );

unsigned int	FloatTest::GetProgressStep() const
{
	return	RESULT_MAX;
}

FloatTest::FloatTest()
{
	FL_ASSERT( (unsigned int)RESULT_MAX <= (unsigned int)RESULT_BUFFER_MAX );
	LoopCount= DEFAULT_LOOP;
	ClearResult();

	// smmla/ummla/usmmla: OPS = 4 outputs x (8 mul + 8 add) = 64
	SetOp2( RESULT_NEON_SMMLA_IR12,	PER_LOOP_INST_12, 64 );
	SetOp2( RESULT_NEON_UMMLA_IR12,	PER_LOOP_INST_12, 64 );
	SetOp2( RESULT_NEON_USMMLA_IR12,	PER_LOOP_INST_12, 64 );

	// sdot .2s: OPS = 2 outputs x (4 mul + 4 add) = 16
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_SDOT_S2_IR8,	PER_LOOP_INST,    16 );
#endif
	SetOp2( RESULT_NEON_SDOT_S2_IR12,	PER_LOOP_INST_12, 16 );
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_SDOT_S2_IR1,	PER_LOOP_INST,    16 );
#endif

	// udot .2s: OPS = 2 outputs x (4 mul + 4 add) = 16
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_UDOT_S2_IR8,	PER_LOOP_INST,    16 );
#endif
	SetOp2( RESULT_NEON_UDOT_S2_IR12,	PER_LOOP_INST_12, 16 );
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_UDOT_S2_IR1,	PER_LOOP_INST,    16 );
#endif

	// sdot .4s: OPS = 4 outputs x (4 mul + 4 add) = 32
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_SDOT_S4_IR8,	PER_LOOP_INST,    32 );
#endif
	SetOp2( RESULT_NEON_SDOT_S4_IR12,	PER_LOOP_INST_12, 32 );
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_SDOT_S4_IR1,	PER_LOOP_INST,    32 );
#endif

	// udot .4s: OPS = 4 outputs x (4 mul + 4 add) = 32
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_UDOT_S4_IR8,	PER_LOOP_INST,    32 );
#endif
	SetOp2( RESULT_NEON_UDOT_S4_IR12,	PER_LOOP_INST_12, 32 );
#if !VB_MINIMUM_ARM_INT8
	SetOp2( RESULT_NEON_UDOT_S4_IR1,	PER_LOOP_INST,    32 );
#endif
}


void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP64INT8 loop=%d\n", Loop );

#if USE_ARM_I8MM
	if( Info.HasInstructionSet( CPUFeature::ARM_I8MM ) ){
		SetResult( RESULT_NEON_SMMLA_IR12,		NEON_SMMLA_IR12(  Loop ) );
		Progress.Increment();

		SetResult( RESULT_NEON_UMMLA_IR12,		NEON_UMMLA_IR12(  Loop ) );
		Progress.Increment();

		SetResult( RESULT_NEON_USMMLA_IR12,	NEON_USMMLA_IR12( Loop ) );
		Progress.Increment();
	}else
#endif
	{
		Progress.Set( Progress.Get() + 3 );
	}

#if USE_ARM_DOTPROD
	if( Info.HasInstructionSet( CPUFeature::ARM_SIMDDP ) ){

# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_SDOT_S2_IR8,	NEON_SDOT_S2_IR8(  Loop ) );
		Progress.Increment();
# endif
		SetResult( RESULT_NEON_SDOT_S2_IR12,	NEON_SDOT_S2_IR12( Loop ) );
		Progress.Increment();
# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_SDOT_S2_IR1,	NEON_SDOT_S2_IR1(  Loop ) );
		Progress.Increment();
# endif

# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_UDOT_S2_IR8,	NEON_UDOT_S2_IR8(  Loop ) );
		Progress.Increment();
# endif
		SetResult( RESULT_NEON_UDOT_S2_IR12,	NEON_UDOT_S2_IR12( Loop ) );
		Progress.Increment();
# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_UDOT_S2_IR1,	NEON_UDOT_S2_IR1(  Loop ) );
		Progress.Increment();
# endif

# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_SDOT_S4_IR8,	NEON_SDOT_S4_IR8(  Loop ) );
		Progress.Increment();
# endif
		SetResult( RESULT_NEON_SDOT_S4_IR12,	NEON_SDOT_S4_IR12( Loop ) );
		Progress.Increment();
# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_SDOT_S4_IR1,	NEON_SDOT_S4_IR1(  Loop ) );
		Progress.Increment();
# endif

# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_UDOT_S4_IR8,	NEON_UDOT_S4_IR8(  Loop ) );
		Progress.Increment();
# endif
		SetResult( RESULT_NEON_UDOT_S4_IR12,	NEON_UDOT_S4_IR12( Loop ) );
		Progress.Increment();
# if !VB_MINIMUM_ARM_INT8
		SetResult( RESULT_NEON_UDOT_S4_IR1,	NEON_UDOT_S4_IR1(  Loop ) );
		Progress.Increment();
# endif
	}else
#endif
	{
		Progress.Set( RESULT_MAX );
	}

	FL_ASSERT( Progress.Get() == RESULT_MAX );

	DoneFlag.Set( 1 );
}


const char*	FloatTest::GetTestName() const
{
	if( !IsMultithread() ){
		return	"NEON (INT8)";
	}else{
		return	"NEON (INT8) multi-thread";
	}
}

const char*	FloatTest::GetInstructionName( unsigned int result_index ) const
{
	if( result_index < RESULT_MAX ){
		return	Instruction_Title[result_index];
	}
	return	nullptr;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // ARM64
//-----------------------------------------------------------------------------
