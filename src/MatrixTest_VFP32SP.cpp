// 2014/01/15 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/Matrix4.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"MatrixTest_VFP32SP.h"


//-----------------------------------------------------------------------------
#if FL_CPU_ARM7 || FL_CPU_ARM6
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


//-----------------------------------------------------------------------------
#if FL_CPU_NEON
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// NEON A 128bit
//-----------------------------------------------------------------------------


inline void Mul_NEON_AQ( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}		\n\
		vldmia	%1, {d8-d15}	\n\
\n\
		vmul.f32	q8,q0,d8[0]		\n\
		vmla.f32	q8,q1,d8[1]		\n\
		vmla.f32	q8,q2,d9[0]		\n\
		vmla.f32	q8,q3,d9[1]		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d10[0]	\n\
		vmla.f32	q8,q1,d10[1]	\n\
		vmla.f32	q8,q2,d11[0]	\n\
		vmla.f32	q8,q3,d11[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d12[0]	\n\
		vmla.f32	q8,q1,d12[1]	\n\
		vmla.f32	q8,q2,d13[0]	\n\
		vmla.f32	q8,q3,d13[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d14[0]	\n\
		vmla.f32	q8,q1,d14[1]	\n\
		vmla.f32	q8,q2,d15[0]	\n\
		vmla.f32	q8,q3,d15[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "cc", "memory"
	);
}

/*
	FOP
		
		vmul.f32	q8,q0,d8[0]			4
		vmla.f32	q8,q1,d8[1]			8
		vmla.f32	q8,q2,d9[0]			8
		vmla.f32	q8,q3,d9[1]			8	= 28

		vmul.f32	q8,q0,d10[0]		4
		vmla.f32	q8,q1,d10[1]		8
		vmla.f32	q8,q2,d11[0]		8
		vmla.f32	q8,q3,d11[1]		8

		vmul.f32	q8,q0,d12[0]		4
		vmla.f32	q8,q1,d12[1]		8
		vmla.f32	q8,q2,d13[0]		8
		vmla.f32	q8,q3,d13[1]		8

		vmul.f32	q8,q0,d14[0]		4
		vmla.f32	q8,q1,d14[1]		8
		vmla.f32	q8,q2,d15[0]		8
		vmla.f32	q8,q3,d15[1]		8
								TOTAL FOP	= 28x4 = 112

*/


//-----------------------------------------------------------------------------
// NEON A 64bit
//-----------------------------------------------------------------------------

inline void Mul_NEON_AD( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}		\n\
		vldmia	%1, {d8-d15}	\n\
\n\
		vmul.f32	d16,d0,d8[0]	\n\
		vmul.f32	d17,d1,d8[0]	\n\
		vmla.f32	d16,d2,d8[1]	\n\
		vmla.f32	d17,d3,d8[1]	\n\
		vmla.f32	d16,d4,d9[0]	\n\
		vmla.f32	d17,d5,d9[0]	\n\
		vmla.f32	d16,d6,d9[1]	\n\
		vmla.f32	d17,d7,d9[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	d16,d0,d10[0]	\n\
		vmul.f32	d17,d1,d10[0]	\n\
		vmla.f32	d16,d2,d10[1]	\n\
		vmla.f32	d17,d3,d10[1]	\n\
		vmla.f32	d16,d4,d11[0]	\n\
		vmla.f32	d17,d5,d11[0]	\n\
		vmla.f32	d16,d6,d11[1]	\n\
		vmla.f32	d17,d7,d11[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	d16,d0,d12[0]	\n\
		vmul.f32	d17,d1,d12[0]	\n\
		vmla.f32	d16,d2,d12[1]	\n\
		vmla.f32	d17,d3,d12[1]	\n\
		vmla.f32	d16,d4,d13[0]	\n\
		vmla.f32	d17,d5,d13[0]	\n\
		vmla.f32	d16,d6,d13[1]	\n\
		vmla.f32	d17,d7,d13[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	d16,d0,d14[0]	\n\
		vmul.f32	d17,d1,d14[0]	\n\
		vmla.f32	d16,d2,d14[1]	\n\
		vmla.f32	d17,d3,d14[1]	\n\
		vmla.f32	d16,d4,d15[0]	\n\
		vmla.f32	d17,d5,d15[0]	\n\
		vmla.f32	d16,d6,d15[1]	\n\
		vmla.f32	d17,d7,d15[1]	\n\
		vstmia	%2!, {d16,d17}		\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "cc", "memory"
	);
}


//-----------------------------------------------------------------------------
// NEON B 128bit
//-----------------------------------------------------------------------------

inline void Mul_NEON_BQ( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
/*
	q0-q3
	q4-q7
*/
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}			\n\
		vldmia	%1, {d8-d15}		\n\
\n\
		vmul.f32	q8,q0,d8[0]			\n\
		vmul.f32	q9,q0,d10[0]		\n\
		vmul.f32	q10,q0,d12[0]		\n\
		vmul.f32	q11,q0,d14[0]		\n\
\n\
		vmla.f32	q8,q1,d8[1]			\n\
		vmla.f32	q9,q1,d10[1]		\n\
		vmla.f32	q10,q1,d12[1]		\n\
		vmla.f32	q11,q1,d14[1]		\n\
\n\
		vmla.f32	q8,q2,d9[0]			\n\
		vmla.f32	q9,q2,d11[0]		\n\
		vmla.f32	q10,q2,d13[0]		\n\
		vmla.f32	q11,q2,d15[0]		\n\
\n\
		vmla.f32	q8,q3,d9[1]			\n\
		vmla.f32	q9,q3,d11[1]		\n\
		vmla.f32	q10,q3,d13[1]		\n\
		vmla.f32	q11,q3,d15[1]		\n\
		vstmia	%2, {d16-d23}			\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9","q10","q11","cc", "memory"
	);
}



