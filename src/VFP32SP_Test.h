// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP32SP_TEST_H_
#define	VFP32SP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace VFP32SP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		//DEFAULT_LOOP	=	100000000,
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum {
		RESULT_VFP_FMULS_IR8,
		RESULT_VFP_FADDS_IR8,
		RESULT_VFP_FMACS_IR8,
		RESULT_VFP_VFMA_F32_IR8,

		RESULT_NEON_D_VMUL_F32_IR8,
		RESULT_NEON_D_VADD_F32_IR8,
		RESULT_NEON_D_VMLA_F32_IR8,
		RESULT_NEON_D_VFMA_F32_IR8,

		RESULT_NEON_Q_VMUL_F32_IR8,
		RESULT_NEON_Q_VADD_F32_IR8,
		RESULT_NEON_Q_VMLA_F32_IR8,
		RESULT_NEON_Q_VFMA_F32_IR8,

		RESULT_VFP_FMULS_IRS4,
		RESULT_VFP_FADDS_IRS4,
		RESULT_VFP_FMACS_IRS4,
		RESULT_VFP_VFMA_F32_IRS4,

		RESULT_NEON_D_VMUL_F32_IRS4,
		RESULT_NEON_D_VADD_F32_IRS4,
		RESULT_NEON_D_VMLA_F32_IRS4,
		RESULT_NEON_D_VFMA_F32_IRS4,

		RESULT_NEON_Q_VMUL_F32_IRS4,
		RESULT_NEON_Q_VADD_F32_IRS4,
		RESULT_NEON_Q_VMLA_F32_IRS4,
		RESULT_NEON_Q_VFMA_F32_IRS4,


		RESULT_VFP_FMULS_IR1,
		RESULT_VFP_FADDS_IR1,
		RESULT_VFP_FMACS_IR1,
		RESULT_VFP_VFMA_F32_IR1,

		RESULT_NEON_D_VMUL_F32_IR1,
		RESULT_NEON_D_VADD_F32_IR1,
		RESULT_NEON_D_VMLA_F32_IR1,
		RESULT_NEON_D_VFMA_F32_IR1,

		RESULT_NEON_Q_VMUL_F32_IR1,
		RESULT_NEON_Q_VADD_F32_IR1,
		RESULT_NEON_Q_VMLA_F32_IR1,
		RESULT_NEON_Q_VFMA_F32_IR1,


		RESULT_NEON_Q_VMUL_F32_IR12,
		RESULT_NEON_Q_VADD_F32_IR12,
		RESULT_NEON_Q_VMLA_F32_IR12,
		RESULT_NEON_Q_VFMA_F32_IR12,

		RESULT_MAX,
	};
private:
	unsigned int	GetProgressStep() const;
public:

	FloatTest();

	void	Run() override;
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		switch( index ){
		case InfoType::INFO_COUNT:
			return	RESULT_MAX;
		case InfoType::INFO_PROGRESS:
			return	GetProgressStep();
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
		return	LOOPTYPE_SP;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



