// 2025 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/SystemInfo.h>
#include	"TimerClass.h"
#include	"SSE64INT8_Test.h"


//-----------------------------------------------------------------------------
#if FL_CPU_X64
//-----------------------------------------------------------------------------

#ifndef	VB_MINIMUM_INT8
# define	VB_MINIMUM_INT8		1
#endif

#if defined(__AVX512F__) && __AVX512F__
# define	ENABLE_AVX512		1
#else
# define	ENABLE_AVX512		0
#endif

#if defined(__AVX512VNNI__) && __AVX512VNNI__
# define	ENABLE_AVX512VNNI	1
#else
# define	ENABLE_AVX512VNNI	0
#endif

#if defined(__AVXVNNI__) && __AVXVNNI__
# define	ENABLE_AVXVNNI_1	1
#else
# define	ENABLE_AVXVNNI_1	0
#endif

#define		ENABLE_AVXVNNI		(ENABLE_AVX512VNNI|ENABLE_AVXVNNI_1)

#if defined(__SSSE3__) && __SSSE3__
# define	ENABLE_SSSE3		1
#else
# define	ENABLE_SSSE3		0
#endif

#if defined(__AVX__) && __AVX__
# define	ENABLE_AVX			1
#else
# define	ENABLE_AVX			0
#endif

#if defined(__AVX2__) && __AVX2__
# define	ENABLE_AVX2			1
#else
# define	ENABLE_AVX2			0
#endif

#if defined(__AVX512BW__) && __AVX512BW__
# define	ENABLE_AVX512BW		1
#else
# define	ENABLE_AVX512BW		0
#endif

using namespace flatlib;
using system::CPUFeature;

namespace SSE64INT8 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	vpdpbusd ymm1, ymm2, ymm3   (256-bit, AVX-VNNI or AVX512-VNNI)
	  ymm1[i] += sum_k( (u8)ymm2[4i+k] * (s8)ymm3[4i+k] )  for k=0..3
	  OPS: 8 outputs x (4 mul + 4 add) = 64

	vpdpbusd zmm1, zmm2, zmm3   (512-bit, AVX512-VNNI)
	  OPS: 16 outputs x (4 mul + 4 add) = 128

	Note: columns labelled "MFLOPS" in the result table show MOPS for integer tests.
*/


typedef	uint64_t	CounterType;


//=============================================================================
// AVX-VNNI: vpdpbusd ymm  (256-bit)
//=============================================================================

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

#define YMM_CLOBBER \
		"cc","%rax","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",	\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"


#if ENABLE_AVXVNNI

//-----------------------------------------------------------------------------
// vpdpbusd ymm  Interleave 8  (8 independent accumulators)
//-----------------------------------------------------------------------------

#if ENABLE_AVX512VNNI
# define VPDPBUSD_YMM_IR8_8() \
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm1\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm2\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm3\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm4\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm5\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm6\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm7\n"
#else
# define VPDPBUSD_YMM_IR8_8() \
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm1\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm2\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm3\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm4\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm5\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm6\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm7\n"
#endif


#if !VB_MINIMUM_INT8
static uint64_t AVX_VPDPBUSD_YMM_IR8( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPDPBUSD_YMM_IR8_8()
		VPDPBUSD_YMM_IR8_8()
		VPDPBUSD_YMM_IR8_8()
		VPDPBUSD_YMM_IR8_8()
		VPDPBUSD_YMM_IR8_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd ymm ir8" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// vpdpbusd ymm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define vex_VPDPBUSD_YMM_IR12_12() \
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm1\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm2\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm3\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm4\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm5\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm6\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm7\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm8\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm9\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm10\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm11\n"


static uint64_t AVX_VPDPBUSD_YMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		vex_VPDPBUSD_YMM_IR12_12()
		vex_VPDPBUSD_YMM_IR12_12()
		vex_VPDPBUSD_YMM_IR12_12()
		vex_VPDPBUSD_YMM_IR12_12()
		vex_VPDPBUSD_YMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd ymm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}