//-----------------------------------------------------------------------------
// NEON B 64bit
//-----------------------------------------------------------------------------

inline void Mul_NEON_BD( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}			\n\
		vldmia	%1, {d8-d15}		\n\
\n\
		vmul.f32	d16,d0,d8[0]		\n\
		vmul.f32	d17,d1,d8[0]		\n\
		vmul.f32	d18,d0,d10[0]		\n\
		vmul.f32	d19,d1,d10[0]		\n\
		vmul.f32	d20,d0,d12[0]		\n\
		vmul.f32	d21,d1,d12[0]		\n\
		vmul.f32	d22,d0,d14[0]		\n\
		vmul.f32	d23,d1,d14[0]		\n\
\n\
		vmla.f32	d16,d2,d8[1]		\n\
		vmla.f32	d17,d3,d8[1]		\n\
		vmla.f32	d18,d2,d10[1]		\n\
		vmla.f32	d19,d3,d10[1]		\n\
		vmla.f32	d20,d2,d12[1]		\n\
		vmla.f32	d21,d3,d12[1]		\n\
		vmla.f32	d22,d2,d14[1]		\n\
		vmla.f32	d23,d3,d14[1]		\n\
\n\
		vmla.f32	d16,d4,d9[0]		\n\
		vmla.f32	d17,d5,d9[0]		\n\
		vmla.f32	d18,d4,d11[0]		\n\
		vmla.f32	d19,d5,d11[0]		\n\
		vmla.f32	d20,d4,d13[0]		\n\
		vmla.f32	d21,d5,d13[0]		\n\
		vmla.f32	d22,d4,d15[0]		\n\
		vmla.f32	d23,d5,d15[0]		\n\
\n\
		vmla.f32	d16,d6,d9[1]		\n\
		vmla.f32	d17,d7,d9[1]		\n\
		vmla.f32	d18,d6,d11[1]		\n\
		vmla.f32	d19,d7,d11[1]		\n\
		vmla.f32	d20,d6,d13[1]		\n\
		vmla.f32	d21,d7,d13[1]		\n\
		vmla.f32	d22,d6,d15[1]		\n\
		vmla.f32	d23,d7,d15[1]		\n\
		vstmia	%2, {d16-d23}			\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9","q10","q11","cc", "memory"
	);
}



//-----------------------------------------------------------------------------
#if FL_CPU_VFPV4
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// FMA A 128bit
//-----------------------------------------------------------------------------


inline void Mul_NEON_AQ_FMA( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}		\n\
		vldmia	%1, {d8-d15}	\n\
\n\
		vmul.f32	q8,q0,d8[0]		\n\
		vdup.32		q9, d8[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q10, d9[0]		\n\
		vfma.f32	q8,q2,q10		\n\
		vdup.32		q11, d9[1]		\n\
		vfma.f32	q8,q3,q11		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d10[0]	\n\
		vdup.32		q12, d10[1]		\n\
		vfma.f32	q8,q1,q12		\n\
		vdup.32		q13, d11[0]		\n\
		vfma.f32	q8,q2,q13		\n\
		vdup.32		q14, d11[1]		\n\
		vfma.f32	q8,q3,q14		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d12[0]	\n\
		vdup.32		q9, d12[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q10, d13[0]		\n\
		vfma.f32	q8,q2,q10		\n\
		vdup.32		q11, d13[1]		\n\
		vfma.f32	q8,q3,q11		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d14[0]	\n\
		vdup.32		q12, d14[1]		\n\
		vfma.f32	q8,q1,q12		\n\
		vdup.32		q13, d15[0]		\n\
		vfma.f32	q8,q2,q13		\n\
		vdup.32		q14, d15[1]		\n\
		vfma.f32	q8,q3,q14		\n\
		vstmia	%2!, {d16,d17}		\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
		"q8", "q9", "q10", "q11", "q12", "q13", "q14",
		"cc", "memory"
	);
}


//-----------------------------------------------------------------------------
// FMA A 128bit shared register (Renaming test)
//-----------------------------------------------------------------------------


inline void Mul_NEON_AQ_FMA_RN( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}		\n\
		vldmia	%1, {d8-d15}	\n\
\n\
		vmul.f32	q8,q0,d8[0]		\n\
		vdup.32		q9, d8[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q9, d9[0]		\n\
		vfma.f32	q8,q2,q9		\n\
		vdup.32		q9, d9[1]		\n\
		vfma.f32	q8,q3,q9		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d10[0]	\n\
		vdup.32		q9, d10[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q9, d11[0]		\n\
		vfma.f32	q8,q2,q9		\n\
		vdup.32		q9, d11[1]		\n\
		vfma.f32	q8,q3,q9		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d12[0]	\n\
		vdup.32		q9, d12[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q9, d13[0]		\n\
		vfma.f32	q8,q2,q9		\n\
		vdup.32		q9, d13[1]		\n\
		vfma.f32	q8,q3,q9		\n\
		vstmia	%2!, {d16,d17}		\n\
\n\
		vmul.f32	q8,q0,d14[0]	\n\
		vdup.32		q9, d14[1]		\n\
		vfma.f32	q8,q1,q9		\n\
		vdup.32		q9, d15[0]		\n\
		vfma.f32	q8,q2,q9		\n\
		vdup.32		q9, d15[1]		\n\
		vfma.f32	q8,q3,q9		\n\
		vstmia	%2!, {d16,d17}		\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "cc", "memory"
	);
}




//-----------------------------------------------------------------------------
// FMA B 128bit
//-----------------------------------------------------------------------------

inline void Mul_NEON_BQ_FMA( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
/*
	q0-q3
	q4-q7
*/
	__asm__ __volatile__ ( "\
		vldmia	%0, {d0-d7}			\n\
		vldmia	%1, {d8-d15}		\n\
\n\
		vmul.f32	q8,q0,d8[0]			\n\
		vmul.f32	q9,q0,d10[0]		\n\
		vmul.f32	q10,q0,d12[0]		\n\
		vmul.f32	q11,q0,d14[0]		\n\
\n\
		vdup.32		q12, d8[1]		\n\
		vdup.32		q13, d10[1]		\n\
		vdup.32		q14, d12[1]		\n\
		vdup.32		q15, d14[1]		\n\
\n\
		vfma.f32	q8,q1,q12		\n\
		vfma.f32	q9,q1,q13		\n\
		vfma.f32	q10,q1,q14		\n\
		vfma.f32	q11,q1,q15		\n\
\n\
		vdup.32		q12, d9[0]		\n\
		vdup.32		q13, d11[0]		\n\
		vdup.32		q14, d13[0]		\n\
		vdup.32		q15, d15[0]		\n\
\n\
		vfma.f32	q8,q2,q12		\n\
		vfma.f32	q9,q2,q13		\n\
		vfma.f32	q10,q2,q14		\n\
		vfma.f32	q11,q2,q15		\n\
\n\
		vdup.32		q12, d9[1]		\n\
		vdup.32		q13, d11[1]		\n\
		vdup.32		q14, d13[1]		\n\
		vdup.32		q15, d15[1]		\n\
\n\
		vfma.f32	q8,q3,q12		\n\
		vfma.f32	q9,q3,q13		\n\
		vfma.f32	q10,q3,q14		\n\
		vfma.f32	q11,q3,q15		\n\
		vstmia	%2, {d16-d23}		\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9","q10","q11","q12","q13","q14","q15","cc", "memory"
	);
}


//-----------------------------------------------------------------------------
#endif // VFPv4
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
#endif // FL_CPU_NEON
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#endif // FL_CPU_ARM7 || FL_CPU_ARM6
//-----------------------------------------------------------------------------



//=============================================================================
//=============================================================================



//-----------------------------------------------------------------------------
#if 0//FL_CPU_ARM64
//-----------------------------------------------------------------------------



inline void Mul_NEON_AQ( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	// v0, v1, v2
	__asm__ __volatile__ ( "\
		ldp	q0, q1, [%0]		\n\
		ldp	q2, q3, [%0,#32]	\n\
		ldp	q4, q5, [%1]		\n\
		ldp	q6, q7, [%1,#32]	\n\
\n\
		fmul.4s	v8, v0, v4[0]	\n\
		fmla.4s	v8, v1, v4[1]	\n\
		fmla.4s	v8, v2, v4[2]	\n\
		fmla.4s	v8, v3, v4[3]	\n\
		str		q8, [%2]		\n\
\n\
		fmul.4s	v8, v0, v5[0]	\n\
		fmla.4s	v8, v1, v5[1]	\n\
		fmla.4s	v8, v2, v5[2]	\n\
		fmla.4s	v8, v3, v5[3]	\n\
		str		q8, [%2,#16]	\n\
\n\
		fmul.4s	v8, v0, v6[0]	\n\
		fmla.4s	v8, v1, v6[1]	\n\
		fmla.4s	v8, v2, v6[2]	\n\
		fmla.4s	v8, v3, v6[3]	\n\
		str		q8, [%2,#32]	\n\
\n\
		fmul.4s	v8, v0, v7[0]	\n\
		fmla.4s	v8, v1, v7[1]	\n\
		fmla.4s	v8, v2, v7[2]	\n\
		fmla.4s	v8, v3, v7[3]	\n\
		str		q8, [%2,#48]	\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "x0", "x1", "x2", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "cc", "memory"
	);
}








inline void Mul_NEON_BQ( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	// v0, v1, v2
	__asm__ __volatile__ ( "\
		ldp	q0, q1, [%0]		\n\
		ldp	q4, q5, [%1]		\n\
		ldp	q6, q7, [%1,#32]	\n\
\n\
		fmul.4s	v8,  v0, v4[0]	\n\
		fmul.4s	v9,  v0, v5[0]	\n\
		fmul.4s	v10, v0, v6[0]	\n\
		fmul.4s	v11, v0, v7[0]	\n\
\n\
		ldp	q2, q3, [%0,#32]	\n\
\n\
		fmla.4s	v8,  v1, v4[1]	\n\
		fmla.4s	v9,  v1, v5[1]	\n\
		fmla.4s	v10, v1, v6[1]	\n\
		fmla.4s	v11, v1, v7[1]	\n\
\n\
		fmla.4s	v8,  v2, v4[2]	\n\
		fmla.4s	v9,  v2, v5[2]	\n\
		fmla.4s	v10, v2, v6[2]	\n\
		fmla.4s	v11, v2, v7[2]	\n\
\n\
		fmla.4s	v8,  v3, v4[3]	\n\
		fmla.4s	v9,  v3, v5[3]	\n\
		fmla.4s	v10, v3, v6[3]	\n\
		fmla.4s	v11, v3, v7[3]	\n\
\n\
		stp		q8,   q9, [%2]		\n\
		stp		q10, q11, [%2,#32]	\n\
	"
	: "=&r"( p1 ), "=&r"( p2 ), "=&r"( p3 )
	: "0"( p1 ), "1"( p2 ), "2"( p3 )
	: "x0", "x1", "x2",
		"v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11",
		"cc", "memory"
	);
}








//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if FL_CPU_ARM7 || FL_CPU_ARM6
//-----------------------------------------------------------------------------





//=============================================================================
//=============================================================================








//-----------------------------------------------------------------------------
// TEMPLATE
//-----------------------------------------------------------------------------


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
		dest1.MulCopy( src2, dest2 );
		dest2.MulCopy( dest1, src1 );
	}
	timer.End();
	timer.Dump( "mat4x4 c++(vfp)" );

    dest= dest2;
	return	timer.Result();
}


