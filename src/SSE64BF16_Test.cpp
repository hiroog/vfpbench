// 2025 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE64BF16_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_X64
//-----------------------------------------------------------------------------

#ifndef	VB_MINIMUM_BF16
# define	VB_MINIMUM_BF16		1
#endif

#if defined(__AVX512F__) && __AVX512F__
# define	ENABLE_AVX512		1
#else
# define	ENABLE_AVX512		0
#endif

#if defined(__AVX512BF16__) && __AVX512BF16__
# define	ENABLE_AVX512BF16	1
#else
# define	ENABLE_AVX512BF16	0
#endif

using namespace flatlib;
using system::CPUFeature;

namespace SSE64BF16 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	vdpbf16ps ymm1, ymm2, ymm3   (256-bit, AVX512-BF16 EVEX or AVX-VNNI-BF16 VEX)
	  ymm1[i] += BF16_to_FP32(ymm2[2i]) * BF16_to_FP32(ymm3[2i])
	           + BF16_to_FP32(ymm2[2i+1]) * BF16_to_FP32(ymm3[2i+1])
	  FLOPs: 8 outputs x (2 mul + 2 add) = 32

	vdpbf16ps zmm1, zmm2, zmm3   (512-bit, AVX512-BF16)
	  FLOPs: 16 outputs x (2 mul + 2 add) = 64
*/


typedef	uint64_t	CounterType;


//=============================================================================
// AVX512-BF16: vdpbf16ps ymm  (256-bit)
//=============================================================================

#if ENABLE_AVX512BF16

#define YMM_REG_CLEAR() \
		"vpxor	%%ymm0,  %%ymm0,  %%ymm0\n"	\
		"vpxor	%%ymm1,  %%ymm1,  %%ymm1\n"	\
		"vpxor	%%ymm2,  %%ymm2,  %%ymm2\n"	\
		"vpxor	%%ymm3,  %%ymm3,  %%ymm3\n"	\
		"vpxor	%%ymm4,  %%ymm4,  %%ymm4\n"	\
		"vpxor	%%ymm5,  %%ymm5,  %%ymm5\n"	\
		"vpxor	%%ymm6,  %%ymm6,  %%ymm6\n"	\
		"vpxor	%%ymm7,  %%ymm7,  %%ymm7\n"	\
		"vpxor	%%ymm8,  %%ymm8,  %%ymm8\n"	\
		"vpxor	%%ymm9,  %%ymm9,  %%ymm9\n"	\
		"vpxor	%%ymm10, %%ymm10, %%ymm10\n"	\
		"vpxor	%%ymm11, %%ymm11, %%ymm11\n"	\
		"vpxor	%%ymm12, %%ymm12, %%ymm12\n"	\
		"vpxor	%%ymm13, %%ymm13, %%ymm13\n"	\
		"vpxor	%%ymm14, %%ymm14, %%ymm14\n"	\
		"vpxor	%%ymm15, %%ymm15, %%ymm15\n"


//-----------------------------------------------------------------------------
// vdpbf16ps ymm  Interleave 8  (8 independent accumulators)
//-----------------------------------------------------------------------------

#define VDPBF16PS_YMM_IR8_8() \
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm1\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm2\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm3\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm4\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm5\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm6\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm7\n"


