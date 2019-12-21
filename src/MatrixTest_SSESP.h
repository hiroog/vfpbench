// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	MATRIX_TEST_SSESP_H_
#define	MATRIX_TEST_SSESP_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace SSESP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class MatrixTest : public TestBase {
public:
	enum {
		//DEFAULT_LOOP=	2000000,
		DEFAULT_LOOP=	BASE_DEFAULT_LOOP_MATRIX,
		FLOAT_OP_ONE=	112	* 16,	// 1792
		//FLOAT_OP	=	FLOAT_OP_ONE * DEFAULT_LOOP,	// 1792 * LOOP
			// 35 8400 0000
	};
	enum {
		RESULT_LIB,
		RESULT_FLATLIB,	// intrinsic

		RESULT_SSE_A128,
		RESULT_AVX_A256,

		RESULT_MAX,
	};
public:

	MatrixTest();

	void	Run() override;
	unsigned int	GetLoopOp( unsigned int index ) const override
	{
		flASSERT( index < RESULT_MAX );
		return	FLOAT_OP_ONE;
	}
	float	GetInstFop( unsigned int index ) const override
	{
		flASSERT( index < RESULT_MAX );
		return	1.0f;
	}
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		switch( index ){
		case InfoType::INFO_COUNT:
			return	RESULT_MAX;
		case InfoType::INFO_PROGRESS:
			return	RESULT_MAX;
		case InfoType::INFO_LOOP:
			return	LoopCount;
		}
		flASSERT( 0 );
		return	0;
	}

	const char*		GetTestName() const override;
	const char*		GetInstructionName( unsigned int result_index ) const override;
	unsigned int	GetLoopType() const override
	{
		return	LOOPTYPE_MAT;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