//-----------------------------------------------------------------------------
// vpdpbusd ymm  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#if ENABLE_AVX512VNNI
# define VPDPBUSD_YMM_IR1_8() \
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"
#else
# define VPDPBUSD_YMM_IR1_8() \
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{vex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"
#endif

#if !VB_MINIMUM_INT8
static uint64_t AVX_VPDPBUSD_YMM_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPDPBUSD_YMM_IR1_8()
		VPDPBUSD_YMM_IR1_8()
		VPDPBUSD_YMM_IR1_8()
		VPDPBUSD_YMM_IR1_8()
		VPDPBUSD_YMM_IR1_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd ymm ir1" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}
#endif

#endif // ENABLE_AVXVNNI



//=============================================================================
// AVX512-VNNI: vpdpbusd ymm  (256-bit)
//=============================================================================

#if ENABLE_AVX512VNNI

#define evex_VPDPBUSD_YMM_IR12_12() \
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm0\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm1\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm2\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm3\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm4\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm5\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm6\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm7\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm8\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm9\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm10\n"	\
			"%{evex%} vpdpbusd	%%ymm15, %%ymm14, %%ymm11\n"


static uint64_t AVX512_VPDPBUSD_YMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		evex_VPDPBUSD_YMM_IR12_12()
		evex_VPDPBUSD_YMM_IR12_12()
		evex_VPDPBUSD_YMM_IR12_12()
		evex_VPDPBUSD_YMM_IR12_12()
		evex_VPDPBUSD_YMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd ymm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}

#endif


//=============================================================================
// AVX512-VNNI: vpdpbusd zmm  (512-bit)
//=============================================================================

#if ENABLE_AVX512VNNI && ENABLE_AVX512

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
// vpdpbusd zmm  Interleave 8  (8 independent accumulators)
//-----------------------------------------------------------------------------

#define VPDPBUSD_ZMM_IR8_8() \
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm1\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm2\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm3\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm4\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm5\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm6\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm7\n"


