// 2024 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"VFP64BF16_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_ARM64
//-----------------------------------------------------------------------------

#if defined(__ARM_FEATURE_BF16_VECTOR_ARITHMETIC) && __ARM_FEATURE_BF16_VECTOR_ARITHMETIC
# define	USE_ARM_BF16			1
#endif

#ifndef	VB_MINIMUM_ARM_BF16
# define	VB_MINIMUM_ARM_BF16		1
#endif

using namespace flatlib;
using system::CPUFeature;

namespace VFP64BF16 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if USE_ARM_BF16


typedef	unsigned long	CounterType;


/*
	bfmmla Vd.4s, Vn.8h, Vm.8h
	  FLOPs: 4 outputs x (4 mul + 4 add) = 32

	bfdot Vd.4s, Vn.8h, Vm.8h
	  FLOPs: 4 outputs x (2 mul + 2 add) = 16

	bfdot Vd.2s, Vn.4h, Vm.4h
	  FLOPs: 2 outputs x (2 mul + 2 add) = 8
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
// bfmmla
//=============================================================================

//-----------------------------------------------------------------------------
// bfmmla Interleave 12  (12 independent accumulators, max throughput)
//-----------------------------------------------------------------------------

#define BFMMLA_IR12_12() \
			"bfmmla v0.4s, v12.8h, v13.8h\n"	\
			"bfmmla v1.4s, v12.8h, v13.8h\n"	\
			"bfmmla v2.4s, v12.8h, v13.8h\n"	\
			"bfmmla v3.4s, v12.8h, v13.8h\n"	\
			"bfmmla v4.4s, v12.8h, v13.8h\n"	\
			"bfmmla v5.4s, v12.8h, v13.8h\n"	\
			"bfmmla v6.4s, v12.8h, v13.8h\n"	\
			"bfmmla v7.4s, v12.8h, v13.8h\n"	\
			"bfmmla v8.4s, v12.8h, v13.8h\n"	\
			"bfmmla v9.4s, v12.8h, v13.8h\n"	\
			"bfmmla v10.4s, v12.8h, v13.8h\n"	\
			"bfmmla v11.4s, v12.8h, v13.8h\n"


static uint64_t NEON_BFMMLA_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v12.8h, w2\n"
		"dup	v13.8h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFMMLA_IR12_12()
		BFMMLA_IR12_12()
		BFMMLA_IR12_12()
		BFMMLA_IR12_12()
		BFMMLA_IR12_12()
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
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfmmla" );
	return	timer.Result();
}


//=============================================================================
// bfdot .4s  (128-bit)
//=============================================================================

//-----------------------------------------------------------------------------
// bfdot.4s  Interleave 8
//-----------------------------------------------------------------------------

#define BFDOT_S4_IR8_8() \
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v1.4s, v8.8h, v9.8h\n"		\
			"bfdot v2.4s, v8.8h, v9.8h\n"		\
			"bfdot v3.4s, v8.8h, v9.8h\n"		\
			"bfdot v4.4s, v8.8h, v9.8h\n"		\
			"bfdot v5.4s, v8.8h, v9.8h\n"		\
			"bfdot v6.4s, v8.8h, v9.8h\n"		\
			"bfdot v7.4s, v8.8h, v9.8h\n"


#if !VB_MINIMUM_ARM_BF16
static uint64_t NEON_BFDOT_S4_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v8.8h, w2\n"
		"dup	v9.8h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S4_IR8_8()
		BFDOT_S4_IR8_8()
		BFDOT_S4_IR8_8()
		BFDOT_S4_IR8_8()
		BFDOT_S4_IR8_8()
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
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// bfdot.4s  Interleave 12
//-----------------------------------------------------------------------------

#define BFDOT_S4_IR12_12() \
			"bfdot v0.4s, v12.8h, v13.8h\n"	\
			"bfdot v1.4s, v12.8h, v13.8h\n"	\
			"bfdot v2.4s, v12.8h, v13.8h\n"	\
			"bfdot v3.4s, v12.8h, v13.8h\n"	\
			"bfdot v4.4s, v12.8h, v13.8h\n"	\
			"bfdot v5.4s, v12.8h, v13.8h\n"	\
			"bfdot v6.4s, v12.8h, v13.8h\n"	\
			"bfdot v7.4s, v12.8h, v13.8h\n"	\
			"bfdot v8.4s, v12.8h, v13.8h\n"	\
			"bfdot v9.4s, v12.8h, v13.8h\n"	\
			"bfdot v10.4s, v12.8h, v13.8h\n"	\
			"bfdot v11.4s, v12.8h, v13.8h\n"


static uint64_t NEON_BFDOT_S4_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v12.8h, w2\n"
		"dup	v13.8h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S4_IR12_12()
		BFDOT_S4_IR12_12()
		BFDOT_S4_IR12_12()
		BFDOT_S4_IR12_12()
		BFDOT_S4_IR12_12()
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
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// bfdot.4s  Interleave 1  (dependent chain, latency)
//-----------------------------------------------------------------------------

#define BFDOT_S4_IR1_8() \
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"		\
			"bfdot v0.4s, v8.8h, v9.8h\n"


#if !VB_MINIMUM_ARM_BF16
static uint64_t NEON_BFDOT_S4_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v8.8h, w2\n"
		"dup	v9.8h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S4_IR1_8()
		BFDOT_S4_IR1_8()
		BFDOT_S4_IR1_8()
		BFDOT_S4_IR1_8()
		BFDOT_S4_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}
#endif


//=============================================================================
// bfdot .2s  (64-bit)
//=============================================================================

//-----------------------------------------------------------------------------
// bfdot.2s  Interleave 8
//-----------------------------------------------------------------------------

#define BFDOT_S2_IR8_8() \
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v1.2s, v8.4h, v9.4h\n"		\
			"bfdot v2.2s, v8.4h, v9.4h\n"		\
			"bfdot v3.2s, v8.4h, v9.4h\n"		\
			"bfdot v4.2s, v8.4h, v9.4h\n"		\
			"bfdot v5.2s, v8.4h, v9.4h\n"		\
			"bfdot v6.2s, v8.4h, v9.4h\n"		\
			"bfdot v7.2s, v8.4h, v9.4h\n"


#if !VB_MINIMUM_ARM_BF16
static uint64_t NEON_BFDOT_S2_IR8( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v8.4h, w2\n"
		"dup	v9.4h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S2_IR8_8()
		BFDOT_S2_IR8_8()
		BFDOT_S2_IR8_8()
		BFDOT_S2_IR8_8()
		BFDOT_S2_IR8_8()
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
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// bfdot.2s  Interleave 12
//-----------------------------------------------------------------------------

#define BFDOT_S2_IR12_12() \
			"bfdot v0.2s, v12.4h, v13.4h\n"	\
			"bfdot v1.2s, v12.4h, v13.4h\n"	\
			"bfdot v2.2s, v12.4h, v13.4h\n"	\
			"bfdot v3.2s, v12.4h, v13.4h\n"	\
			"bfdot v4.2s, v12.4h, v13.4h\n"	\
			"bfdot v5.2s, v12.4h, v13.4h\n"	\
			"bfdot v6.2s, v12.4h, v13.4h\n"	\
			"bfdot v7.2s, v12.4h, v13.4h\n"	\
			"bfdot v8.2s, v12.4h, v13.4h\n"	\
			"bfdot v9.2s, v12.4h, v13.4h\n"	\
			"bfdot v10.2s, v12.4h, v13.4h\n"	\
			"bfdot v11.2s, v12.4h, v13.4h\n"


static uint64_t NEON_BFDOT_S2_IR12( CounterType LoopCount )
{
	uint32_t	ret0, ret1, ret2, ret3, ret4, ret5, ret6, ret7;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v12.4h, w2\n"
		"dup	v13.4h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S2_IR12_12()
		BFDOT_S2_IR12_12()
		BFDOT_S2_IR12_12()
		BFDOT_S2_IR12_12()
		BFDOT_S2_IR12_12()
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
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// bfdot.2s  Interleave 1  (dependent chain, latency)
//-----------------------------------------------------------------------------

#define BFDOT_S2_IR1_8() \
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"		\
			"bfdot v0.2s, v8.4h, v9.4h\n"


#if !VB_MINIMUM_ARM_BF16
static uint64_t NEON_BFDOT_S2_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		NEON_REG_CLEAR()
		"mov	w2, #0x3f80\n"
		"dup	v8.4h, w2\n"
		"dup	v9.4h, w2\n"
		"mov	x0, %[loop]\n"
		"mov	w0, w0\n"
	"1:\n"
		BFDOT_S2_IR1_8()
		BFDOT_S2_IR1_8()
		BFDOT_S2_IR1_8()
		BFDOT_S2_IR1_8()
		BFDOT_S2_IR1_8()
		"subs	w0, w0, #1\n"
		"b.ne	1b\n"
		"str	s0, %[o0]\n"
	:	[o0]"=m"(ret0)
	: [loop]"r"(LoopCount)
	: "cc","x0","w2",
	"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9","v10","v11","v12","v13","v14","v15" );

	timer.End();
	timer.Dump( "bfdot" );
	return	timer.Result();
}
#endif


#endif // USE_ARM_BF16


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static const char*	Instruction_Title[]= {
	"NEON bfmmla.4s (bf16 x8) n12",

#if !VB_MINIMUM_ARM_BF16
	"NEON bfdot.2s (bf16 x4) n8",
#endif
	"NEON bfdot.2s (bf16 x4) n12",
#if !VB_MINIMUM_ARM_BF16
	"NEON bfdot.2s (bf16 x4) n1",
#endif

#if !VB_MINIMUM_ARM_BF16
	"NEON bfdot.4s (bf16 x8) n8",
#endif
	"NEON bfdot.4s (bf16 x8) n12",
#if !VB_MINIMUM_ARM_BF16
	"NEON bfdot.4s (bf16 x8) n1",
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

	// bfmmla FOP=32: 4 outputs x (4 mul + 4 add)
	SetOp2( RESULT_NEON_BFMMLA_IR12,	PER_LOOP_INST_12, 32 );

	// bfdot.2s FOP=8: 2 outputs x (2 mul + 2 add)
#if !VB_MINIMUM_ARM_BF16
	SetOp2( RESULT_NEON_BFDOT_S2_IR8,	PER_LOOP_INST,    8  );
#endif
	SetOp2( RESULT_NEON_BFDOT_S2_IR12,	PER_LOOP_INST_12, 8  );
#if !VB_MINIMUM_ARM_BF16
	SetOp2( RESULT_NEON_BFDOT_S2_IR1,	PER_LOOP_INST,    8  );
#endif

	// bfdot.4s FOP=16: 4 outputs x (2 mul + 2 add)
#if !VB_MINIMUM_ARM_BF16
	SetOp2( RESULT_NEON_BFDOT_S4_IR8,	PER_LOOP_INST,    16 );
#endif
	SetOp2( RESULT_NEON_BFDOT_S4_IR12,	PER_LOOP_INST_12, 16 );
#if !VB_MINIMUM_ARM_BF16
	SetOp2( RESULT_NEON_BFDOT_S4_IR1,	PER_LOOP_INST,    16 );
#endif
}


void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "VFP64BF16 loop=%d\n", Loop );

#if USE_ARM_BF16
	if( Info.HasInstructionSet( CPUFeature::ARM_BF16 ) ){

		SetResult( RESULT_NEON_BFMMLA_IR12,		NEON_BFMMLA_IR12(  Loop ) );
		Progress.Increment();


# if !VB_MINIMUM_ARM_BF16
		SetResult( RESULT_NEON_BFDOT_S2_IR8,	NEON_BFDOT_S2_IR8(  Loop ) );
		Progress.Increment();
# endif

		SetResult( RESULT_NEON_BFDOT_S2_IR12,	NEON_BFDOT_S2_IR12( Loop ) );
		Progress.Increment();

# if !VB_MINIMUM_ARM_BF16
		SetResult( RESULT_NEON_BFDOT_S2_IR1,	NEON_BFDOT_S2_IR1(  Loop ) );
		Progress.Increment();
# endif


# if !VB_MINIMUM_ARM_BF16
		SetResult( RESULT_NEON_BFDOT_S4_IR8,	NEON_BFDOT_S4_IR8(  Loop ) );
		Progress.Increment();
# endif

		SetResult( RESULT_NEON_BFDOT_S4_IR12,	NEON_BFDOT_S4_IR12( Loop ) );
		Progress.Increment();

# if !VB_MINIMUM_ARM_BF16
		SetResult( RESULT_NEON_BFDOT_S4_IR1,	NEON_BFDOT_S4_IR1(  Loop ) );
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
		return	"NEON (BF16)";
	}else{
		return	"NEON (BF16) multi-thread";
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
