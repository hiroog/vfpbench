// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"SystemInfo.h"
#include	"ThreadFunction.h"
#include	"FixedArray.h"
#include	"SystemAPI.h"
#include	"ThreadAtomic.h"
#include	"Matrix4.h"

#if flOS_LINUX
# include	<stdio.h>
# include	<stdlib.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<map>
#endif

#if (flCPU_X86 || flCPU_X64) && flCC_CLANG
# include	<cpuid.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

SystemInfo	Info;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if flCPU_X86 || flCPU_X64
//-----------------------------------------------------------------------------

enum {
	//-- id=1 ECX
	CPUID_1_ECX_FEATURE_SSE3		=	(1<< 0),
	CPUID_1_ECX_FEATURE_MONITOR		=	(1<< 3),
	CPUID_1_ECX_FEATURE_SSSE3		=	(1<< 9),
	CPUID_1_ECX_FEATURE_FMA3		=	(1<<12),
	CPUID_1_ECX_FEATURE_SSE41		=	(1<<19),
	CPUID_1_ECX_FEATURE_SSE42		=	(1<<20),
	CPUID_1_ECX_FEATURE_AES			=	(1<<25),
	CPUID_1_ECX_FEATURE_AVX			=	(1<<28),
	CPUID_1_ECX_FEATURE_F16C		=	(1<<29),
	CPUID_1_ECX_FEATURE_RAND		=	(1<<30),
	//-- id=1 EDX
	CPUID_1_EDX_FEATURE_MMX			=	(1<<23),
	CPUID_1_EDX_FEATURE_SSE			=	(1<<25),
	CPUID_1_EDX_FEATURE_SSE2		=	(1<<26),
	CPUID_1_EDX_FEATURE_HT			=	(1<<28),
	//-- id=7 EBX
	CPUID_7_EBX_FEATURE_AVX2		=	(1<< 5),
	CPUID_7_EBX_FEATURE_AVX512F		=	(1<<16),	// Foundation
	CPUID_7_EBX_FEATURE_AVX512DQ	=	(1<<17),	// Doubleword and Quadword instructions
	CPUID_7_EBX_FEATURE_AVX512IFMA	=	(1<<21),	// 
	CPUID_7_EBX_FEATURE_AVX512PF	=	(1<<26),	//
	CPUID_7_EBX_FEATURE_AVX512ER	=	(1<<27),	//
	CPUID_7_EBX_FEATURE_AVX512CD	=	(1<<28),	//
	CPUID_7_EBX_FEATURE_SHA			=	(1<<29),
	CPUID_7_EBX_FEATURE_AVX512BW	=	(1<<30),	// Byte and Word instructions
	CPUID_7_EBX_FEATURE_AVX512VL	=	(1<<31),	// Vector Length extensions
	//-- id=7 ECX
	CPUID_7_ECX_FEATURE_AVX512VBMI	=	(1<< 1),	//
	CPUID_7_ECX_FEATURE_AVX512VBMI2	=	(1<< 6),	//
	CPUID_7_ECX_FEATURE_GFNI		=	(1<< 8),	//
	CPUID_7_ECX_FEATURE_VAES		=	(1<< 9),	//
	CPUID_7_ECX_FEATURE_AVX512VNNI	=	(1<<11),	// Neural Network Instructions
	CPUID_7_ECX_FEATURE_AVX512BITALG=	(1<<12),	//
	CPUID_7_ECX_FEATURE_AVX512VPOPCNTDQ=(1<<14),	//
	//-- id=7 EDX
	CPUID_7_ECX_FEATURE_AVX5124VNNIW=	(1<< 2),	//
	CPUID_7_ECX_FEATURE_AVX5124FMAPS=	(1<< 3),	//
	//-- id=0x80000001 ECX
	CPUID_80_ECX_FEATURE_SSE4A		=	(1<< 6),
	CPUID_80_ECX_FEATURE_FMA4		=	(1<<16),
	//-- id=0x80000001 EDX
	CPUID_80_EDX_FEATURE_X64		=	(1<<29),	// Long Mode
	CPUID_80_EDX_FEATURE_3DNOWEXT	=	(1<<30),
	CPUID_80_EDX_FEATURE_3DNOW		=	(1<<31),
};

