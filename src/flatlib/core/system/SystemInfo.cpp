// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/CoreBase.h"
#include	"flatlib/core/system/SystemInfo.h"
#include	"flatlib/core/memory/MemoryAllocator.h"
#include	"flatlib/core/platform/WindowsAPI.h"
#include	"flatlib/core/platform/WinString.h"

#if FL_OS_WINDOWS
# include	<intrin.h>
#endif

#if FL_OS_LINUX
# include	"flatlib/core/thread/Sleep.h"
# include	"flatlib/core/thread/AtomicValue.h"
# include	"flatlib/core/thread/ThreadInstance.h"
# include	"flatlib/core/time/SystemClock.h"
# include	"flatlib/core/math/Matrix4.h"
# include	"flatlib/core/ut/FixedArray.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<map>
#endif


#if (FL_CPU_X86 || FL_CPU_X64) && FL_CC_CLANG_GCC
# include	<cpuid.h>
#endif

#if FL_OS_LINUX && FL_CPU_ARM64
# if __ARM_FEATURE_SVE
#   include	<arm_sve.h>
# endif
#endif


#define	FL_USE_CUSTOM_CLOCK		0
#define	FL_USE_POWERBOOST_LOOP	1


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------

//=============================================================================
// OS
//=============================================================================

//-----------------------------------------------------------------------------
#if FL_OS_WINDOWS
//-----------------------------------------------------------------------------

