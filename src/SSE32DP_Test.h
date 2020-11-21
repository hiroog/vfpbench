// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	SSE32DP_TEST_H_
#define	SSE32DP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace SSE32DP {
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

		RESULT_SSE_MULSD_IR8,
		RESULT_SSE_ADDSD_IR8,
		RESULT_SSE_FMADDSD_IR8,

		RESULT_SSE_MULPD_IR8,
		RESULT_SSE_ADDPD_IR8,
		RESULT_SSE_MULPD_ADDPD_IR8,
		RESULT_SSE_FMADDPD_IR8,

		RESULT_SSE_ML_AD_ADDPD_IR6,



		RESULT_SSE_MULSD_IRS4,
		RESULT_SSE_ADDSD_IRS4,

		RESULT_SSE_MULPD_IRS4,
		RESULT_SSE_ADDPD_IRS4,



		RESULT_AVX_VMULPD_IR4,
		RESULT_AVX_VADDPD_IR4,
		RESULT_AVX_VMULPD_VADDPD_IR4,
		RESULT_AVX_VFMADDPD_IR4,


		RESULT_AVX_VML_AD_VADDPD_IR6,




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
		return	LOOPTYPE_DP;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