struct RegCPUID {
	unsigned int	EAX;
	unsigned int	EBX;
	unsigned int	ECX;
	unsigned int	EDX;
};

static void	GetCPUID( RegCPUID& reg, unsigned int index )
{
#if flCC_VC
	__cpuid( reinterpret_cast<int*>(&reg.EAX), index );
#endif
#if flCC_CLANG || flCC_GCC
	unsigned int	a= 0;
	unsigned int	b= 0;
	unsigned int	c= 0;
	unsigned int	d= 0;
	__cpuid( index, a, b, c, d );
	reg.EAX= a;
	reg.EBX= b;
	reg.ECX= c;
	reg.EDX= d;
#endif
}

void	SystemInfo::GetCPUSpecification()
{
	RegCPUID	Feature01;
	RegCPUID	Feature07;
	RegCPUID	Feature80;

	GetCPUID( Feature01, 1 );
	GetCPUID( Feature07, 7 );
	GetCPUID( Feature80, 0x80000001 );


	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE3 ){
		SetInstructionSet( CPUFeature::IA_SSE3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSSE3 ){
		SetInstructionSet( CPUFeature::IA_SSSE3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE41 ){
		SetInstructionSet( CPUFeature::IA_SSE41 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE42 ){
		SetInstructionSet( CPUFeature::IA_SSE42 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_AES ){
		SetInstructionSet( CPUFeature::IA_AES );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_AVX ){
		SetInstructionSet( CPUFeature::IA_AVX );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_FMA3 ){
		SetInstructionSet( CPUFeature::IA_FMA3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_F16C ){
		SetInstructionSet( CPUFeature::IA_F16C );
	}


	if( Feature01.EDX & CPUID_1_EDX_FEATURE_SSE ){
		SetInstructionSet( CPUFeature::IA_SSE );
	}
	if( Feature01.EDX & CPUID_1_EDX_FEATURE_SSE2 ){
		SetInstructionSet( CPUFeature::IA_SSE2 );
	}


	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX2 ){
		SetInstructionSet( CPUFeature::IA_AVX2 );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512F ){
		SetInstructionSet( CPUFeature::IA_AVX512F );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512DQ ){
		SetInstructionSet( CPUFeature::IA_AVX512DQ );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_SHA ){
		SetInstructionSet( CPUFeature::IA_SHA );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512BW ){
		SetInstructionSet( CPUFeature::IA_AVX512BW );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512VL ){
		SetInstructionSet( CPUFeature::IA_AVX512VL );
	}


	if( Feature80.ECX & CPUID_80_ECX_FEATURE_FMA4 ){
		SetInstructionSet( CPUFeature::IA_FMA4 );
	}

	if( Feature80.EDX & CPUID_80_EDX_FEATURE_X64 ){
		SetInstructionSet( CPUFeature::IA_X64 );
	}
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#if flOS_LINUX
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------

static unsigned int GetBitCount( uint64_t bit )
{
	unsigned int	count= 0;
	for( unsigned int i= 0 ; i< 32 ; i++, bit>>= 1 ){
		if( bit & 1 ){
			count++;
		}
	}
	return	count;
}

static bool	LoadOneLine( char* line_buffer, size_t buffer_size, const char* path )
{
	*line_buffer= '\0';
	FILE*	fp= fopen( path, "r" );
	if( fp ){
		if( !fgets( line_buffer, buffer_size-1, fp ) ){
			*line_buffer= '\0';
		}
		fclose( fp );
	}
	return	*line_buffer != '\0';
}

static unsigned int		IsExists( const char* path )
{
	struct stat	sbuf;
	if( stat( path, &sbuf ) == -1 ){
		return	0;
	}
	if( S_ISDIR( sbuf.st_mode ) ){
		return	2;
	}
	if( S_ISREG( sbuf.st_mode ) ){
		return	1;
	}
	return	0;
}

static char*	SkipColon( char* ptr )
{
	for(; *ptr && *ptr != ':' ; ptr++ );
	if( *ptr == ':' ){
		ptr++;
	}
	return	ptr;
}

static char*	SkipSpace( char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

static const char*	SkipSpace( const char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

static uint64_t	DecodeCpuRegion( const char* ptr )
{
	uint64_t		bit= 0;
	unsigned int	previd= 0;
	for(; *ptr ;){
		bool	region= false;
		ptr= SkipSpace( ptr );
		if( *ptr == '-' ){
			region= true;
			ptr++;
		}
		if( *ptr < '0' || *ptr > '9' ){
			break;
		}
		unsigned int id= atoi( ptr );
		for(; *ptr >= '0' && *ptr <= '9' ; ptr++ );
		for(; *ptr == ',' ; ptr++ );
		if( region ){
		   for( unsigned int i= previd+1 ; i< id ; i++ ){
			   bit|= 1<<i;
		   }
		}
		bit|= 1<<id;
		previd= id;
	}
	return	bit;
}

static unsigned int GetCpuCount( const char* path )
{
	const int	MAX_LINE_SIZE= 128;
	char	line_buffer[MAX_LINE_SIZE];
	if( LoadOneLine( line_buffer, MAX_LINE_SIZE-2, path ) ){
		return	GetBitCount( DecodeCpuRegion( line_buffer ) );
	}
	return	0;
}


//-----------------------------------------------------------------------------
}

void SystemInfo::DumpCpuGroup() const
{
	FL_LOG( "Total Core Cound : %d\n", TotalThreadCount );
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		FL_PRINT( "%2d: P:%02d G:%02d %8dKHz G:%08llx T:%08llx\n", core.Index, core.PhysicalCoreID, core.ClusterGroupID, core.CoreClock, core.ClusterMask, core.ThreadMask );
	}
}

void SystemInfo::DecodeCpuTopologyImmediate()
{
	{
		const int	MAX_PATH_SIZE= 256;
		const int	MAX_LINE_SIZE= 256;
		char	path_buffer[MAX_PATH_SIZE];
		char	line_buffer[MAX_LINE_SIZE];
		for( unsigned int ci= 0 ; ci< CPU_COUNT_MAX ; ci++ ){
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d", ci );
			if( !IsExists( path_buffer ) ){
				break;
			}
			auto&	core= CoreList[ci];
			core.Index= ci;
#if 0
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/core_id", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.PhysicalCoreID= atoi( line_buffer );
			}
#endif
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/thread_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ThreadMask= strtoull( line_buffer, nullptr, 16 );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/core_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ClusterMask= strtoull( line_buffer, nullptr, 16 );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.CoreClock= atoi( line_buffer );
			}

			TotalThreadCount= ci + 1;
		}
	}
	{
		unsigned int	core_count= TotalThreadCount;
		unsigned int	group_id= 0;
		unsigned int	thread_id= 0;
		std::map<uint64_t,unsigned int>		group_map;
		std::map<uint64_t,unsigned int>		thread_map;
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			{
				const auto&	thread= thread_map.find( core.ThreadMask );
				if( thread == thread_map.end() ){
					thread_map[ core.ThreadMask ]= thread_id++;
				}
			}
			{
				const auto&	group= group_map.find( core.ClusterMask );
				if( group == group_map.end() ){
					group_map[ core.ClusterMask ]= group_id++;
				}
			}
		}
		PhysicalCoreCount= thread_id;
		CoreGroupCount= group_id;
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			const auto&	thread= thread_map.find( core.ThreadMask );
			core.PhysicalCoreID= thread->second;
			const auto&	group= group_map.find( core.ClusterMask );
			core.ClusterGroupID= group->second;
		}
	}

	//DumpCpuGroup();
}


