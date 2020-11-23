// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_PLATFORM_H_
#define FLATLIB_PLATFORM_H_


//-----------------------------------------------------------------------------
// MacOSX
//-----------------------------------------------------------------------------
#if defined(FL_PRESET_OSX) && FL_PRESET_OSX
# if defined(__i386__)
#  define	FL_CPU_X86			1
# elif defined(__x86_64__)
#  define	FL_CPU_X64			1
# elif defined(__arm64__)
#  define	FL_CPU_ARM64		1
# else
#  error "macos"
# endif
# define	FL_OS_UNIX			1
# define	FL_OS_DARWIN			1
# define	FL_OS_OSX			1
# define	FL_CC_CLANG			1
# define	FL_SYSTEMTYPE_DEFINED
#endif




//-----------------------------------------------------------------------------
// iOS
//-----------------------------------------------------------------------------
#if defined(FL_PRESET_IOS) && FL_PRESET_IOS
# if defined(__i386__)
#  define	FL_CPU_X86			1
# elif defined(__x86_64__)
#  define	FL_CPU_X64			1
# elif defined(__arm64__)
#  define	FL_CPU_ARM64			1
#  define	FL_CPU_NEON			1
# elif __ARM_ARCH == 7
#  define	FL_CPU_ARM7			1
#  define	FL_CPU_NEON			1
# else
#  define	FL_CPU_ARM6			1
#  define	FL_CPU_NEON			0
# endif
# define	FL_OS_UNIX			1
# define	FL_OS_DARWIN			1
# define	FL_OS_IOS			1
# define	FL_CC_CLANG			1
# ifdef __ARM_VFPV4__
#  define	FL_CPU_VFPV4			1
# else
#  define	FL_CPU_VFPV4			0
# endif
# define	FL_SYSTEMTYPE_DEFINED
#endif





//-----------------------------------------------------------------------------
// Linux
//-----------------------------------------------------------------------------
#if defined(FL_PRESET_PPO) && FL_PRESET_PPO
# define	FL_PRESET_LINUX	1
# define	FL_OS_PPO		1
#endif

#if defined(FL_PRESET_LINUX) && FL_PRESET_LINUX
# if defined(__x86_64__)
#  define	FL_CPU_X64			1
# elif defined(__aarch64__)
#  define	FL_CPU_ARM64			1
#  define	FL_CPU_NEON			1
# elif defined(__i386__)
#  define	FL_CPU_X86			1
# elif defined(__ARM_ARCH_6__)
#  define	FL_CPU_ARM6			1
# elif defined(__arm__)
#  define	FL_CPU_ARM7			1
#  define	FL_CPU_NEON			1
# endif
# define	FL_OS_UNIX			1
# define	FL_OS_LINUX			1
# ifdef __clang__
#  define	FL_CC_CLANG			1
# else
#  define	FL_CC_GCC			1
# endif
# ifdef __ARM_FEATURE_FMA
#  define	FL_CPU_VFPV4			1 // nexus7=0, raspberry pi 2=1
# endif
# define	FL_SYSTEMTYPE_DEFINED
#endif



//-----------------------------------------------------------------------------
// Android
//-----------------------------------------------------------------------------
#if defined(FL_PRESET_ANDROID) && FL_PRESET_ANDROID
# if defined(__x86_64__)
#  define	FL_CPU_X64			1
# elif defined(__AARCH64EL__)
#  define	FL_CPU_ARM64			1
# elif defined(__mips64)
#  define	FL_CPU_MIPS64		1
# elif defined(__i386__)
#  define	FL_CPU_X86			1
# elif defined(__mips__)
#  define	FL_CPU_MIPS32		1
# elif defined(__arm__)
#  if defined(FL_CPU_ARM5) && FL_CPU_ARM5
#	define	FL_CPU_ARM5			1
#  else
#   define	FL_CPU_ARM7			1
#  endif
# endif
# define	FL_OS_UNIX			1
# define	FL_OS_ANDROID		1
# ifdef __clang__
#  define	FL_CC_CLANG			1
# else
#  define	FL_CC_GCC			1
# endif
# define	FL_SYSTEMTYPE_DEFINED
#endif


#ifdef __ARM_FEATURE_FP16_SCALAR_ARITHMETIC
# define	FL_CPU_FP16			1
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	FL_CPU_ARM	(FL_CPU_ARM5 || FL_CPU_ARM6 || FL_CPU_ARM7 || FL_CPU_ARM64)


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#ifndef	FL_SYSTEMTYPE_DEFINED
# error "FL_PRESET_* not defined"
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// CPU
#ifndef FL_CPU_X86
# define	FL_CPU_X86			0
#endif
#ifndef	FL_CPU_X64
# define	FL_CPU_X64			0
#endif
#ifndef	FL_CPU_ARM5
# define	FL_CPU_ARM5			0
#endif
#ifndef	FL_CPU_ARM6
# define	FL_CPU_ARM6			0
#endif
#ifndef	FL_CPU_ARM7
# define	FL_CPU_ARM7			0
#endif
#ifndef	FL_CPU_ARM64
# define	FL_CPU_ARM64			0
#endif
#ifndef	FL_CPU_MIPS32
# define	FL_CPU_MIPS32		0
#endif
#ifndef	FL_CPU_MIPS64
# define	FL_CPU_MIPS64		0
#endif
#ifndef	FL_CPU_PPC64
# define	FL_CPU_PPC64			0
#endif


#ifndef	FL_CPU_NEON
# define	FL_CPU_NEON			0
#endif
#ifndef	FL_CPU_VFPV4
# define	FL_CPU_VFPV4			0
#endif
#ifndef FL_CPU_FP16
# define	FL_CPU_FP16			0
#endif



// OS
#ifndef	FL_OS_WINDOWS
# define	FL_OS_WINDOWS		0
#endif
#ifndef	FL_OS_UNIX
# define	FL_OS_UNIX			0
#endif
#ifndef	FL_OS_DARWIN
# define	FL_OS_DARWIN			0
#endif

#ifndef	FL_OS_LINUX
# define	FL_OS_LINUX			0
#endif
#ifndef	FL_OS_ANDROID
# define	FL_OS_ANDROID		0
#endif
#ifndef	FL_OS_OSX
# define	FL_OS_OSX			0
#endif
#ifndef	FL_OS_IOS
# define	FL_OS_IOS			0
#endif
#ifndef	FL_OS_PPO
# define	FL_OS_PPO			0
#endif




// COMPILER
#ifndef	FL_CC_VC
# define	FL_CC_VC				0
#endif
#ifndef	FL_CC_GCC
# define	FL_CC_GCC			0
#endif
#ifndef	FL_CC_CLANG
# define	FL_CC_CLANG			0
#endif




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef	FL_DEBUG
# if defined(NDEBUG)
#  define	FL_DEBUG			0
# elif defined(_DEBUG) || defined(DEBUG)
#  define	FL_DEBUG			1
# else
#  define	FL_DEBUG			1
# endif
#endif
#ifndef	FL_RETAIL
# define	FL_RETAIL		0
#endif




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif

