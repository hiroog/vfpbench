// 2014/01/15 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/Matrix4.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"MatrixTest_SSESP.h"


//-----------------------------------------------------------------------------
#if FL_CPU_X86 || FL_CPU_X64
//-----------------------------------------------------------------------------

using namespace flatlib;

namespace SSESP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// SSE A 128bit
//-----------------------------------------------------------------------------

#if FL_CPU_X86

inline void Mul_SSE_A128( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%edx\n\
		mov	%[p1], %%eax\n\
		mov	%[p2], %%ecx\n\
\n\
		movaps	(%%eax), %%xmm4\n\
		movss	(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movaps	16(%%eax), %%xmm5\n\
		movss	4(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	32(%%eax), %%xmm6\n\
		movss	8(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	48(%%eax), %%xmm7\n\
		movss	12(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, (%%edx)\n\
\n\
		movss	16(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	20(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	24(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	28(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 16(%%edx)\n\
\n\
		movss	32(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	36(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	40(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	44(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 32(%%edx)\n\
\n\
		movss	48(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	52(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	56(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	60(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 48(%%edx)\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%eax", "%ecx", "%edx",
		"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7",
		"cc", "memory"
	);
}


#else	// FL_CPU_X64


inline void Mul_SSE_A128( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%rdx\n\
		mov	%[p1], %%rax\n\
		mov	%[p2], %%rcx\n\
\n\
		movaps	(%%rax), %%xmm4\n\
		movss	(%%rcx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movaps	16(%%rax), %%xmm5\n\
		movss	4(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	32(%%rax), %%xmm6\n\
		movss	8(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	48(%%rax), %%xmm7\n\
		movss	12(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, (%%rdx)\n\
\n\
		movss	16(%%rcx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	20(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	24(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	28(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 16(%%rdx)\n\
\n\
		movss	32(%%rcx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	36(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	40(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	44(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 32(%%rdx)\n\
\n\
		movss	48(%%rcx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	52(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm5, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	56(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	60(%%rcx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	%%xmm1, 48(%%rdx)\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%rax", "%rcx", "%rdx",
		"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7",
		"cc", "memory"
	);
}


#endif





#if 0


inline void Mul_SSE_A128( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%edx\n\
		mov	%[p1], %%eax\n\
		mov	%[p2], %%ecx\n\
\n\
		movaps	(%%eax), %%xmm4\n\
		movss	(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movaps	32(%%eax), %%xmm6\n\
		movss	8(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movaps	16(%%eax), %%xmm5\n\
		movss	4(%%ecx), %%xmm2\n\
		shufps	$0, %%xmm2, %%xmm2\n\
		mulps	%%xmm5, %%xmm2\n\
\n\
		movaps	48(%%eax), %%xmm7\n\
		movss	12(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm2\n\
\n\
		addps	%%xmm2, %%xmm1\n\
		movaps	%%xmm1, (%%edx)\n\
\n\
		movss	16(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	24(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	20(%%ecx), %%xmm2\n\
		shufps	$0, %%xmm2, %%xmm2\n\
		mulps	%%xmm5, %%xmm2\n\
\n\
		movss	28(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm2\n\
\n\
		addps	%%xmm2, %%xmm1\n\
		movaps	%%xmm1, 16(%%edx)\n\
\n\
		movss	32(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	40(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	36(%%ecx), %%xmm2\n\
		shufps	$0, %%xmm2, %%xmm2\n\
		mulps	%%xmm5, %%xmm2\n\
\n\
		movss	44(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm2\n\
\n\
		addps	%%xmm2, %%xmm1\n\
		movaps	%%xmm1, 32(%%edx)\n\
\n\
		movss	48(%%ecx), %%xmm1\n\
		shufps	$0, %%xmm1, %%xmm1\n\
		mulps	%%xmm4, %%xmm1\n\
\n\
		movss	56(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm6, %%xmm0\n\
		addps	%%xmm0, %%xmm1\n\
\n\
		movss	52(%%ecx), %%xmm2\n\
		shufps	$0, %%xmm2, %%xmm2\n\
		mulps	%%xmm5, %%xmm2\n\
\n\
		movss	60(%%ecx), %%xmm0\n\
		shufps	$0, %%xmm0, %%xmm0\n\
		mulps	%%xmm7, %%xmm0\n\
		addps	%%xmm0, %%xmm2\n\
\n\
		addps	%%xmm2, %%xmm1\n\
		movaps	%%xmm1, 48(%%edx)\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%eax", "%ecx", "%edx",
		"%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7",
		"cc", "memory"
	);
}

#endif



//=============================================================================
// AVX
//=============================================================================


//-----------------------------------------------------------------------------
// AVX A 256bit
//-----------------------------------------------------------------------------
//  0  0  0  0 =  0 = 0
// 40 10  4  1 = 55 = 85
// 80 20  8  2 = aa = 170
// c0 30  c  3 = ff = 255

#if FL_CPU_X86


#if 1

#if 0


inline void Mul_AVX_A256( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%edx\n\
		mov	%[p1], %%eax\n\
		mov	%[p2], %%ecx\n\
\n\
		vmovups	(%%eax), %%ymm0\n\
		vmovups	32(%%eax), %%ymm1\n\
\n\
\n\
		vmovups	(%%ecx), %%ymm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm6, %%ymm6\n\
\n\
\n\
		vextractf128	$1, %%ymm2, %%xmm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm7\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm7, %%ymm7\n\
\n\
\n\
		vperm2f128	$0x20, %%ymm7, %%ymm6, %%ymm3\n\
		vperm2f128	$0x31, %%ymm7, %%ymm6, %%ymm4\n\
		vaddps		%%ymm3, %%ymm4, %%ymm4\n\
		vmovups		%%ymm4, (%%edx)\n\
\n\
\n\
		vmovups	32(%%ecx), %%ymm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm6, %%ymm6\n\
\n\
\n\
		vextractf128	$1, %%ymm2, %%xmm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm7\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm7, %%ymm7\n\
\n\
\n\
		vperm2f128	$0x20, %%ymm7, %%ymm6, %%ymm3\n\
		vperm2f128	$0x31, %%ymm7, %%ymm6, %%ymm4\n\
		vaddps		%%ymm3, %%ymm4, %%ymm4\n\
		vmovups		%%ymm4, 32(%%edx)\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%eax", "%ecx", "%edx",
		"%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7",
		"cc", "memory"
	);
}

#else //--------


inline void Mul_AVX_A256( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%edx\n\
		mov	%[p1], %%eax\n\
		mov	%[p2], %%ecx\n\
\n\
		vmovups	(%%eax), %%xmm0\n\
		vmovups	16(%%eax), %%xmm1\n\
		vmovups	32(%%eax), %%xmm2\n\
		vmovups	48(%%eax), %%xmm3\n\
		vperm2f128	$0x00, %%ymm0, %%ymm0, %%ymm0\n\
		vperm2f128	$0x00, %%ymm1, %%ymm1, %%ymm1\n\
		vperm2f128	$0x00, %%ymm2, %%ymm2, %%ymm2\n\
		vperm2f128	$0x00, %%ymm3, %%ymm3, %%ymm3\n\
\n\
		vmovups	(%%ecx), %%ymm4\n\
		vpermilps	$0x00, %%ymm4, %%ymm5\n\
		vmulps		%%ymm0, %%ymm5, %%ymm6\n\
\n\
		vpermilps	$0x55, %%ymm4, %%ymm5\n\
		vmulps		%%ymm1, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%ymm4, %%ymm5\n\
		vmulps		%%ymm2, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xff, %%ymm4, %%ymm5\n\
		vmulps		%%ymm3, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vmovups		%%ymm6, (%%edx)\n\
\n\
\n\
		vmovups	32(%%ecx), %%ymm4\n\
		vpermilps	$0x00, %%ymm4, %%ymm5\n\
		vmulps		%%ymm0, %%ymm5, %%ymm6\n\
\n\
		vpermilps	$0x55, %%ymm4, %%ymm5\n\
		vmulps		%%ymm1, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%ymm4, %%ymm5\n\
		vmulps		%%ymm2, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xff, %%ymm4, %%ymm5\n\
		vmulps		%%ymm3, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vmovups		%%ymm6, 32(%%edx)\n\
\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%eax", "%ecx", "%edx",
		"%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7",
		"cc", "memory"
	);
}

#endif //----------



#else



#endif







#else // FL_CPU_X64


#if 0

inline void Mul_AVX_A256( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%rdx\n\
		mov	%[p1], %%rax\n\
		mov	%[p2], %%rcx\n\
\n\
		vmovups	(%%rax), %%ymm0\n\
		vmovups	32(%%rax), %%ymm1\n\
\n\
\n\
		vmovups	(%%rcx), %%ymm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm6, %%ymm6\n\
\n\
\n\
		vextractf128	$1, %%ymm2, %%xmm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm7\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm7, %%ymm7\n\
\n\
\n\
		vperm2f128	$0x20, %%ymm7, %%ymm6, %%ymm3\n\
		vperm2f128	$0x31, %%ymm7, %%ymm6, %%ymm4\n\
		vaddps		%%ymm3, %%ymm4, %%ymm4\n\
		vmovups		%%ymm4, (%%rdx)\n\
\n\
\n\
		vmovups	32(%%rcx), %%ymm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm6, %%ymm6\n\
\n\
\n\
		vextractf128	$1, %%ymm2, %%xmm2\n\
		vpermilps	$0x00, %%xmm2, %%xmm3\n\
		vpermilps	$0x55, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm0, %%ymm7\n\
\n\
		vpermilps	$0xaa, %%xmm2, %%xmm3\n\
		vpermilps	$0xff, %%xmm2, %%xmm4\n\
		vperm2f128	$0x20, %%ymm4, %%ymm3, %%ymm3\n\
		vmulps		%%ymm3, %%ymm1, %%ymm3\n\
		vaddps		%%ymm3, %%ymm7, %%ymm7\n\
\n\
\n\
		vperm2f128	$0x20, %%ymm7, %%ymm6, %%ymm3\n\
		vperm2f128	$0x31, %%ymm7, %%ymm6, %%ymm4\n\
		vaddps		%%ymm3, %%ymm4, %%ymm4\n\
		vmovups		%%ymm4, 32(%%rdx)\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%rax", "%rcx", "%rdx",
		"%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7",
		"cc", "memory"
	);
}




#else

inline void Mul_AVX_A256( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		mov	%[p3], %%rdx\n\
		mov	%[p1], %%rax\n\
		mov	%[p2], %%rcx\n\
\n\
		vmovups	(%%rax), %%xmm0\n\
		vmovups	16(%%rax), %%xmm1\n\
		vmovups	32(%%rax), %%xmm2\n\
		vmovups	48(%%rax), %%xmm3\n\
		vperm2f128	$0x00, %%ymm0, %%ymm0, %%ymm0\n\
		vperm2f128	$0x00, %%ymm1, %%ymm1, %%ymm1\n\
		vperm2f128	$0x00, %%ymm2, %%ymm2, %%ymm2\n\
		vperm2f128	$0x00, %%ymm3, %%ymm3, %%ymm3\n\
\n\
		vmovups	(%%rcx), %%ymm4\n\
		vpermilps	$0x00, %%ymm4, %%ymm5\n\
		vmulps		%%ymm0, %%ymm5, %%ymm6\n\
\n\
		vpermilps	$0x55, %%ymm4, %%ymm5\n\
		vmulps		%%ymm1, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%ymm4, %%ymm5\n\
		vmulps		%%ymm2, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xff, %%ymm4, %%ymm5\n\
		vmulps		%%ymm3, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vmovups		%%ymm6, (%%rdx)\n\
\n\
\n\
		vmovups	32(%%rcx), %%ymm4\n\
		vpermilps	$0x00, %%ymm4, %%ymm5\n\
		vmulps		%%ymm0, %%ymm5, %%ymm6\n\
\n\
		vpermilps	$0x55, %%ymm4, %%ymm5\n\
		vmulps		%%ymm1, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xaa, %%ymm4, %%ymm5\n\
		vmulps		%%ymm2, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vpermilps	$0xff, %%ymm4, %%ymm5\n\
		vmulps		%%ymm3, %%ymm5, %%ymm5\n\
		vaddps		%%ymm5, %%ymm6, %%ymm6\n\
\n\
		vmovups		%%ymm6, 32(%%rdx)\n\
\n\
	"
	:
	: [p1]"m"( p1 ), [p2]"m"( p2 ), [p3]"m"( p3 )
	:	"%rax", "%rcx", "%rdx",
		"%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7",
		"cc", "memory"
	);
}

#endif


#endif //-------------- X64





//=============================================================================
//=============================================================================








//-----------------------------------------------------------------------------
// TEMPLATE
//-----------------------------------------------------------------------------

#if 1

#define	TEMPLATE_MUL( FUNC_NAME, DUMP_NAME )	\
static uint64_t Matrix_##FUNC_NAME( math::Matrix4& dest, const math::Matrix4& src1, const math::Matrix4& src2, unsigned int Loop )		\
{		\
	math::Matrix4 dest1;		\
	math::Matrix4 dest2;		\
		\
    dest2= src2;		\
		\
	TimerClass	timer;		\
	timer.Begin();		\
	for( unsigned int i= 0 ; i< Loop ; i++ ){		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
		FUNC_NAME( &dest2, &src1, &dest1 );		\
	}		\
	timer.End();		\
	timer.Dump( DUMP_NAME );		\
		\
    dest= dest2;		\
	return	timer.Result();		\
}


#else


#define	TEMPLATE_MUL( FUNC_NAME, DUMP_NAME )	\
static uint64_t Matrix_##FUNC_NAME( math::Matrix4& dest, const math::Matrix4& src1, const math::Matrix4& src2, unsigned int Loop )		\
{		\
	math::Matrix4 dest1;		\
	math::Matrix4 dest2;		\
		\
    dest2= src2;		\
		\
	TimerClass	timer;		\
	timer.Begin();		\
	for( unsigned int i= 0 ; i< Loop ; i++ ){		\
		FUNC_NAME( &dest1, &dest2, &src2 );		\
dest2= dest1;\
	}		\
	timer.End();		\
	timer.Dump( DUMP_NAME );		\
		\
    dest= dest2;		\
	return	timer.Result();		\
}

#endif











//-----------------------------------------------------------------------------
// CPU flatlib3 default
//-----------------------------------------------------------------------------

static uint64_t Matrix_Mul_FLATLIB_A( math::Matrix4& dest, const math::Matrix4& src1, const math::Matrix4& src2, unsigned int Loop )
{
	math::Matrix4 dest1;
	math::Matrix4 dest2;

    dest2= src2;

	TimerClass	timer;
	timer.Begin();
	for( int i= 0 ; i< Loop ; i++ ){
		dest1.MulCopy( src2, dest2 );
#if 1
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
#else
dest2= dest1;
#endif
	}
	timer.End();
	timer.Dump( "mat4x4 c++(flatlib)" );

    dest= dest2;
	return	timer.Result();
}



//-----------------------------------------------------------------------------
// CPU FPU
//-----------------------------------------------------------------------------

static uint64_t Matrix_Mul_LIB_A( math::Matrix4& dest, const math::Matrix4& src1, const math::Matrix4& src2, unsigned int Loop )
{
	math::Matrix4 dest1;
	math::Matrix4 dest2;

    dest2= src2;

	TimerClass	timer;
	timer.Begin();
	for( int i= 0 ; i< Loop ; i++ ){
		dest1.MulCopy_4_4_FPU( src2, dest2 );
#if 1
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
		dest1.MulCopy_4_4_FPU( src2, dest2 );
		dest2.MulCopy_4_4_FPU( dest1, src1 );
#else
dest2= dest1;
#endif
	}
	timer.End();
	timer.Dump( "mat4x4 c++(fpu)" );

    dest= dest2;
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// Compare
//-----------------------------------------------------------------------------

#define		COMPARE_PREC		1e-6f

static bool Matrix_Equal( const math::Matrix4& src1, const math::Matrix4& src2 )
{
    const float*    fp1= &src1._11;
    const float*    fp2= &src2._11;
    for( int i= 0 ; i< 16 ; i++, fp1++, fp2++ ){
        if( !math::Equal( *fp1, *fp2, COMPARE_PREC ) ){
			FL_ERROR("----\n");
			src1.Dump( "src1" );
			FL_ERROR("----\n");
			src2.Dump( "src2" );
			FL_ERROR("----\n");
            return  false;
        }
    }
    return  true;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


TEMPLATE_MUL( Mul_SSE_A128, "SSE A128" );

TEMPLATE_MUL( Mul_AVX_A256, "AVX B256" );






//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct alignas(32) Result {
	math::Matrix4	dest;
};

#define	CHECK_( ret )	if( !Matrix_Equal( ret.dest, result_LIB.dest ) ){ err_count++; }



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

MatrixTest::MatrixTest()
{
	LoopCount= DEFAULT_LOOP;
	ClearResult();
}



void MatrixTest::Run()
{
	ClearResult();
	unsigned int	Loop= LoopCount;

FL_LOG( "Matrix loop=%d\n", Loop );

	math::Matrix4 src1;
	math::Matrix4 src2;

	src1.SetIdentity();
	src2.SetIdentity();


	src1.Translation( 2.0f, -2.8f, 0.9f );
	src1.RotationX( math::DEGtoRAD(33.0f) );
	src1.RotationY( math::DEGtoRAD(33.0f) );
	src1.RotationZ( math::DEGtoRAD(33.0f) );

	src2.Translation( 1.0f, 1.5f, -3.0f );
	src2.RotationX( math::DEGtoRAD(13.0f) );
	src2.RotationY( math::DEGtoRAD(23.0f) );
	src2.RotationZ( math::DEGtoRAD(39.0f) );


	unsigned int	err_count= 0;


	Result	result_LIB;
	SetResult( RESULT_LIB, Matrix_Mul_LIB_A( result_LIB.dest, src1, src2, Loop ) );
	Progress++;


	Result	result_FLATLIB;
	SetResult( RESULT_FLATLIB, Matrix_Mul_FLATLIB_A( result_FLATLIB.dest, src1, src2, Loop ) );
	CHECK_( result_FLATLIB );
	Progress++;



	Result	result_A128;
	SetResult( RESULT_SSE_A128, Matrix_Mul_SSE_A128( result_A128.dest, src1, src2, Loop ) );
	CHECK_( result_A128 );


	Progress++;



	Result	result_A256;
	if( Info.HasInstructionSet( CPUFeature::IA_AVX ) ){

		SetResult( RESULT_AVX_A256, Matrix_Mul_AVX_A256( result_A256.dest, src1, src2, Loop ) );
		CHECK_( result_A256 );

		Progress++;

	}

#if 0
	FL_ERROR("----\n");
	result_LIB.dest.Dump( "FPU" );
	FL_ERROR("----\n");
	result_FLATLIB.dest.Dump( "FLATLIB" );
	FL_ERROR("----\n");
	result_A128.dest.Dump( "SSE 128" );
	FL_ERROR("----\n");
	result_A256.dest.Dump( "AVX 128" );
	FL_ERROR("----\n");
#endif

	DoneFlag= true;

	//flASSERT( err_count == 0 );
}



static const char*	Instruction_Title[]= {

	"C++ code",
	"C++ Intrinsic SSE 128bit",
	"SSE mul/addps  128bit A",
	"AVX vmul/addps 256bit A",
};

const char*	MatrixTest::GetInstructionName( unsigned int result_index ) const
{
	flASSERT( result_index < GetResultCount() );
	flASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
	return	Instruction_Title[result_index];
}

const char*	MatrixTest::GetTestName() const
{
	if( !IsMultithread() ){
		return	"Matrix 4x4";
	}else{
		return	"Matrix 4x4 multi-thread";
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
#endif // FL_CPU_X86 || FL_CPU_X64
//-----------------------------------------------------------------------------