void SystemInfo::DecodeCpuTopology()
{
	unsigned int	present= GetCpuCount( "/sys/devices/system/cpu/present" );
	{
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 ){
			DecodeCpuTopologyImmediate();
			return;
		}
	}
	thread::Atomic<unsigned int>	ExitFlag( 0 );
	util::FixedArrayPOD<thread::ThreadFunctionBase*>	ThreadArray( present );
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= thread::CreateThreadFunction( [&ExitFlag](){
					for(; ExitFlag.Get() == 0 ;){
						math::Matrix4	m1, m2, m3;
						m1.SetIdentity();
						m2.SetIdentity();
						m3.MulCopy( m1, m2 );
					}
				});
		ThreadArray[ti]= thread;
		thread->Run();
	}
	for(;;){
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 ){
			DecodeCpuTopologyImmediate();
			ExitFlag= 1;
			break;
		}
		time::SleepMS( 50 );
	}
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= ThreadArray[ti];
		thread->Join();
		memory::SafeDelete( thread );
		ThreadArray[ti]= nullptr;
	}
}


void SystemInfo::DecodeCpuInfo()
{
	bool	info_feature= false;
	bool	info_devname= false;
	const int	MAX_LINE_BYTE= 1024 * 2;
	char	linebuffer[MAX_LINE_BYTE+8];
	FILE*	fp= fopen( "/proc/cpuinfo", "r" );
	if( !fp ){
		return;
	}
	for(; fgets( linebuffer, MAX_LINE_BYTE, fp ) ;){
//FL_LOG( "[%s]\n", linebuffer );
		if( !info_feature ){
			if(
#if flCPU_ARM6 || flCPU_ARM7 || flCPU_ARM64
				!strncmp( linebuffer, "Features", 8 )
#endif
#if flCPU_X64 || flCPU_X86
				!strncmp( linebuffer, "flags", 5 )
#endif
					){
				char*	lp= SkipColon( linebuffer );
				if( *lp ){
					for(; *lp ;){
						lp= SkipSpace( lp );
						const char*	top= lp;
						for(; *lp && !(*lp <= ' ' && *lp) ; lp++ );
						if( *lp && *lp <= ' ' ){
							*lp++= '\0';
						}
  //FL_LOG( "word=[%s]\n", top );
#if flCPU_ARM7 || flCPU_ARM64
						if( !strcmp( top, "neon" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "vfpv4" ) ){
							SetInstructionSet( CPUFeature::ARM_VFPV4 );
						}else if( !strcmp( top, "asimd" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "fphp" ) ){
							SetInstructionSet( CPUFeature::ARM_FPHP );
						}else if( !strcmp( top, "asimdhp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDHP );
						}else if( !strcmp( top, "crc32" ) ){
							SetInstructionSet( CPUFeature::ARM_CRC32 );
						}else if( !strcmp( top, "sha1" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA1 );
						}else if( !strcmp( top, "sha2" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA2 );
						}else if( !strcmp( top, "aes" ) ){
							SetInstructionSet( CPUFeature::ARM_AES );
						}
#else
						if( !strcmp( top, "aes" ) ){
						}
#endif
					}
				}
				info_feature= true;
			}
		}
		if( !info_devname ){
#if flCPU_ARM7 || flCPU_ARM64
			if( !strncmp( linebuffer, "Hardware", 8 ) ){
				char*	lp= SkipColon( linebuffer );
				lp= SkipSpace( lp );
				strcpy_s( DeviceName, DEVICE_NAME_BUFFER_SIZE-1, lp );
				info_devname= true;
			}
#endif
#if flCPU_X64 || flCPU_X86
			if( !strncmp( linebuffer, "model name", 10 ) ){
				char*	lp= SkipColon( linebuffer );
				lp= SkipSpace( lp );
				strcpy_s( DeviceName, DEVICE_NAME_BUFFER_SIZE-1, lp );
				info_devname= true;
			}
#endif
		}
	}
	fclose( fp );
}


//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
#if flCPU_ARM6 || flCPU_ARM7 || flCPU_ARM64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
#if flCPU_ARM64
	SetInstructionSet( CPUFeature::ARM_NEON );
	SetInstructionSet( CPUFeature::ARM_VFPV4 );
	SetInstructionSet( CPUFeature::ARM_64 );
#endif
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if flCPU_MIPS || flCPU_MIPS64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

SystemInfo::SystemInfo() :
			InstructionSet( 0 ),
			TotalThreadCount( 1 ),
			PhysicalCoreCount( 1 ),
			CoreGroupCount( 1 ),
			Initialized( false )
{
	flASSERT( (unsigned int)CPUFeature::FEATURE_MAX <= CPU_FEATURE_MAX );
	memory::MemClear( CoreList );
	memory::MemClear( DeviceName );
}

void	SystemInfo::Init()
{
	if( !Initialized ){
		Initialized= true;
		GetCPUSpecification();
#if flOS_LINUX
		DecodeCpuInfo();
		DecodeCpuTopology();
#endif
	}
	FL_LOG( "InstructionSet= %08x\n", InstructionSet );
}

unsigned int	SystemInfo::GetCoreClock( unsigned int group_index ) const
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			return	core.CoreClock;
		}
	}
	return	0;
}

