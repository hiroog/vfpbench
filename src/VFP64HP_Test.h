// 2019 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP64HP_TEST_H_
#define	VFP64HP_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace VFP64HP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum : unsigned int {
		RESULT_VFP_FMUL_IR8,
		RESULT_VFP_FADD_IR8,
		RESULT_VFP_FMADD_IR8,

		RESULT_NEON_FMUL_H4_IR8,
		RESULT_NEON_FADD_H4_IR8,
		RESULT_NEON_FMLA_H4_IR8,

		RESULT_NEON_FMUL_H8_IR8,
		RESULT_NEON_FADD_H8_IR8,
		RESULT_NEON_FMLA_H8_IR8,


		RESULT_VFP_FMUL_IRS4,
		RESULT_VFP_FADD_IRS4,
		RESULT_VFP_FMADD_IRS4,

		RESULT_NEON_FMUL_H4_IRS4,
		RESULT_NEON_FADD_H4_IRS4,
		RESULT_NEON_FMLA_H4_IRS4,

		RESULT_NEON_FMUL_H8_IRS4,
		RESULT_NEON_FADD_H8_IRS4,
		RESULT_NEON_FMLA_H8_IRS4,


		RESULT_VFP_FMUL_IR1,
		RESULT_VFP_FADD_IR1,
		RESULT_VFP_FMADD_IR1,

		RESULT_NEON_FMUL_H4_IR1,
		RESULT_NEON_FADD_H4_IR1,
		RESULT_NEON_FMLA_H4_IR1,

		RESULT_NEON_FMUL_H8_IR1,
		RESULT_NEON_FADD_H8_IR1,
		RESULT_NEON_FMLA_H8_IR1,


		RESULT_NEON_FMUL_H8_IR12,
		RESULT_NEON_FADD_H8_IR12,
		RESULT_NEON_FMLA_H8_IR12,


		RESULT_SVE_FMUL_H_IR12,
		RESULT_SVE_FADD_H_IR12,
		RESULT_SVE_FMLA_H_IR12,

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
		FL_ASSERT( 0 );
		return	0;
	}

	const char*		GetTestName() const override;
	const char*		GetInstructionName( unsigned int result_index ) const override;
	unsigned int	GetLoopType() const override
	{
		return	LOOPTYPE_HP;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