#if !VB_MINIMUM_BF16
static uint64_t AVX_VDPBF16PS_YMM_IR8( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_YMM_IR8_8()
		VDPBF16PS_YMM_IR8_8()
		VDPBF16PS_YMM_IR8_8()
		VDPBF16PS_YMM_IR8_8()
		VDPBF16PS_YMM_IR8_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	"cc","%rax","%eax",
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" );
	timer.End();
	timer.Dump( "vdpbf16ps ymm ir8" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// vdpbf16ps ymm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VDPBF16PS_YMM_IR12_12() \
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm1\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm2\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm3\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm4\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm5\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm6\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm7\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm8\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm9\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm10\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm11\n"


static uint64_t AVX_VDPBF16PS_YMM_IR12( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_YMM_IR12_12()
		VDPBF16PS_YMM_IR12_12()
		VDPBF16PS_YMM_IR12_12()
		VDPBF16PS_YMM_IR12_12()
		VDPBF16PS_YMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	"cc","%rax","%eax",
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" );
	timer.End();
	timer.Dump( "vdpbf16ps ymm ir12" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// vdpbf16ps ymm  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define VDPBF16PS_YMM_IR1_8() \
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vdpbf16ps	%%ymm15, %%ymm14, %%ymm0\n"


#if !VB_MINIMUM_BF16
static uint64_t AVX_VDPBF16PS_YMM_IR1( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_YMM_IR1_8()
		VDPBF16PS_YMM_IR1_8()
		VDPBF16PS_YMM_IR1_8()
		VDPBF16PS_YMM_IR1_8()
		VDPBF16PS_YMM_IR1_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	"cc","%rax","%eax",
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15" );
	timer.End();
	timer.Dump( "vdpbf16ps ymm ir1" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}
#endif


//=============================================================================
// AVX512-BF16: vdpbf16ps zmm  (512-bit)
//=============================================================================

#if ENABLE_AVX512

#define ZMM_REG_CLEAR() \
		"vxorps	%%zmm0,  %%zmm0,  %%zmm0\n"	\
		"vxorps	%%zmm1,  %%zmm1,  %%zmm1\n"	\
		"vxorps	%%zmm2,  %%zmm2,  %%zmm2\n"	\
		"vxorps	%%zmm3,  %%zmm3,  %%zmm3\n"	\
		"vxorps	%%zmm4,  %%zmm4,  %%zmm4\n"	\
		"vxorps	%%zmm5,  %%zmm5,  %%zmm5\n"	\
		"vxorps	%%zmm6,  %%zmm6,  %%zmm6\n"	\
		"vxorps	%%zmm7,  %%zmm7,  %%zmm7\n"	\
		"vxorps	%%zmm8,  %%zmm8,  %%zmm8\n"	\
		"vxorps	%%zmm9,  %%zmm9,  %%zmm9\n"	\
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
		"vxorps	%%zmm31, %%zmm31, %%zmm31\n"

#define ZMM_CLOBBER \
		"cc","%rax","%eax",	\
		"%zmm0","%zmm1","%zmm2","%zmm3","%zmm4","%zmm5","%zmm6","%zmm7",	\
		"%zmm8","%zmm9","%zmm10","%zmm11","%zmm12","%zmm13","%zmm14","%zmm15",	\
		"%zmm16","%zmm17","%zmm18","%zmm19","%zmm20","%zmm21","%zmm22","%zmm23",	\
		"%zmm24","%zmm25","%zmm26","%zmm27","%zmm28","%zmm29","%zmm30","%zmm31",	\
		"%k1","%k2","%k3","%k4","%k5","%k6","%k7"


//-----------------------------------------------------------------------------
// vdpbf16ps zmm  Interleave 8  (8 independent accumulators)
//-----------------------------------------------------------------------------

#define VDPBF16PS_ZMM_IR8_8() \
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm1\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm2\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm3\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm4\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm5\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm6\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm7\n"


#if !VB_MINIMUM_BF16
static uint64_t AVX512_VDPBF16PS_ZMM_IR8( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_ZMM_IR8_8()
		VDPBF16PS_ZMM_IR8_8()
		VDPBF16PS_ZMM_IR8_8()
		VDPBF16PS_ZMM_IR8_8()
		VDPBF16PS_ZMM_IR8_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vdpbf16ps zmm ir8" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// vdpbf16ps zmm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VDPBF16PS_ZMM_IR12_12() \
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm1\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm2\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm3\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm4\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm5\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm6\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm7\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm8\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm9\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm10\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm11\n"


static uint64_t AVX512_VDPBF16PS_ZMM_IR12( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_ZMM_IR12_12()
		VDPBF16PS_ZMM_IR12_12()
		VDPBF16PS_ZMM_IR12_12()
		VDPBF16PS_ZMM_IR12_12()
		VDPBF16PS_ZMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vdpbf16ps zmm ir12" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// vdpbf16ps zmm  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define VDPBF16PS_ZMM_IR1_8() \
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vdpbf16ps	%%zmm13, %%zmm12, %%zmm0\n"


#if !VB_MINIMUM_BF16
static uint64_t AVX512_VDPBF16PS_ZMM_IR1( CounterType LoopCount )
{
	float	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x3f803f80, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VDPBF16PS_ZMM_IR1_8()
		VDPBF16PS_ZMM_IR1_8()
		VDPBF16PS_ZMM_IR1_8()
		VDPBF16PS_ZMM_IR1_8()
		VDPBF16PS_ZMM_IR1_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovss	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vdpbf16ps zmm ir1" );
	FL_LOG( "%f\n", ret0 );
	return	timer.Result();
}
#endif

#endif // ENABLE_AVX512
#endif // ENABLE_AVX512BF16


//=============================================================================
//=============================================================================

static const char*	Instruction_Title[]= {
#if !VB_MINIMUM_BF16
	"AVX512-BF16 vdpbf16ps y(bf16 x16) n8",
#endif
	"AVX512-BF16 vdpbf16ps y(bf16 x16)",	// n12
#if !VB_MINIMUM_BF16
	"AVX512-BF16 vdpbf16ps y(bf16 x16) n1",
#endif

#if !VB_MINIMUM_BF16
	"AVX512-BF16 vdpbf16ps z(bf16 x32) n8",
#endif
	"AVX512-BF16 vdpbf16ps z(bf16 x32)",	// n12
#if !VB_MINIMUM_BF16
	"AVX512-BF16 vdpbf16ps z(bf16 x32) n1",
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

	// vdpbf16ps ymm FOP=32: 8 outputs x (2 mul + 2 add)
#if !VB_MINIMUM_BF16
	SetOp2( RESULT_AVX512_VDPBF16PS_YMM_IR8,  PER_LOOP_INST,    32 );
#endif
	SetOp2( RESULT_AVX512_VDPBF16PS_YMM_IR12, PER_LOOP_INST_12, 32 );
#if !VB_MINIMUM_BF16
	SetOp2( RESULT_AVX512_VDPBF16PS_YMM_IR1,  PER_LOOP_INST,    32 );
#endif

	// vdpbf16ps zmm FOP=64: 16 outputs x (2 mul + 2 add)
#if !VB_MINIMUM_BF16
	SetOp2( RESULT_AVX512_VDPBF16PS_ZMM_IR8,  PER_LOOP_INST,    64 );
#endif
	SetOp2( RESULT_AVX512_VDPBF16PS_ZMM_IR12, PER_LOOP_INST_12, 64 );
#if !VB_MINIMUM_BF16
	SetOp2( RESULT_AVX512_VDPBF16PS_ZMM_IR1,  PER_LOOP_INST,    64 );
#endif
}


void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "SSE64BF16 loop=%d\n", Loop );

#if ENABLE_AVX512BF16
	if( Info.HasInstructionSet( CPUFeature::IA_AVX512BF16 ) ){

# if !VB_MINIMUM_BF16
		SetResult( RESULT_AVX512_VDPBF16PS_YMM_IR8,  AVX_VDPBF16PS_YMM_IR8(  Loop ) );
		Progress.Increment();
# endif

		SetResult( RESULT_AVX512_VDPBF16PS_YMM_IR12, AVX_VDPBF16PS_YMM_IR12( Loop ) );
		Progress.Increment();

# if !VB_MINIMUM_BF16
		SetResult( RESULT_AVX512_VDPBF16PS_YMM_IR1,  AVX_VDPBF16PS_YMM_IR1(  Loop ) );
		Progress.Increment();
# endif


# if ENABLE_AVX512

#  if !VB_MINIMUM_BF16
		SetResult( RESULT_AVX512_VDPBF16PS_ZMM_IR8,  AVX512_VDPBF16PS_ZMM_IR8(  Loop ) );
		Progress.Increment();
#  endif

		SetResult( RESULT_AVX512_VDPBF16PS_ZMM_IR12, AVX512_VDPBF16PS_ZMM_IR12( Loop ) );
		Progress.Increment();

#  if !VB_MINIMUM_BF16
		SetResult( RESULT_AVX512_VDPBF16PS_ZMM_IR1,  AVX512_VDPBF16PS_ZMM_IR1(  Loop ) );
		Progress.Increment();
#  endif

# else
		Progress.LoadAdd( 3 );
# endif // ENABLE_AVX512

	}else
#endif
	{
#if !VB_MINIMUM_BF16
		Progress.LoadAdd( 6 );
#else
		Progress.LoadAdd( 2 );
#endif
	}

	FL_ASSERT( Progress.Get() == RESULT_MAX );

	DoneFlag.Set( 1 );
}


const char*	FloatTest::GetTestName() const
{
	if( !IsMultithread() ){
		return	"SSE/AVX (BF16)";
	}else{
		return	"SSE/AVX (BF16) multi-thread";
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
#endif // x64
//-----------------------------------------------------------------------------