unsigned int	SystemInfo::GetThreadCount( unsigned int group_index ) const
{
	unsigned int	thread_count= 0;
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			thread_count++;
		}
	}
	return	thread_count;
}

uint64_t	SystemInfo::GetAffinityMask( unsigned int group_index ) const
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			return	core.ClusterMask;
		}
	}
	return	0;
}

bool	SystemInfo::HasInstructionSet( CPUFeature feature ) const
{
	flASSERT( feature < CPUFeature::FEATURE_MAX );
	return	(InstructionSet& (1<<(unsigned int)feature)) != 0;
}

void	SystemInfo::SetInstructionSet( CPUFeature feature )
{
	flASSERT( feature < CPUFeature::FEATURE_MAX );
	InstructionSet|= 1<<(unsigned int)feature;
}

const char*	SystemInfo::GetDeviceName() const
{
	return	DeviceName;
}

CPUArch	SystemInfo::GetArch() const
{
#if flCPU_ARM64
	return	CPUArch::CPU_ARM64;
#elif flCPU_ARM7
	return	CPUArch::CPU_ARM7;
#elif flCPU_ARM5
	return	CPUArch::CPU_ARM5;
#elif flCPU_ARM6
	return	CPUArch::CPU_ARM6;
#elif flCPU_X86
	return	CPUArch::CPU_X86;
#elif flCPU_X64
	return	CPUArch::CPU_X64;
#elif flCPU_MIPS32
	return	CPUArch::CPU_MIPS32;
#elif flCPU_MIPS64
	return	CPUArch::CPU_MIPS64;
#else
	return	CPUArch::CPU_UNKNOWN;
#endif
}