void	SystemInfo::DecodeCpuTopology()
{
	SYSTEM_INFO	info;
	GetSystemInfo( &info );
	unsigned int	thread_count= info.dwNumberOfProcessors;

	uint64_t		all_cpu_mask= 0;
	unsigned int	group_index= 0;
	unsigned int	physical_core_index= 0;
	unsigned int	core_count= 0;

	if( false ){

		DWORD	info_size= 0;
		GetLogicalProcessorInformation( nullptr, &info_size );
		unsigned int	info_count= info_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		auto*	info_buffer= FL_MEMORY::AllocByte<SYSTEM_LOGICAL_PROCESSOR_INFORMATION>( info_size );

		auto	result= GetLogicalProcessorInformation( info_buffer, &info_size );
		if( !result ){
			return;
		}

		const auto*		info_ptr= info_buffer;
		for( unsigned int ii= 0 ; ii< info_count ; ii++, info_ptr++ ){
			switch( info_ptr->Relationship ){
			case RelationProcessorCore: {
					int		htCount= 0;
					auto	mask= info_ptr->ProcessorMask;
					for( unsigned int mi= 0 ; mi< 64 ; mi++ ){
						if( mask & (1ull<<mi) ){
							unsigned int	cpu_id= mi;
							auto&	core= CoreList[cpu_id];
							core.ClusterMask= 0;
							core.ThreadMask= mask;
							core.Index= cpu_id;
							core.PhysicalCoreID= physical_core_index;
							core.CoreGroupID= 0;
							core.CoreClock= 0;
							all_cpu_mask|= mask;
							core_count++;
						}
					}
					physical_core_index++;
				}
				break;
#if 0
			case RelationNumaNode: // 1
				FL_LOG( "   numa node=%d %llx\n", info_ptr->NumaNode.NodeNumber, info_ptr->ProcessorMask );
				break;
			case RelationCache: // 2
				FL_LOG( "   cache L%d %d Line=%d Size=%d %d\n",
							info_ptr->Cache.Level,
							info_ptr->Cache.Associativity,
							info_ptr->Cache.LineSize,
							info_ptr->Cache.Size,
							(int)info_ptr->Cache.Type // 0=Unified, 1=Inst, 2=Data, 3=Trace
						);
				break;
			case RelationProcessorPackage: // 3
				FL_LOG( "   Package=%llx\n", info_ptr->ProcessorMask );
				break;
			case RelationGroup: // 4
				FL_LOG( "   Group=%llx\n", info_ptr->ProcessorMask );
				break;
#endif
			default:
				break;
			}
		}

		FL_MEMORY::ZFree( info_buffer );

	}else{

		DWORD	info_size= 0;
//		auto	info_mode= RelationAll;
//		auto	info_mode= RelationGroup;
//		auto	info_mode= RelationProcessorPackage;
//		auto	info_mode= RelationCache;
		auto	info_mode= RelationProcessorCore;
		GetLogicalProcessorInformationEx( info_mode, nullptr, &info_size );
		auto*	info_buffer= FL_MEMORY::AllocByte<uint8_t>( info_size );
		auto	result= GetLogicalProcessorInformationEx( info_mode, reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>( info_buffer ), &info_size );
		if( !result ){
			return;
		}

		const auto*		info_ptr= info_buffer;
		const auto*		info_ptr_end= info_buffer + info_size;

		for( unsigned int ii= 0 ; info_ptr< info_ptr_end ; ii++ ){
			const auto*		info= reinterpret_cast<const SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>( info_ptr );
			switch( info->Relationship ){
			case RelationProcessorCore: { // 0
					int		htCount= 0;
					auto	mask= info->Processor.GroupMask[0].Mask;
					for( unsigned int mi= 0 ; mi< 64 ; mi++ ){
						if( mask & (1ull<<mi) ){
							unsigned int	cpu_id= mi;
							auto&	core= CoreList[cpu_id];
							core.ClusterMask= 0;
							core.ThreadMask= mask;
							core.Index= cpu_id;
							core.PhysicalCoreID= physical_core_index;
							core.CoreGroupID= info->Processor.EfficiencyClass;
							core.CoreClock= 0;
							all_cpu_mask|= mask;
							core_count++;
							if( group_index >= core.CoreGroupID ){
								group_index= core.CoreGroupID;
							}
						}
					}
					physical_core_index++;
				}
				break;
#if 0
			case RelationNumaNode: // 1
				FL_LOG( "   numa node=%d\n",	
								info->NumaNode.NodeNumber
								);
				break;
			case RelationCache: // 2
				FL_LOG( "   cache L%d %d Line=%d Size=%d type=%d %llx\n",
							info->Cache.Level,
							info->Cache.Associativity,
							info->Cache.LineSize,
							info->Cache.CacheSize,
							(int)info->Cache.Type, // 0=Unified, 1=Inst, 2=Data, 3=Trace
							info->Cache.GroupMask.Mask
						);
				break;
			case RelationProcessorPackage: // 3
				FL_LOG( "   Package\n" );
				break;
			case RelationGroup: // 4
				FL_LOG( "   MaxGroupCount=%d ActiveGroupCount=%d  act=%llx\n",
								info->Group.MaximumGroupCount,
								info->Group.ActiveGroupCount,
								info->Group.GroupInfo[0].ActiveProcessorMask );
				break;
#endif
			default:
				break;
			}
			info_ptr+= info->Size;
		}

		FL_MEMORY::ZFree( info_buffer );


	}


	FL_ASSERT( thread_count == core_count );
	{
		unsigned int	count= 0;
		for( unsigned int ci= 0 ; ci< 64 ; ci++ ){
			if( all_cpu_mask & (1ull<<ci) ){
				count++;
			}
		}
		FL_ASSERT( count == core_count );
	}

	unsigned int	group_count= group_index + 1;
	{
		constexpr unsigned int	MAX_GROUP= 4;
		FL_ASSERT( group_count <= MAX_GROUP );
		uint64_t	GroupMask[MAX_GROUP];
		memset( GroupMask, 0, sizeof(GroupMask) );
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			const auto&	core= CoreList[ci];
			GroupMask[core.CoreGroupID]|= core.ThreadMask;
		}
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			core.ClusterMask= GroupMask[core.CoreGroupID];
		}
	}

	unsigned int	cpu_clock= 0;
	HKEY	key;
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key ) == ERROR_SUCCESS ){
		DWORD	clock= 0;
		DWORD	clock_size= sizeof(DWORD);
		RegQueryValueEx( key, "~MHz", nullptr, nullptr, reinterpret_cast<LPBYTE>(&clock), &clock_size );
		DWORD	name_size= DEVICE_NAME_BUFFER_SIZE-1;
		RegQueryValueEx( key, "ProcessorNameString", nullptr, nullptr, reinterpret_cast<LPBYTE>(&DeviceName), &name_size );
		DeviceName[name_size]= '\0';
		RegCloseKey( key );
		cpu_clock= clock * 1000;
	}

	TotalThreadCount= core_count;
	PhysicalCoreCount= physical_core_index;
	CoreGroupCount= group_count;

	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		auto&	core= CoreList[ci];
		core.CoreClock= cpu_clock;
	}
}


//-----------------------------------------------------------------------------
#endif // Windows
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_OS_LINUX
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------

static bool	IsWSL2()
{
	FILE*	fp= fopen( "/proc/version", "r" );
	if( !fp ){
		return	false;
	}
	bool	is_wsl2= false;
	const int	MAX_LINE_BYTE= 1024 * 2;
	char	linebuffer[MAX_LINE_BYTE+8];
	if( fgets( linebuffer, MAX_LINE_BYTE, fp ) ){
		const char*	ptr= linebuffer;
		for(; *ptr && *ptr != '#' ; ptr++ ){
			if( strncmp( ptr, "WSL2", 4 ) == 0 ){
				is_wsl2= true;
				break;
			}
		}
	}
	fclose( fp );
	return	is_wsl2;
}

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

