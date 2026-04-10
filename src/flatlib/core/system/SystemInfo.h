// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>
#include	<flatlib/core/system/Types.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

enum class CPUArch : unsigned int {
	CPU_UNKNOWN,
	CPU_ARM5,		// Android
	CPU_ARM6,		// Raspberry Pi
	CPU_ARM7,		// iOS, Linux, Android, Termux, Raspberry Pi 2, Windows
	CPU_ARM64,		// iOS, Linux, Android, Termux, Raspberry Pi 3/4, Jetson, macOS, Windows
	CPU_X86,		// Linux, Android, Termux, Windows, macOS
	CPU_X64,		// Linux, Android, Termux, Windows, macOS
	CPU_MIPS32,		// Android
	CPU_MIPS64,		// Android
	CPU_PPC64,		// PS3 Linux
	CPU_WASM32,		// WebAssembly
	CPU_WASM64,		// WebAssembly
};


enum class CPUFeature : unsigned int {
	IA_SSE,
	IA_SSE2,
	IA_SSE3,
	IA_SSSE3,
	IA_SSE41,
	IA_SSE42,
	IA_SSE4A,
	IA_AVX,
	IA_AVX2,
	IA_AVX512F,
	IA_AVX512BW,
	IA_AVX512DQ,
	IA_AVX512VL,
	IA_AVX512VNNI,
	IA_AVX512BF16,
	IA_AVX512FP16,
	IA_AES,
	IA_FMA3,
	IA_FMA4,
	IA_SHA,
	IA_F16C,
	IA_X64,
	IA_AVXVNNI,
	//--
	ARM_NEON,		// AdvSIMD		asimd
	ARM_VFPV4,		// (32bit only = fma/half)
	ARM_FPHP,		// FEAT_FP16	fphp
	ARM_SIMDHP,		// FEAT_FP16	asimdhp
	ARM_SIMDDP,		// FEAT_DotProd	asimddp
	ARM_SVE,		// FEAT_SVE		sve
	ARM_SVE2,		// FEAT_SVE2	sve2
	ARM_CRC32,		// FEAT_CRC32	crc32
	ARM_SHA1,		// FEAT_SHA1	sha1
	ARM_SHA2,		// FEAT_SHA256	sha2
	ARM_SHA3,		// FEAT_SHA3	sha3
	ARM_SHA512,		// FEAT_SHA512	sha512
	ARM_AES,		// FEAT_AES		aes
	ARM_64,
	ARM_I8MM,		// FEAT_I8MM	i8mm
	ARM_BF16,		// FEAT_BF16	bf16
	ARM_SVEI8MM,	// FEAT_I8MM	svei8mm
	ARM_SVEBF16,	// FEAT_BF16	svebf16
	ARM_SME,		// FEAT_SME		sme
	ARM_SME2,		// FEAT_SME2	sme2
	ARM_FHM,		// FEAT_FHM		asimdfhm
	//--
	MIPS_MSA,
	MIPS_F64,
	MIPS_PS,
	//--
	FEATURE_MAX,
};

struct CPUFeatureNameTable {
	const char*	FeatureName;
	CPUFeature	Feature;
};

struct CPUCoreInfo {
	uint64_t		ClusterMask;
	uint64_t		ThreadMask;		// SMT Mask
	unsigned int	Index;			// == Thread ID
	unsigned int	PhysicalCoreID;
	unsigned int	CoreGroupID;	// big.LITTLE Cluster
	unsigned int	CoreClock;		// KHz	3600000 = 3.6GHz
	unsigned int	BaseClock;		// KHz	2100000 = 2.1GHz
};

/*!
	@code
		system::SystemInfo	info;
		info.Init();
		auto	thread_count= info.GetTotalThreadCount();
		auto	core_count= info.GetPhysicalCoreCount();
		bool	bfma3= info.HasInstructionSet( system::CPUFeature::IA_FMA3 );
	@endcode
*/
class SystemInfo {
public:
	enum : unsigned int {
		DEVICE_NAME_BUFFER_SIZE	=	256,
		CPU_FEATURE_MAX			=	64,
		CPU_COUNT_MAX			=	128,
	};
private:
	CPUCoreInfo		CoreList[CPU_COUNT_MAX];
	uint64_t		InstructionSet;
	unsigned int	TotalThreadCount;
	unsigned int	PhysicalCoreCount;
	unsigned int	CoreGroupCount;
	CPUArch			InstArch;
	CPUArch			CompiledArch;
	bool			Initialized;
	bool			Net_HasIPV6;
	char			DeviceName[DEVICE_NAME_BUFFER_SIZE];
	static CPUFeatureNameTable	FeatureNameTable[];
private:
	void	DecodeCpuInfo();
	void	DecodeCpuTopologyImmediate();
	void	DecodeCpuTopology();
	void	DecodeCpuClock();
	void	DecodeVersion();
	void	GetCPUSpecification();
	void	SetCoreInfo( unsigned int cpu_id, unsigned int cpu_count, unsigned int group_id, unsigned int physical_id, unsigned int core_clock, unsigned int ht_step= 1 );
public:
	SystemInfo();
	void	Init();
	unsigned int	GetTotalThreadCount() const
	{
		return	TotalThreadCount;
	}
	unsigned int	GetPhysicalCoreCount() const
	{
		return	PhysicalCoreCount;
	}
	unsigned int	GetCoreGroupCount() const
	{
		return	CoreGroupCount;
	}
	unsigned int	GetThreadCount( unsigned int group_index ) const;
	unsigned int	GetCoreClock( unsigned int group_index ) const;
	uint64_t		GetAffinityMask( unsigned int group_index ) const;
	bool IsSingleThread() const
	{
		return	GetTotalThreadCount() == 1;
	}
	bool	HasInstructionSet( CPUFeature feature ) const;
	void	SetInstructionSet( CPUFeature feature );

	const char*		GetDeviceName() const;
	CPUArch			GetInstArch() const;
	CPUArch			GetCompiledArch() const;
	const char* 	GetArchNameLong( CPUArch arch ) const;
	const char* 	GetArchNameShort( CPUArch arch ) const;

	const char*		GetFeatureName( CPUFeature feature ) const;
	void			DumpCpuGroup() const;
	void			GetCpuFeatureString( char* buffer, size_t buffer_size ) const;
	void			DumpSystemInfo() const;
	bool			HasIPV6_() const;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