const char* SystemInfo::GetArchNameLong() const
{
#if flCPU_ARM64
	if( HasInstructionSet( CPUFeature::ARM_FPHP ) ){
		return	"ARMv8.2A AArch64";
	}
	return	"ARMv8A AArch64";
#elif flCPU_ARM7
	return	"ARMv7A";
#elif flCPU_ARM5
	return	"ARMv5TE";
#elif flCPU_ARM6
	return	"ARMv6";
#elif flCPU_X86
	return	"x86";
#elif flCPU_X64
	return	"x64 (x86_64)";
#elif flCPU_MIPS32
	return	"MIPS32";
#elif flCPU_MIPS64
	return	"MIPS64";
#else
	return	"Unknown";
#endif
}

const char* SystemInfo::GetArchNameShort() const
{
#if flCPU_ARM64
	return	"ARMv8A";
#elif flCPU_ARM7
	return	"ARMv7A";
#elif flCPU_ARM5
	return	"ARMv5TE";
#elif flCPU_ARM6
	return	"ARMv6";
#elif flCPU_X86
	return	"x86";
#elif flCPU_X64
	return	"x64";
#elif flCPU_MIPS32
	return	"MIPS32";
#elif flCPU_MIPS64
	return	"MIPS64";
#else
	return	"Unknown";
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