#if !VB_MINIMUM_INT8
static uint64_t AVX512_VPDPBUSD_ZMM_IR8( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPDPBUSD_ZMM_IR8_8()
		VPDPBUSD_ZMM_IR8_8()
		VPDPBUSD_ZMM_IR8_8()
		VPDPBUSD_ZMM_IR8_8()
		VPDPBUSD_ZMM_IR8_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd zmm ir8" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}
#endif


//-----------------------------------------------------------------------------
// vpdpbusd zmm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VPDPBUSD_ZMM_IR12_12() \
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm1\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm2\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm3\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm4\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm5\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm6\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm7\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm8\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm9\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm10\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm11\n"


static uint64_t AVX512_VPDPBUSD_ZMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPDPBUSD_ZMM_IR12_12()
		VPDPBUSD_ZMM_IR12_12()
		VPDPBUSD_ZMM_IR12_12()
		VPDPBUSD_ZMM_IR12_12()
		VPDPBUSD_ZMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd zmm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// vpdpbusd zmm  Interleave 1  (latency)
//-----------------------------------------------------------------------------

#define VPDPBUSD_ZMM_IR1_8() \
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpdpbusd	%%zmm13, %%zmm12, %%zmm0\n"


#if !VB_MINIMUM_INT8
static uint64_t AVX512_VPDPBUSD_ZMM_IR1( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPDPBUSD_ZMM_IR1_8()
		VPDPBUSD_ZMM_IR1_8()
		VPDPBUSD_ZMM_IR1_8()
		VPDPBUSD_ZMM_IR1_8()
		VPDPBUSD_ZMM_IR1_8()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vpdpbusd zmm ir1" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}
#endif

#endif // ENABLE_AVX512VNNI && ENABLE_AVX512


//=============================================================================
// SSSE3: pmaddubsw xmm  (128-bit, legacy SSE, 2-operand)
//=============================================================================

#if ENABLE_SSSE3

/*
	pmaddubsw xmm1, xmm2   (128-bit, SSSE3)
	  xmm1[i] = saturate16( (u8)xmm1[2i] * (s8)xmm2[2i] + (u8)xmm1[2i+1] * (s8)xmm2[2i+1] )
	  OPS: 16 mul + 8 add = 24
*/

#define XMM_REG_CLEAR() \
		"pxor	%%xmm0,  %%xmm0\n"	\
		"pxor	%%xmm1,  %%xmm1\n"	\
		"pxor	%%xmm2,  %%xmm2\n"	\
		"pxor	%%xmm3,  %%xmm3\n"	\
		"pxor	%%xmm4,  %%xmm4\n"	\
		"pxor	%%xmm5,  %%xmm5\n"	\
		"pxor	%%xmm6,  %%xmm6\n"	\
		"pxor	%%xmm7,  %%xmm7\n"	\
		"pxor	%%xmm8,  %%xmm8\n"	\
		"pxor	%%xmm9,  %%xmm9\n"	\
		"pxor	%%xmm10, %%xmm10\n"	\
		"pxor	%%xmm11, %%xmm11\n"	\
		"pxor	%%xmm12, %%xmm12\n"	\
		"pxor	%%xmm13, %%xmm13\n"	\
		"pxor	%%xmm14, %%xmm14\n"	\
		"pxor	%%xmm15, %%xmm15\n"

#define XMM_CLOBBER \
		"cc","%rax","%eax",	\
		"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7",	\
		"%xmm8","%xmm9","%xmm10","%xmm11","%xmm12","%xmm13","%xmm14","%xmm15"

//-----------------------------------------------------------------------------
// pmaddubsw xmm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define PMADDUBSW_XMM_IR12_12() \
			"pmaddubsw	%%xmm15, %%xmm0\n"		\
			"pmaddubsw	%%xmm15, %%xmm1\n"		\
			"pmaddubsw	%%xmm15, %%xmm2\n"		\
			"pmaddubsw	%%xmm15, %%xmm3\n"		\
			"pmaddubsw	%%xmm15, %%xmm4\n"		\
			"pmaddubsw	%%xmm15, %%xmm5\n"		\
			"pmaddubsw	%%xmm15, %%xmm6\n"		\
			"pmaddubsw	%%xmm15, %%xmm7\n"		\
			"pmaddubsw	%%xmm15, %%xmm8\n"		\
			"pmaddubsw	%%xmm15, %%xmm9\n"		\
			"pmaddubsw	%%xmm15, %%xmm10\n"		\
			"pmaddubsw	%%xmm15, %%xmm11\n"

static uint64_t SSSE3_PMADDUBSW_XMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		XMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"movd	%%eax, %%xmm15\n"
		"punpcklbw	%%xmm15, %%xmm15\n"
		"punpcklwd	%%xmm15, %%xmm15\n"
		"pshufd	$0, %%xmm15, %%xmm15\n"
		"movdqa	%%xmm15, %%xmm0\n"
		"movdqa	%%xmm15, %%xmm1\n"
		"movdqa	%%xmm15, %%xmm2\n"
		"movdqa	%%xmm15, %%xmm3\n"
		"movdqa	%%xmm15, %%xmm4\n"
		"movdqa	%%xmm15, %%xmm5\n"
		"movdqa	%%xmm15, %%xmm6\n"
		"movdqa	%%xmm15, %%xmm7\n"
		"movdqa	%%xmm15, %%xmm8\n"
		"movdqa	%%xmm15, %%xmm9\n"
		"movdqa	%%xmm15, %%xmm10\n"
		"movdqa	%%xmm15, %%xmm11\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		PMADDUBSW_XMM_IR12_12()
		PMADDUBSW_XMM_IR12_12()
		PMADDUBSW_XMM_IR12_12()
		PMADDUBSW_XMM_IR12_12()
		PMADDUBSW_XMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"movd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	XMM_CLOBBER );
	timer.End();
	timer.Dump( "pmaddubsw xmm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}

#endif // ENABLE_SSSE3


//=============================================================================
// AVX: vpmaddubsw xmm  (128-bit, VEX 3-operand)
//=============================================================================

#if ENABLE_AVX

//-----------------------------------------------------------------------------
// vpmaddubsw xmm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VPMADDUBSW_XMM_IR12_12() \
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm0\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm1\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm2\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm3\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm4\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm5\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm6\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm7\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm8\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm9\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm10\n"	\
			"%{vex%} vpmaddubsw	%%xmm15, %%xmm14, %%xmm11\n"

static uint64_t AVX_VPMADDUBSW_XMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%xmm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPMADDUBSW_XMM_IR12_12()
		VPMADDUBSW_XMM_IR12_12()
		VPMADDUBSW_XMM_IR12_12()
		VPMADDUBSW_XMM_IR12_12()
		VPMADDUBSW_XMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpmaddubsw xmm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}

#endif // ENABLE_AVX


//=============================================================================
// AVX2: vpmaddubsw ymm  (256-bit)
//=============================================================================

#if ENABLE_AVX2

//-----------------------------------------------------------------------------
// vpmaddubsw ymm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VPMADDUBSW_YMM_IR12_12() \
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm0\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm1\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm2\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm3\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm4\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm5\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm6\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm7\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm8\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm9\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm10\n"	\
			"vpmaddubsw	%%ymm15, %%ymm14, %%ymm11\n"

static uint64_t AVX2_VPMADDUBSW_YMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		YMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm14\n"
		"vpbroadcastd	%%xmm14, %%ymm14\n"
		"vpbroadcastd	%%xmm14, %%ymm15\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPMADDUBSW_YMM_IR12_12()
		VPMADDUBSW_YMM_IR12_12()
		VPMADDUBSW_YMM_IR12_12()
		VPMADDUBSW_YMM_IR12_12()
		VPMADDUBSW_YMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	YMM_CLOBBER );
	timer.End();
	timer.Dump( "vpmaddubsw ymm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}

#endif // ENABLE_AVX2


//=============================================================================
// AVX512BW: vpmaddubsw zmm  (512-bit)
//=============================================================================

#if ENABLE_AVX512BW && ENABLE_AVX512

/*
	vpmaddubsw zmm1, zmm2, zmm3   (512-bit, AVX512BW)
	  OPS: 64 mul + 32 add = 96
*/

//-----------------------------------------------------------------------------
// vpmaddubsw zmm  Interleave 12  (12 independent accumulators)
//-----------------------------------------------------------------------------

#define VPMADDUBSW_ZMM_IR12_12() \
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm0\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm1\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm2\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm3\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm4\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm5\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm6\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm7\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm8\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm9\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm10\n"	\
			"vpmaddubsw	%%zmm13, %%zmm12, %%zmm11\n"

static uint64_t AVX512BW_VPMADDUBSW_ZMM_IR12( CounterType LoopCount )
{
	uint32_t	ret0;
	TimerClass	timer;
	timer.Begin();
	__asm__ __volatile__(
		ZMM_REG_CLEAR()
		"mov	$0x01010101, %%eax\n"
		"vmovd	%%eax, %%xmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm12\n"
		"vpbroadcastd	%%xmm12, %%zmm13\n"
		"mov	%[loop], %%rax\n"
	"1:\n"
		VPMADDUBSW_ZMM_IR12_12()
		VPMADDUBSW_ZMM_IR12_12()
		VPMADDUBSW_ZMM_IR12_12()
		VPMADDUBSW_ZMM_IR12_12()
		VPMADDUBSW_ZMM_IR12_12()
		"dec	%%rax\n"
		"jne	1b\n"
		"vmovd	%%xmm0, %[ret0]\n"
	:	[ret0]"=m"(ret0)
	:	[loop]"r"(LoopCount)
	:	ZMM_CLOBBER );
	timer.End();
	timer.Dump( "vpmaddubsw zmm ir12" );
	FL_LOG( "%u\n", ret0 );
	return	timer.Result();
}

#endif // ENABLE_AVX512BW && ENABLE_AVX512


//=============================================================================
//=============================================================================

static const char*	Instruction_Title[]= {
	"SSSE3     pmaddubsw  x(i8bit x16)",		// n12
	"AVX       vpmaddubsw x(i8bit x16)",		// n12
	"AVX2      vpmaddubsw y(i8bit x32)",		// n12
	"AVX512    vpmaddubsw z(i8bit x64)",		// n12

#if !VB_MINIMUM_INT8
	"AVX-VNNI    vpdpbusd y(i8bit x32) n8",
#endif
	"AVX-VNNI    vpdpbusd y(i8bit x32)",		// n12
#if !VB_MINIMUM_INT8
	"AVX-VNNI    vpdpbusd y(i8bit x32) n1",
#endif

	"AVX512-VNNI vpdpbusd y(i8bit x32)",		// n12

#if !VB_MINIMUM_INT8
	"AVX512-VNNI vpdpbusd z(i8bit x64) n8",
#endif
	"AVX512-VNNI vpdpbusd z(i8bit x64)",	// n12
#if !VB_MINIMUM_INT8
	"AVX512-VNNI vpdpbusd z(i8bit x64) n1",
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

	// pmaddubsw xmm OPS=24: 16 mul + 8 add
	SetOp2( RESULT_SSSE3_PMADDUBSW_XMM_IR12,     PER_LOOP_INST_12, 24 );
	// vpmaddubsw xmm OPS=24: 16 mul + 8 add
	SetOp2( RESULT_AVX_VPMADDUBSW_XMM_IR12,      PER_LOOP_INST_12, 24 );
	// vpmaddubsw ymm OPS=48: 32 mul + 16 add
	SetOp2( RESULT_AVX2_VPMADDUBSW_YMM_IR12,     PER_LOOP_INST_12, 48 );
	// vpmaddubsw zmm OPS=96: 64 mul + 32 add
	SetOp2( RESULT_AVX512BW_VPMADDUBSW_ZMM_IR12, PER_LOOP_INST_12, 96 );

	// vpdpbusd ymm OPS=64: 8 outputs x (4 mul + 4 add)
#if !VB_MINIMUM_INT8
	SetOp2( RESULT_AVX_VPDPBUSD_YMM_IR8,  PER_LOOP_INST,    64 );
#endif
	SetOp2( RESULT_AVX_VPDPBUSD_YMM_IR12, PER_LOOP_INST_12, 64 );
#if !VB_MINIMUM_INT8
	SetOp2( RESULT_AVX_VPDPBUSD_YMM_IR1,  PER_LOOP_INST,    64 );
#endif

	// vpdpbusd ymm OPS=64: 8 outputs x (4 mul + 4 add)
	SetOp2( RESULT_AVX512_VPDPBUSD_YMM_IR12, PER_LOOP_INST_12, 64 );

	// vpdpbusd zmm OPS=128: 16 outputs x (4 mul + 4 add)
#if !VB_MINIMUM_INT8
	SetOp2( RESULT_AVX512_VPDPBUSD_ZMM_IR8,  PER_LOOP_INST,    128 );
#endif
	SetOp2( RESULT_AVX512_VPDPBUSD_ZMM_IR12, PER_LOOP_INST_12, 128 );
#if !VB_MINIMUM_INT8
	SetOp2( RESULT_AVX512_VPDPBUSD_ZMM_IR1,  PER_LOOP_INST,    128 );
#endif
}


void FloatTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "SSE64INT8 loop=%d\n", Loop );


#if ENABLE_SSSE3
	if( Info.HasInstructionSet( CPUFeature::IA_SSSE3 ) ){
		SetResult( RESULT_SSSE3_PMADDUBSW_XMM_IR12, SSSE3_PMADDUBSW_XMM_IR12( Loop ) );
		Progress.Increment();
	}else
#endif
	{
		Progress.Increment();
	}

#if ENABLE_AVX
	if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){
		SetResult( RESULT_AVX_VPMADDUBSW_XMM_IR12, AVX_VPMADDUBSW_XMM_IR12( Loop ) );
		Progress.Increment();
	}else
