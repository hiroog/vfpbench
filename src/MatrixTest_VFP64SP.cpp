// 2014/01/15 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	<minilib/CoreLib.h>
#include	<minilib/Matrix4.h>
#include	<minilib/SystemInfo.h>
#include	"TimerClass.h"
#include	"MatrixTest_VFP64SP.h"


//=============================================================================
#if flCPU_ARM64
//=============================================================================

using namespace flatlib;

namespace VFP64SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if flOS_ANDROID
# define	USE_ANDROID_GCC		1
#else
# define	USE_ANDROID_GCC		0
#endif





#if USE_ANDROID_GCC



inline void Mul_NEON_AQ( math::Matrix4* p3, const math::Matrix4* p1, const math::Matrix4* p2 )
{
	// v0, v1, v2
	__asm__ __volatile__ ( "\
		ldp	q0, q1, [%0]		\n\
		ldp	q2, q3, [%0,#32]	\n\
		ldp	q4, q5, [%1]		\n\
		ldp	q6, q7, [%1,#32]	\n\
\n\
		fmul	v8.4s, v0.4s, v4.s[0]	\n\
		fmla	v8.4s, v1.4s, v4.s[1]	\n\
		fmla	v8.4s, v2.4s, v4.s[2]	\n\
		fmla	v8.4s, v3.4s, v4.s[3]	\n\
		str		q8, [%2]		\n\
\n\
		fmul	v8.4s, v0.4s, v5.s[0]	\n\
		fmla	v8.4s, v1.4s, v5.s[1]	\n\
		fmla	v8.4s, v2.4s, v5.s[2]	\n\
		fmla	v8.4s, v3.4s, v5.s[3]	\n\
		str		q8, [%2,#16]	\n\
\n\
		fmul	v8.4s, v0.4s, v6.s[0]	\n\
		fmla	v8.4s, v1.4s, v6.s[1]	\n\
		fmla	v8.4s, v2.4s, v6.s[2]	\n\
		fmla	v8.4s, v3.4s, v6.s[3]	\n\
		str		q8, [%2,#32]	\n\
\n\
		fmul	v8.4s, v0.4s, v7.s[0]	\n\
		fmla	v8.4s, v1.4s, v7.s[1]	\n\
		fmla	v8.4s, v2.4s, v7.s[2]	\n\
		fmla	v8.4s, v3.4s, v7.s[3]	\n\
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
		fmul	v8.4s,  v0.4s, v4.s[0]	\n\
		fmul	v9.4s,  v0.4s, v5.s[0]	\n\
		fmul	v10.4s, v0.4s, v6.s[0]	\n\
		fmul	v11.4s, v0.4s, v7.s[0]	\n\
\n\
		ldp	q2, q3, [%0,#32]	\n\
\n\
		fmla	v8.4s,  v1.4s, v4.s[1]	\n\
		fmla	v9.4s,  v1.4s, v5.s[1]	\n\
		fmla	v10.4s, v1.4s, v6.s[1]	\n\
		fmla	v11.4s, v1.4s, v7.s[1]	\n\
\n\
		fmla	v8.4s,  v2.4s, v4.s[2]	\n\
		fmla	v9.4s,  v2.4s, v5.s[2]	\n\
		fmla	v10.4s, v2.4s, v6.s[2]	\n\
		fmla	v11.4s, v2.4s, v7.s[2]	\n\
\n\
		fmla	v8.4s,  v3.4s, v4.s[3]	\n\
		fmla	v9.4s,  v3.4s, v5.s[3]	\n\
		fmla	v10.4s, v3.4s, v6.s[3]	\n\
		fmla	v11.4s, v3.4s, v7.s[3]	\n\
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





#else

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


#endif





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

static bool Matrix_Equal( const math::Matrix4& src1, const math::Matrix4& src2 )
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
//-----------------------------------------------------------------------------

TEMPLATE_MUL( Mul_NEON_AQ, "NEON AQ" );
TEMPLATE_MUL( Mul_NEON_BQ, "NEON BQ" );

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------








//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct Result {
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


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

	Result	result_AQ;
	SetResult( RESULT_NEON_AQ, Matrix_Mul_NEON_AQ( result_AQ.dest, src1, src2, Loop ) );
	CHECK_( result_AQ );

	Progress++;

	Result	result_BQ;
	SetResult( RESULT_NEON_BQ, Matrix_Mul_NEON_BQ( result_BQ.dest, src1, src2, Loop ) );
	CHECK_( result_BQ );

	Progress++;

	Progress+= 5;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


	DoneFlag= true;

//	flASSERT( err_count == 0 );
}





static const char*	Instruction_Title[]= {
	"C++ code",
	"NEON fmla.4s 128bit A",
	"NEON fmla.4s 128bit B",
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


//=============================================================================
#endif
//=============================================================================


