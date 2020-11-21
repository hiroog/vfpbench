// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	SSE32SP_TEST_H_
#define	SSE32SP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace SSE32SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		//DEFAULT_LOOP	=	100000000,
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,	// 40
		PER_LOOP_INST_6	=	7 * 6,	// 42
		PER_LOOP_INST_12=	5 * 12,	// 60
	};
	enum {

		RESULT_SSE_MULSS_IR8,
		RESULT_SSE_ADDSS_IR8,
		RESULT_SSE_FMADDSS_IR8,

		RESULT_SSE_MULPS_IR8,
		RESULT_SSE_ADDPS_IR8,
		RESULT_SSE_MULPS_ADDPS_IR8,
		RESULT_SSE_FMADDPS_IR8,

		RESULT_SSE_ML_AD_ADDPS_IR6,



		RESULT_SSE_MULSS_IRS4,
		RESULT_SSE_ADDSS_IRS4,

		RESULT_SSE_MULPS_IRS4,
		RESULT_SSE_ADDPS_IRS4,



		RESULT_AVX_VMULPS_IR4,
		RESULT_AVX_VADDPS_IR4,
		RESULT_AVX_VMULPS_VADDPS_IR4,
		RESULT_AVX_VFMADDPS_IR8,
		RESULT_AVX_VFMADDPS_IR12,

		RESULT_AVX_VML_AD_VADDPS_IR6,



		RESULT_MAX,
	};
public:

	FloatTest();

	void	Run() override;
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
		FL_ASSERT( 0 );
		return	0;
	}

	const char*		GetTestName() const override;
	const char*		GetInstructionName( unsigned int result_index ) const override;
	unsigned int	GetLoopType() const override
	{
		return	LOOPTYPE_SP;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