//-----------------------------------------------------------------------------
// Compare
//-----------------------------------------------------------------------------

#define		COMPARE_PREC		1e-6f

bool Matrix_Equal_VFP( const math::Matrix4& src1, const math::Matrix4& src2 )
{
    const float*    fp1= &src1._11;
    const float*    fp2= &src2._11;
    for( int i= 0 ; i< 16 ; i++, fp1++, fp2++ ){
        if( !math::Equal( *fp1, *fp2, COMPARE_PREC ) ){
            return  false;
        }
    }
    return  true;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if FL_CPU_NEON
//-----------------------------------------------------------------------------

TEMPLATE_MUL( Mul_NEON_AQ, "NEON AQ" );
TEMPLATE_MUL( Mul_NEON_BQ, "NEON BQ" );

#if FL_CPU_ARM7
TEMPLATE_MUL( Mul_NEON_AD, "NEON AD" );
TEMPLATE_MUL( Mul_NEON_BD, "NEON BD" );

#if FL_CPU_VFPV4
TEMPLATE_MUL( Mul_NEON_AQ_FMA, "FMA AQ" );
TEMPLATE_MUL( Mul_NEON_BQ_FMA, "FMA BQ" );
TEMPLATE_MUL( Mul_NEON_AQ_FMA_RN, "FMA AQ RN" );
#endif
#endif

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------








//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct Result {
	math::Matrix4	dest;
};