static uint64_t	GetCpuBitMask( const char* line )
{
	const int	MAX_MASK= 8;
	uint64_t	mask_buffer[MAX_MASK];
	unsigned int	mask_index= 0;
	const char*	ptr= line;
	for(; *ptr && mask_index < MAX_MASK ;){
		auto	mask= strtoull( ptr, nullptr, 16 );
		mask_buffer[mask_index++]= mask;
		for(; *ptr && *ptr != ',' ; ptr++ );
		if( *ptr == ',' ){
			ptr++;
		}
	}
	if( mask_index ){
		return	mask_buffer[mask_index-1];
	}
	return	0;
}


//-----------------------------------------------------------------------------
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
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/thread_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ThreadMask= GetCpuBitMask( line_buffer );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/core_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ClusterMask= GetCpuBitMask( line_buffer );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.CoreClock= atoi( line_buffer );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/cpufreq/base_frequency", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.BaseClock= atoi( line_buffer );
			}else{
				core.BaseClock= core.CoreClock;
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
		if( group_id == 1 ){
			// AlderLake/RaptorLake
			group_id= 0;
			std::map<uint32_t,unsigned int>		clock_map;
			std::map<uint32_t,uint64_t>			mask_map;
			for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
				auto&	core= CoreList[ci];
				const auto&	clock= clock_map.find( core.BaseClock );
				if( clock == clock_map.end() ){
					clock_map[ core.BaseClock ]= group_id++;
					mask_map[ core.BaseClock ]= 1uLL<<ci;
				}else{
					mask_map[ core.BaseClock ]|= 1uLL<<ci;
				}
			}
			for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
				auto&	core= CoreList[ci];
				const auto&	mask= mask_map.find( core.BaseClock );
				core.ClusterMask= mask->second;
				const auto&	clock= clock_map.find( core.BaseClock );
				group_map[ core.ClusterMask ]= clock->second;
			}
		}
		PhysicalCoreCount= thread_id;
		CoreGroupCount= group_id;
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			const auto&	thread= thread_map.find( core.ThreadMask );
			core.PhysicalCoreID= thread->second;
			const auto&	group= group_map.find( core.ClusterMask );
			core.CoreGroupID= group->second;
		}
	}
}


void SystemInfo::DecodeCpuTopology()
{
	unsigned int	present= GetCpuCount( "/sys/devices/system/cpu/present" );
	{
		unsigned int	online= GetCpuCount( "/sys/devices/system/cpu/online" );
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 || online >= present ){
			DecodeCpuTopologyImmediate();
			return;
		}
	}
#if FL_USE_POWERBOOST_LOOP
	auto	start_time= time::GetPerfCounterUS();
	thread::AtomicValue<unsigned int>	ExitFlag( 0 );
	ut::FixedArray<thread::ThreadInstance*>	ThreadArray( present );
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= thread::CreateThreadFunction( [&ExitFlag](){
					for(; ExitFlag.Get() == 0 ;){
						math::Matrix4	m1, m2, m3;
						m1.SetIdentity();
						m2.SetIdentity();
						m3.Mul( m1, m2 );
						thread::OS_Pause();
					}
				});
		ThreadArray[ti]= thread;
		thread->Start();
	}
	for(;;){
		if( time::GetPerfCounterUS() - start_time > 1000000 ){
			DecodeCpuTopologyImmediate();
			ExitFlag= 1;
			break;
		}
		unsigned int	online= GetCpuCount( "/sys/devices/system/cpu/online" );
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 || online >= present ){
			DecodeCpuTopologyImmediate();
			ExitFlag= 1;
			break;
		}
		thread::SleepThreadMS( 50 );
	}
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= ThreadArray[ti];
		thread->Join();
		FL_MEMORY::ZDelete( thread );
		ThreadArray[ti]= nullptr;
	}
#else
	DecodeCpuTopologyImmediate();