#endif
	{
		Progress.Increment();
	}

#if ENABLE_AVX2
	if( Info.HasInstructionSet( CPUFeature::IA_AVX2 ) ){
		SetResult( RESULT_AVX2_VPMADDUBSW_YMM_IR12, AVX2_VPMADDUBSW_YMM_IR12( Loop ) );
		Progress.Increment();
	}else
#endif
	{
		Progress.Increment();
	}

#if ENABLE_AVX512BW && ENABLE_AVX512
	if( Info.HasInstructionSet( CPUFeature::IA_AVX512BW ) ){
		SetResult( RESULT_AVX512BW_VPMADDUBSW_ZMM_IR12, AVX512BW_VPMADDUBSW_ZMM_IR12( Loop ) );
		Progress.Increment();
	}else
#endif
	{
		Progress.Increment();
	}


#if ENABLE_AVXVNNI
	if( Info.HasInstructionSet( CPUFeature::IA_AVXVNNI ) ){

# if !VB_MINIMUM_INT8
		SetResult( RESULT_AVX_VPDPBUSD_YMM_IR8,  AVX_VPDPBUSD_YMM_IR8(  Loop ) );
		Progress.Increment();
# endif

		SetResult( RESULT_AVX_VPDPBUSD_YMM_IR12, AVX_VPDPBUSD_YMM_IR12( Loop ) );
		Progress.Increment();

# if !VB_MINIMUM_INT8
		SetResult( RESULT_AVX_VPDPBUSD_YMM_IR1,  AVX_VPDPBUSD_YMM_IR1(  Loop ) );
		Progress.Increment();
# endif

	}else