#define	CHECK_( ret )	if( !Matrix_Equal_VFP( ret.dest, result_LIB.dest ) ){ err_count++; }



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


	Progress.Increment();


//-----------------------------------------------------------------------------
#if FL_CPU_ARM7
//-----------------------------------------------------------------------------

#if FL_CPU_NEON

	if( Info.HasInstructionSet( CPUFeature::ARM_NEON ) ){

		Result	result_AQ;
		SetResult( RESULT_NEON_AQ, Matrix_Mul_NEON_AQ( result_AQ.dest, src1, src2, Loop ) );
		CHECK_( result_AQ );

		Progress.Increment();

		Result	result_BQ;
		SetResult( RESULT_NEON_BQ, Matrix_Mul_NEON_BQ( result_BQ.dest, src1, src2, Loop ) );
		CHECK_( result_BQ );

		Progress.Increment();

		Result	result_AD;
		SetResult( RESULT_NEON_AD, Matrix_Mul_NEON_AD( result_AD.dest, src1, src2, Loop ) );
		CHECK_( result_AD );

		Progress.Increment();

		Result	result_BD;
		SetResult( RESULT_NEON_BD, Matrix_Mul_NEON_BD( result_BD.dest, src1, src2, Loop ) );
		CHECK_( result_BD );

		Progress.Increment();

# if FL_CPU_VFPV4

		Result	result_AQ_FMA;
		SetResult( RESULT_NEON_AQ_FMA, Matrix_Mul_NEON_AQ_FMA( result_AQ_FMA.dest,   src1, src2, Loop ) );
		CHECK_( result_AQ_FMA );

		Progress.Increment();

		Result	result_BQ_FMA;
		SetResult( RESULT_NEON_BQ_FMA, Matrix_Mul_NEON_BQ_FMA( result_BQ_FMA.dest,   src1, src2, Loop ) );
		CHECK_( result_BQ_FMA );

		Progress.Increment();

		Result	result_BQ_FMA_RN;
		SetResult( RESULT_NEON_AQ_FMA_RN, Matrix_Mul_NEON_AQ_FMA_RN( result_BQ_FMA_RN.dest, src1, src2, Loop ) );
		CHECK_( result_BQ_FMA_RN );

		Progress.Increment();

#  if FL_DEBUG
		result_LIB.dest.Dump( "LIB" );
		result_AQ.dest.Dump( "AQ" );
		result_AQ_FMA.dest.Dump( "FMA" );
#  endif

# endif

	}else{
		Progress+= 7;
	}
#endif

//-----------------------------------------------------------------------------
#elif FL_CPU_ARM64
//-----------------------------------------------------------------------------

		Result	result_AQ;
		SetResult( RESULT_NEON_AQ, Matrix_Mul_NEON_AQ( result_AQ.dest, src1, src2, Loop ) );
		CHECK_( result_AQ );

		Progress.Increment();

		Result	result_BQ;
		SetResult( RESULT_NEON_BQ, Matrix_Mul_NEON_BQ( result_BQ.dest, src1, src2, Loop ) );
		CHECK_( result_BQ );

		Progress.Increment();

		Progress+= 5;

//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------

	Progress+= 7;
	err_count= 0;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


	DoneFlag= true;

//	FL_ASSERT( err_count == 0 );
}






static const char*	Instruction_Title[]= {
	"C++ code",
	"NEON vmla 128bit A",
	"NEON vmla  64bit A",
	"NEON vfma 128bit A",
	"NEON vmla 128bit B",
	"NEON vmla  64bit B",
	"NEON vfma 128bit B",
	"NEON vfma 128bit C",
};


const char*	MatrixTest::GetInstructionName( unsigned int result_index ) const
{
	FL_ASSERT( result_index < GetResultCount() );
	FL_ASSERT( sizeof(Instruction_Title)/sizeof(const char*) == GetResultCount() );
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
#endif
//-----------------------------------------------------------------------------



