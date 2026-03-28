// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>


/*!
	@file
	@brief

	@li FL_CPU_SIMD			0 or 1
	@li FL_CPU_SIMD_SSE		SSE version 0,2,3,4
	@li FL_CPU_SIMD_AVX		AVX version 0,1,2,512
	@li FL_CPU_SIMD_NEON	NEON version 0,1,64


	@code
	FL_CPU_SIMD_SSE		=	2	SSE2
	FL_CPU_SIMD_SSE		=	3	SSSE3
	FL_CPU_SIMD_SSE		=	4	SSE4.1

	FL_CPU_SIMD_AVX		=	1	AVX1
	FL_CPU_SIMD_AVX		=	2	AVX2/FMA3
	FL_CPU_SIMD_AVX		=	512	AVX512F/BW/DQ

	FL_CPU_SIMD_NEON	=	1	ARMv7A NEON
	FL_CPU_SIMD_NEON	=	64	ARMv8A NEON
	@endcode


	@code
	* SSE
		Intel-Atom
			Bonnell/Saltwell		SSSE3		-- Android
			Silvermont/Airmont		SSE4.2/AES	-- Windows
			Goldmont/GoldmontPlus	SSE4.2/AES
		Intel-Desktop
			Pentium4				SSE2
			Prescot(P4)				SSE3
			Merom(Core2)			SSSE3
			Penryn(Core2)			SSE4.1
			Nehalem/Westmere		SSE4.2/AES
			SandyBridge				AVX1/AES		-- MacOSX
			IvyBridge				AVX1/AES/F16C
			Haswell/Broadwell		AVX2/FMA3/AES/F16C	-- macOS 11 BigSur
			Sky/Kaby/Coffee/Comet	AVX2/FMA3/AES/F16C
			Ice/Tiger/RocketLake	AVX2/AVX512/VNNI/AES/F16C
		AMD
			Bobcat					SSSE3
			Jaguar					AVX1/AES/F16C	-- Console
			Puma					AVX1/AES/F16C
			Bulldozer				AVX1/FMA4/AES/F16C
			Piledriver				AVX1/FMA4/FMA3/AES/F16C
			Steamroller				AVX1/FMA4/FMA3/AES/F16C
			Excavator				AVX2/FMA4/FMA3/AES/F16C
			Zen/Zen+				AVX2/FMA3/AES/F16C
			Zen2					AVX2/FMA3/AES/F16C	-- Console
			Zen3					AVX2/FMA3/AES/F16C
	* NEON
		ARM
			ARM9					None			-- Android ARMv5TE
			ARM11					VFPv2			-- Linux arm6l
			Cortex-A8				VFPv3/NEON
			Cortex-A5				VFPv3/NEON
			Cortex-A9				VFPv3			-- Android ARMv7A
			Cortex-A15/A7			VFPv4/NEON
			Cortex-A17/A7			VFPv4/NEON
			Cortex-A57/A53			v8.0a AArch64/NEON	-- Linux aarch64
			Cortex-A72/A53			v8.0a AArch64/NEON
			Cortex-A73/A53			v8.0a AArch64/NEON			snapdragon 835
			Cortex-A75/A55			v8.2a AArch64/NEON/fp16		snapdragon 845
			Cortex-A76				v8.3a AArch64/NEON/fp16		snapdragon 855
			Cortex-A77				v8.3a AArch64/NEON/fp16		snapdragon 865
		Qualcomm
			Scorpion				VFPv3/NEON
			Krait					VFPv4/NEON
			Kryo					v8.0a AArch64/NEON		snapdragon 820
		NVIDIA
			Denver					v8.0a AArch64/NEON	-- Android ARMv8A
		Apple
			Swift(A6)				VFPv4/NEON			-- iOS armv7s
			Cyclone(A7)				v8.0a AArch64/NEON	-- iOS arm64
			Typhoon(A8)				v8.0a AArch64/NEON	-- iOS arm64
			Twister(A9)				v8.0a AArch64/NEON	-- iOS arm64
			Hurricane/Zephyr(A10)	v8.1a AArch64/NEON	-- iOS
			Monsoon/Mistral(A11)	v8.2a AArch64/NEON	-- iOS
			Vortex/Tempest(A12)		v8.3a AArch64/NEON	-- iOS
			Lightning/Thunder(A13)	v8.4a AArch64/NEON	-- iOS
			Firestorm/Icestorm(A14)	v8.4a AArch64/NEON	-- iOS
	@endcode

*/

//-----------------------------------------------------------------------------
#if FL_CPU_X86 || FL_CPU_X64

#if FL_CC_VC
# include	<intrin.h>
#elif FL_CC_CLANG_GCC
# include	<x86intrin.h>
#endif

#if defined(__AVX512F__)
# define	FL_CPU_SIMD_AVX		512
# define	FL_CPU_SIMD_SSE		4
#elif defined(__AVX2__)
# define	FL_CPU_SIMD_AVX		2
# define	FL_CPU_SIMD_SSE		4
#elif defined(__AVX__)
# define	FL_CPU_SIMD_AVX		1
# define	FL_CPU_SIMD_SSE		4
#elif defined(__SSE4_2__)
# define	FL_CPU_SIMD_SSE		4
#else
# define	FL_CPU_SIMD_SSE		3
#endif

#if defined(__F16C__)
# define	FL_CPU_F16C			1
#endif
#if defined(__AES__)
# define	FL_CPU_AESNI		1
#endif


#endif	// X86/X64
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_CPU_ARM7 || FL_CPU_ARM64

#if FL_CC_CLANG_GCC
# if defined(__ARM_NEON__) || defined(__ARM_NEON)
#  include	<arm_neon.h>
#  if FL_CPU_ARM64
#   define	FL_CPU_SIMD_NEON	64
#  else
#   define	FL_CPU_SIMD_NEON	1
#  endif
# endif
# if defined(__ARM_FETURE_FP16_VECTOR_ARITHMETIC)
#  define	FL_CPU_SIMD_FP16	1
# endif
#endif

#endif // ARM
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#ifndef	FL_CPU_SIMD_SSE
# define	FL_CPU_SIMD_SSE		0
#endif

#ifndef	FL_CPU_SIMD_AVX
# define	FL_CPU_SIMD_AVX		0
#endif

#ifndef	FL_CPU_F16C
# define	FL_CPU_F16C			0
#endif

#ifndef	FL_CPU_AESNI
# define	FL_CPU_AESNI		0
#endif

#ifndef	FL_CPU_SIMD_NEON
# define	FL_CPU_SIMD_NEON	0
#endif

#ifndef	FL_CPU_SIMD_FP16
# define	FL_CPU_SIMD_FP16	0
#endif

#if FL_CPU_SIMD_SSE || FL_CPU_SIMD_AVX || FL_CPU_SIMD_NEON
# define	FL_CPU_SIMD			1
#else
# define	FL_CPU_SIMD			0
#endif
//-----------------------------------------------------------------------------