#endif
	{
# if !VB_MINIMUM_INT8
		Progress.LoadAdd( 3 );
#else
		Progress.LoadAdd( 1 );
#endif
	}

#if ENABLE_AVX512VNNI && ENABLE_AVX512
	if( Info.HasInstructionSet( CPUFeature::IA_AVX512VNNI ) ){

		SetResult( RESULT_AVX512_VPDPBUSD_YMM_IR12, AVX512_VPDPBUSD_YMM_IR12( Loop ) );
		Progress.Increment();


# if !VB_MINIMUM_INT8
		SetResult( RESULT_AVX512_VPDPBUSD_ZMM_IR8,  AVX512_VPDPBUSD_ZMM_IR8(  Loop ) );
		Progress.Increment();
# endif

		SetResult( RESULT_AVX512_VPDPBUSD_ZMM_IR12, AVX512_VPDPBUSD_ZMM_IR12( Loop ) );
		Progress.Increment();

# if !VB_MINIMUM_INT8
		SetResult( RESULT_AVX512_VPDPBUSD_ZMM_IR1,  AVX512_VPDPBUSD_ZMM_IR1(  Loop ) );
		Progress.Increment();
# endif

	}else
#endif
	{
# if !VB_MINIMUM_INT8
		Progress.LoadAdd( 4 );
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
		return	"SSE/AVX (INT8)";
	}else{
		return	"SSE/AVX (INT8) multi-thread";
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