#endif
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
		if( !info_feature ){
			if(
#if FL_CPU_ARM6 || FL_CPU_ARM7 || FL_CPU_ARM64
				!strncmp( linebuffer, "Features", 8 )
#endif
#if FL_CPU_X64 || FL_CPU_X86
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
#if FL_CPU_ARM7 || FL_CPU_ARM64
						if( !strcmp( top, "neon" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "asimd" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "vfpv4" ) ){
							SetInstructionSet( CPUFeature::ARM_VFPV4 );
						}else if( !strcmp( top, "fphp" ) ){
							SetInstructionSet( CPUFeature::ARM_FPHP );
						}else if( !strcmp( top, "asimdhp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDHP );
						}else if( !strcmp( top, "simddp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDDP );
						}else if( !strcmp( top, "asimddp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDDP );
						}else if( !strcmp( top, "sve" ) ){
							SetInstructionSet( CPUFeature::ARM_SVE );
						}else if( !strcmp( top, "sve2" ) ){
							SetInstructionSet( CPUFeature::ARM_SVE2 );
						}else if( !strcmp( top, "crc32" ) ){
							SetInstructionSet( CPUFeature::ARM_CRC32 );
						}else if( !strcmp( top, "sha1" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA1 );
						}else if( !strcmp( top, "sha2" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA2 );
						}else if( !strcmp( top, "sha3" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA3 );
						}else if( !strcmp( top, "sha512" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA512 );
						}else if( !strcmp( top, "aes" ) ){
							SetInstructionSet( CPUFeature::ARM_AES );
						}else if( !strcmp( top, "i8mm" ) ){
							SetInstructionSet( CPUFeature::ARM_I8MM );
						}else if( !strcmp( top, "bf16" ) ){
							SetInstructionSet( CPUFeature::ARM_BF16 );
						}else if( !strcmp( top, "svei8mm" ) ){
							SetInstructionSet( CPUFeature::ARM_SVEI8MM );
						}else if( !strcmp( top, "svebf16" ) ){
							SetInstructionSet( CPUFeature::ARM_SVEBF16 );
						}else if( !strcmp( top, "sme" ) ){
							SetInstructionSet( CPUFeature::ARM_SME );
						}else if( !strcmp( top, "sme2" ) ){
							SetInstructionSet( CPUFeature::ARM_SME2 );
						}else if( !strcmp( top, "asimdfhm" ) ){
							SetInstructionSet( CPUFeature::ARM_FHM );
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
#if FL_CPU_ARM7 || FL_CPU_ARM64
			if( !strncmp( linebuffer, "Hardware", 8 ) ){
				char*	lp= SkipColon( linebuffer );
				lp= SkipSpace( lp );
				strcpy_s( DeviceName, DEVICE_NAME_BUFFER_SIZE-1, lp );
				info_devname= true;
			}
#endif
#if FL_CPU_X64 || FL_CPU_X86
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


void SystemInfo::DecodeCpuClock()
{
#if FL_CPU_X64 || FL_CPU_X86
	const int	MAX_LINE_BYTE= 1024 * 2;
	char	linebuffer[MAX_LINE_BYTE+8];
	FILE*	fp= fopen( "/proc/cpuinfo", "r" );
	if( !fp ){
		return;
	}
	int		cpu_index= 0;
	for(; fgets( linebuffer, MAX_LINE_BYTE, fp ) ;){
		if( !strncmp( linebuffer, "processor", 9 ) ){
			char*	lp= SkipColon( linebuffer );
			cpu_index= atoi( lp );
		}
		if( !strncmp( linebuffer, "cpu MHz", 7 ) ){
			char*	lp= SkipColon( linebuffer );
			unsigned int	cpu_clock= static_cast<unsigned int>( atof( lp ) * 1000.0 );	// MHz to KHz
			auto&	core= CoreList[cpu_index];
			if( core.CoreClock == 0 ){
				core.BaseClock= cpu_clock;
				core.CoreClock= cpu_clock;
			}
		}
	}
	fclose( fp );
#endif
}


void SystemInfo::DecodeVersion()
{
	if( IsWSL2() ){
		Net_HasIPV6= false;
	}
}


//-----------------------------------------------------------------------------
#endif  // Linux
//-----------------------------------------------------------------------------


//=============================================================================
// CPU
//=============================================================================



//-----------------------------------------------------------------------------
#if FL_CPU_X86 || FL_CPU_X64
//-----------------------------------------------------------------------------

enum : uint32_t {
	//-- id=1 ECX
	CPUID_1_ECX_FEATURE_SSE3		=	(1u<< 0),
	CPUID_1_ECX_FEATURE_MONITOR		=	(1u<< 3),
	CPUID_1_ECX_FEATURE_SSSE3		=	(1u<< 9),
	CPUID_1_ECX_FEATURE_FMA3		=	(1u<<12),
	CPUID_1_ECX_FEATURE_SSE41		=	(1u<<19),
	CPUID_1_ECX_FEATURE_SSE42		=	(1u<<20),
	CPUID_1_ECX_FEATURE_AES			=	(1u<<25),
	CPUID_1_ECX_FEATURE_AVX			=	(1u<<28),
	CPUID_1_ECX_FEATURE_F16C		=	(1u<<29),
	CPUID_1_ECX_FEATURE_RAND		=	(1u<<30),
	//-- id=1 EDX
	CPUID_1_EDX_FEATURE_MMX			=	(1u<<23),
	CPUID_1_EDX_FEATURE_SSE			=	(1u<<25),
	CPUID_1_EDX_FEATURE_SSE2		=	(1u<<26),
	CPUID_1_EDX_FEATURE_HT			=	(1u<<28),
	//-- id=7,0 EBX
	CPUID_7_EBX_FEATURE_AVX2		=	(1u<< 5),
	CPUID_7_EBX_FEATURE_AVX512F		=	(1u<<16),	// Foundation
	CPUID_7_EBX_FEATURE_AVX512DQ	=	(1u<<17),	// Doubleword and Quadword instructions
	CPUID_7_EBX_FEATURE_AVX512IFMA	=	(1u<<21),	// 
	CPUID_7_EBX_FEATURE_AVX512PF	=	(1u<<26),	//
	CPUID_7_EBX_FEATURE_AVX512ER	=	(1u<<27),	//
	CPUID_7_EBX_FEATURE_AVX512CD	=	(1u<<28),	//
	CPUID_7_EBX_FEATURE_SHA			=	(1u<<29),
	CPUID_7_EBX_FEATURE_AVX512BW	=	(1u<<30),	// Byte and Word instructions
	CPUID_7_EBX_FEATURE_AVX512VL	=	(1u<<31),	// Vector Length extensions
	//-- id=7,0 ECX
	CPUID_7_ECX_FEATURE_AVX512VBMI	=	(1u<< 1),	//
	CPUID_7_ECX_FEATURE_AVX512VBMI2	=	(1u<< 6),	//
	CPUID_7_ECX_FEATURE_GFNI		=	(1u<< 8),	//
	CPUID_7_ECX_FEATURE_VAES		=	(1u<< 9),	//
	CPUID_7_ECX_FEATURE_AVX512VNNI	=	(1u<<11),	// Neural Network Instructions
	CPUID_7_ECX_FEATURE_AVX512BITALG=	(1u<<12),	//
	CPUID_7_ECX_FEATURE_AVX512VPOPCNTDQ=(1u<<14),	//
	//-- id=7,0 EDX
	CPUID_7_EDX_FEATURE_AVX5124VNNIW=	(1u<< 2),	//
	CPUID_7_EDX_FEATURE_AVX5124FMAPS=	(1u<< 3),	//
	CPUID_7_EDX_FEATURE_AVX512VP2INTERSECT=	(1u<< 8),	//
	CPUID_7_EDX_FEATURE_AVX512FP16=		(1u<< 23),	// FP16
	//-- id=7,1 EAX
	CPUID_7_1_EAX_FEATURE_AVXVNNI=		(1u<< 4),	// AVX-VNNI
	CPUID_7_1_EAX_FEATURE_AVX512BFLOAT16=	(1u<< 5),	// BFloat16
	//-- id=0x80000001 ECX
	CPUID_80_ECX_FEATURE_SSE4A		=	(1u<< 6),
	CPUID_80_ECX_FEATURE_FMA4		=	(1u<<16),
	//-- id=0x80000001 EDX
	CPUID_80_EDX_FEATURE_X64		=	(1u<<29),	// Long Mode
	CPUID_80_EDX_FEATURE_3DNOWEXT	=	(1u<<30),
	CPUID_80_EDX_FEATURE_3DNOW		=	(1u<<31),
};

struct RegCPUID {
	unsigned int	EAX;
	unsigned int	EBX;
	unsigned int	ECX;
	unsigned int	EDX;
};

static void	GetCPUID( RegCPUID& reg, unsigned int index, unsigned int subindex= 0 )
{
#if FL_CC_VC
//	__cpuid( reinterpret_cast<int*>(&reg.EAX), index );
	__cpuidex( reinterpret_cast<int*>(&reg.EAX), index, subindex );
#endif
#if FL_CC_CLANG_GCC
	unsigned int	a= 0;
	unsigned int	b= 0;
	unsigned int	c= 0;
	unsigned int	d= 0;
	__cpuid_count( index, subindex, a, b, c, d );
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
	RegCPUID	Feature07_01;
	RegCPUID	Feature80;

	GetCPUID( Feature01, 1 );
	GetCPUID( Feature07, 7 );
	GetCPUID( Feature07_01, 7, 1 );
	GetCPUID( Feature80, 0x80000001 );

FL_LOG( "01: %08x %08x %08x %08x\n", Feature01.EAX, Feature01.EBX, Feature01.ECX, Feature01.EDX );
FL_LOG( "07: %08x %08x %08x %08x\n", Feature07.EAX, Feature07.EBX, Feature07.ECX, Feature07.EDX );
FL_LOG( "071 %08x %08x %08x %08x\n", Feature07_01.EAX, Feature07_01.EBX, Feature07_01.ECX, Feature07_01.EDX );
FL_LOG( "80: %08x %08x %08x %08x\n", Feature80.EAX, Feature80.EBX, Feature80.ECX, Feature80.EDX );

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

	if( Feature07.ECX & CPUID_7_ECX_FEATURE_AVX512VNNI ){
		SetInstructionSet( CPUFeature::IA_AVX512VNNI );
	}

	if( Feature07.EDX & CPUID_7_EDX_FEATURE_AVX512FP16 ){
		SetInstructionSet( CPUFeature::IA_AVX512FP16 );
	}

	if( Feature07_01.EAX & CPUID_7_1_EAX_FEATURE_AVXVNNI ){
		SetInstructionSet( CPUFeature::IA_AVXVNNI );
	}
	if( Feature07_01.EAX & CPUID_7_1_EAX_FEATURE_AVX512BFLOAT16 ){
		SetInstructionSet( CPUFeature::IA_AVX512BF16 );
	}

	if( Feature80.ECX & CPUID_80_ECX_FEATURE_SSE4A ){
		SetInstructionSet( CPUFeature::IA_SSE4A );
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
#if FL_CPU_ARM6 || FL_CPU_ARM7 || FL_CPU_ARM64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
#if FL_CPU_ARM64
	SetInstructionSet( CPUFeature::ARM_NEON );
	SetInstructionSet( CPUFeature::ARM_64 );
#endif
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_CPU_MIPS || FL_CPU_MIPS64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_CPU_WASM32 || FL_CPU_WASM64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------



//=============================================================================
// Common
//=============================================================================

SystemInfo::SystemInfo() :
			InstructionSet( 0 ),
			TotalThreadCount( 1 ),
			PhysicalCoreCount( 1 ),
			CoreGroupCount( 1 ),
			InstArch( CPUArch::CPU_UNKNOWN ),
			CompiledArch( CPUArch::CPU_UNKNOWN ),
			Initialized( false ),
			Net_HasIPV6( true )
{
	FL_ASSERT( (unsigned int)CPUFeature::FEATURE_MAX <= CPU_FEATURE_MAX );
	memory::MemClear( CoreList );
	memory::MemClear( DeviceName );
}


void	SystemInfo::Init()
{
	if( Initialized ){
		return;
	}
	Initialized= true;
	GetCPUSpecification();
#if FL_USE_CUSTOM_CLOCK
	CPUCoreInfo	core_info;
	memory::MemClear( core_info );
	//core_info.CoreClock= 2100000;
	//core_info.ClusterMask= 0x0000000f;
	SetCoreInfo( 0, 4, 0, 0, 2100000, 2 );
	TotalThreadCount=  4;
	PhysicalCoreCount= 2;
	CoreGroupCount= 1;
#elif FL_OS_WINDOWS
	DecodeCpuTopology();
#elif FL_OS_LINUX
	DecodeCpuInfo();
	DecodeCpuTopology();
	DecodeVersion();
	DecodeCpuClock();
#elif FL_OS_DARWIN
	DecodeCpuTopology();
#endif
#if FL_CPU_ARM64
	CompiledArch= CPUArch::CPU_ARM64;
#elif FL_CPU_ARM7
	CompiledArch= CPUArch::CPU_ARM7;
#elif FL_CPU_ARM5
	CompiledArch= CPUArch::CPU_ARM5;
#elif FL_CPU_ARM6
	CompiledArch= CPUArch::CPU_ARM6;
#elif FL_CPU_X86
	CompiledArch= CPUArch::CPU_X86;
#elif FL_CPU_X64
	CompiledArch= CPUArch::CPU_X64;
#elif FL_CPU_MIPS32
	CompiledArch= CPUArch::CPU_MIPS32;
#elif FL_CPU_MIPS64
	CompiledArch= CPUArch::CPU_MIPS64;
#elif FL_CPU_WASM32
	CompiledArch= CPUArch::CPU_WASM32;
#elif FL_CPU_WASM64
	CompiledArch= CPUArch::CPU_WASM64;
#endif
	FL_LOG( "InstructionSet= %08x\n", InstructionSet );
}


void SystemInfo::DumpCpuGroup() const
{
	FL_PRINT( "Total Thread Count  : %d\n", TotalThreadCount );
	FL_PRINT( "Physical Core Count : %d\n", PhysicalCoreCount );
	FL_PRINT( "Group Count         : %d\n", CoreGroupCount );
	FL_PRINT( "Device Name         : %s\n", DeviceName );
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		FL_PRINT( "%2d: P:%02d G:%02d %8dKHz G:%08llx T:%08llx\n",
				core.Index,
				core.PhysicalCoreID,
				core.CoreGroupID,
				core.CoreClock,
				core.ClusterMask,
				core.ThreadMask );
	}
}


void SystemInfo::SetCoreInfo( unsigned int cpu_id, unsigned int cpu_count, unsigned int group_id, unsigned int physical_id, unsigned int core_clock, unsigned int ht_step )
{
	// setup dummy info
	uint64_t	cluster_mask= ((1ULL<<cpu_count)-1) << cpu_id;
	uint64_t	thread_mask= ((1ULL<<ht_step)-1) << cpu_id;
	for( unsigned int ci= 0 ; ci < cpu_count ; ci++ ){
		auto&	core= CoreList[cpu_id + ci];
		core.ClusterMask= cluster_mask;
		auto		physical_index= physical_id + ci/ht_step;
		uint64_t	mask= thread_mask<< ((ci/ht_step) * ht_step);
		core.ThreadMask= mask;
		core.Index= cpu_id + ci;
		core.PhysicalCoreID= physical_index;
		core.CoreGroupID= group_id;
		core.CoreClock= core_clock;
	}
}


#define	DEF_CPUFEATURE_NAME_IA(name)	{	#name, CPUFeature::IA_##name	}
#define	DEF_CPUFEATURE_NAME_ARM(name)	{	#name, CPUFeature::ARM_##name	}
#define	DEF_CPUFEATURE_NAME_MIPS(name)	{	#name, CPUFeature::MIPS_##name	}
CPUFeatureNameTable	SystemInfo::FeatureNameTable[]= {
DEF_CPUFEATURE_NAME_IA( SSE ),
DEF_CPUFEATURE_NAME_IA( SSE2 ),
DEF_CPUFEATURE_NAME_IA( SSE3 ),
DEF_CPUFEATURE_NAME_IA( SSSE3 ),
DEF_CPUFEATURE_NAME_IA( SSE41 ),
DEF_CPUFEATURE_NAME_IA( SSE42 ),
DEF_CPUFEATURE_NAME_IA( SSE4A ),
DEF_CPUFEATURE_NAME_IA( AVX ),
DEF_CPUFEATURE_NAME_IA( AVX2 ),
DEF_CPUFEATURE_NAME_IA( AVX512F ),
DEF_CPUFEATURE_NAME_IA( AVX512BW ),
DEF_CPUFEATURE_NAME_IA( AVX512DQ ),
DEF_CPUFEATURE_NAME_IA( AVX512VL ),
DEF_CPUFEATURE_NAME_IA( AVX512VNNI ),
DEF_CPUFEATURE_NAME_IA( AVX512BF16 ),
DEF_CPUFEATURE_NAME_IA( AVX512FP16 ),
DEF_CPUFEATURE_NAME_IA( AES ),
DEF_CPUFEATURE_NAME_IA( FMA3 ),
DEF_CPUFEATURE_NAME_IA( FMA4 ),
DEF_CPUFEATURE_NAME_IA( SHA ),
DEF_CPUFEATURE_NAME_IA( F16C ),
DEF_CPUFEATURE_NAME_IA( X64 ),
DEF_CPUFEATURE_NAME_IA( AVXVNNI ),

DEF_CPUFEATURE_NAME_ARM( NEON ),
DEF_CPUFEATURE_NAME_ARM( VFPV4 ),
DEF_CPUFEATURE_NAME_ARM( FPHP ),
DEF_CPUFEATURE_NAME_ARM( SIMDHP ),
DEF_CPUFEATURE_NAME_ARM( SIMDDP ),
DEF_CPUFEATURE_NAME_ARM( SVE ),
DEF_CPUFEATURE_NAME_ARM( SVE2 ),
DEF_CPUFEATURE_NAME_ARM( CRC32 ),
DEF_CPUFEATURE_NAME_ARM( SHA1 ),
DEF_CPUFEATURE_NAME_ARM( SHA2 ),
DEF_CPUFEATURE_NAME_ARM( SHA3 ),
DEF_CPUFEATURE_NAME_ARM( SHA512 ),
DEF_CPUFEATURE_NAME_ARM( AES ),
DEF_CPUFEATURE_NAME_ARM( 64 ),
DEF_CPUFEATURE_NAME_ARM( I8MM ),
DEF_CPUFEATURE_NAME_ARM( BF16 ),
DEF_CPUFEATURE_NAME_ARM( SVEI8MM ),
DEF_CPUFEATURE_NAME_ARM( SVEBF16 ),
DEF_CPUFEATURE_NAME_ARM( SME ),
DEF_CPUFEATURE_NAME_ARM( SME2 ),
DEF_CPUFEATURE_NAME_ARM( FHM ),

DEF_CPUFEATURE_NAME_MIPS( MSA ),
DEF_CPUFEATURE_NAME_MIPS( F64 ),
DEF_CPUFEATURE_NAME_MIPS( PS ),
};

const char* SystemInfo::GetFeatureName( CPUFeature feature ) const
{
	FL_ASSERT( sizeof(FeatureNameTable)/sizeof(CPUFeatureNameTable) == (int)CPUFeature::FEATURE_MAX );
	if( feature < CPUFeature::FEATURE_MAX ){
		unsigned int	index= (unsigned int)feature;
		const auto&	name= FeatureNameTable[index];
		FL_ASSERT( name.Feature == feature );
		return	name.FeatureName;
	}
	return	"Unknown";
}

void SystemInfo::GetCpuFeatureString( char* buffer, size_t buffer_size ) const
{
	char*	str= buffer;
	char*	str_end= buffer + buffer_size;
	for( unsigned int fi= 0 ; str < str_end && fi< (unsigned int)CPUFeature::FEATURE_MAX ; fi++ ){
		if( HasInstructionSet( (CPUFeature)fi ) ){
			const char*	ptr= GetFeatureName( (CPUFeature)fi );
			strcpy_s( str, str_end - str, ptr );
			str+= strlen(ptr);
			*str++= ' ';
		}
	}
	*str= '\0';
}

void SystemInfo::DumpSystemInfo() const
{
	FL_ASSERT( Initialized != 0 );
	DumpCpuGroup();
	const int	CPU_FEATURE_BUFFER_SIZE= 512;
	char	buffer[CPU_FEATURE_BUFFER_SIZE];
	GetCpuFeatureString( buffer, CPU_FEATURE_BUFFER_SIZE );
	FL_PRINT( "%s\n", buffer );
}

unsigned int	SystemInfo::GetCoreClock( unsigned int group_index ) const
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.CoreGroupID == group_index ){
			return	core.CoreClock;
		}
	}
	return	0;
}

void	SystemInfo::SetCoreClock( unsigned int group_index, unsigned int clock_kh )
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		auto&	core= CoreList[ci];
		if( core.CoreGroupID == group_index ){
			core.CoreClock= clock_kh;
		}
	}
}

unsigned int	SystemInfo::GetThreadCount( unsigned int group_index ) const
{
	FL_ASSERT( Initialized != 0 );
	unsigned int	thread_count= 0;
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.CoreGroupID == group_index ){
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
		if( core.CoreGroupID == group_index ){
			return	core.ClusterMask;
		}
	}
	return	0;
}

bool	SystemInfo::HasInstructionSet( CPUFeature feature ) const
{
	FL_ASSERT( feature < CPUFeature::FEATURE_MAX );
	return	(InstructionSet& (1ull<<(unsigned int)feature)) != 0;
}

void	SystemInfo::SetInstructionSet( CPUFeature feature )
{
	FL_ASSERT( feature < CPUFeature::FEATURE_MAX );
	InstructionSet|= 1ull<<(unsigned int)feature;
}

const char*	SystemInfo::GetDeviceName() const
{
	return	DeviceName;
}

CPUArch	SystemInfo::GetInstArch() const
{
	return	InstArch;
}

CPUArch	SystemInfo::GetCompiledArch() const
{
	return	CompiledArch;
}

const char* SystemInfo::GetArchNameLong( CPUArch arch ) const
{
	switch( arch ){
	case CPUArch::CPU_ARM64:
		if( HasInstructionSet( CPUFeature::ARM_SVE2 ) || HasInstructionSet( CPUFeature::ARM_SME ) ){
			return	"ARMv9A AArch64";
		}
		if( HasInstructionSet( CPUFeature::ARM_FPHP ) ){
			return	"ARMv8.2A AArch64";
		}
		return	"ARMv8A AArch64";
	case CPUArch::CPU_ARM7:
		return	"ARMv7A";
	case CPUArch::CPU_ARM5:
		return	"ARMv5TE";
	case CPUArch::CPU_ARM6:
		return	"ARMv6";
	case CPUArch::CPU_X86:
		return	"x86";
	case CPUArch::CPU_X64:
		return	"x64 (x86_64)";
	case CPUArch::CPU_MIPS32:
		return	"MIPS32";
	case CPUArch::CPU_MIPS64:
		return	"MIPS64";
	default:
		break;
	}
	return	"Unknown";
}

const char* SystemInfo::GetArchNameShort( CPUArch arch ) const
{
	switch( arch ){
	case CPUArch::CPU_ARM64:
		if( HasInstructionSet( CPUFeature::ARM_SVE2 ) || HasInstructionSet( CPUFeature::ARM_SME ) ){
			return	"ARMv9A";
		}
		return	"ARMv8A";
	case CPUArch::CPU_ARM7:
		return	"ARMv7A";
	case CPUArch::CPU_ARM5:
		return	"ARMv5TE";
	case CPUArch::CPU_ARM6:
		return	"ARMv6";
	case CPUArch::CPU_X86:
		return	"x86";
	case CPUArch::CPU_X64:
		return	"x64";
	case CPUArch::CPU_MIPS32:
		return	"MIPS32";
	case CPUArch::CPU_MIPS64:
		return	"MIPS64";
	default:
		break;
	}
	return	"Unknown";
}

bool	SystemInfo::HasIPV6_() const
{
	FL_ASSERT( Initialized != 0 );
	return	Net_HasIPV6;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


