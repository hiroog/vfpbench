// flatlib 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4:

#pragma once

/*

	CONFIG
		FL_DEBUG
		FL_RETAIL

	OS
		FL_OS_WINDOWS
		FL_OS_LINUX
		FL_OS_MACOS
		FL_OS_ANDROID
		FL_OS_IOS
		FL_OS_TVOS
		FL_OS_WATCHOS
		FL_OS_EMSCRIPTEN
		FL_OS_WASI
		FL_OS_CS
		FL_OS_CS2
		FL_OS_CS3

		FL_OS_DARWIN	(FL_OS_MACOS|FL_OS_IOS|FL_OS_TVOS|FL_OS_WATCHOS)
		FL_OS_UNIX		(!FL_OS_WINDOWS)

	CPU
		FL_CPU_X64
		FL_CPU_X86
		FL_CPU_ARM64
		FL_CPU_ARM7
		FL_CPU_MIPS64
		FL_CPU_MIPS32
		FL_CPU_WASM32
		FL_CPU_WASM64

		FL_CPU_PTR64

	COMPILER
		FL_CC_VC
		FL_CC_CLANG
		FL_CC_GCC
		FL_CC_CLANG_GCC		(FL_CC_CLANG|FL_CC_GCC)

	FLB
		FLB_TARGET_LINUX
		FLB_TARGET_CS
		FLB_TARGET_CS2
		FLB_TARGET_CS3
*/


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Unix

#if defined(__ANDROID__)
# if !defined(FLB_TARGET_LINUX) // Termux
#  define	FL_OS_ANDROID	1
#  define	FL_OS_UNIX		1
# endif
#endif

#if defined(__linux__)
# define	FL_OS_LINUX		1
# define	FL_OS_UNIX		1
#endif

#if defined(__EMSCRIPTEN__)
# define	FL_OS_EMSCRIPTEN	1
# define	FL_OS_UNIX		1
#endif

#if defined(__wasi__)
# define	FL_OS_WASI		1
# define	FL_OS_UNIX		1
#endif

#if defined(__MACH__)
# if 0 //defined(FLB_TARGET_LINUX) // Linux on mac
#  define	FL_OS_LINUX		1
#  define	FL_OS_UNIX		1
# else
#  if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#   define	FL_OS_MACOS		1
#  elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#   define	FL_OS_IOS		1
#  elif defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#   define	FL_OS_TVOS		1
#  elif defined(__ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__)
#   define	FL_OS_WATCHOS	1
#  endif
#  define	FL_OS_UNIX		1
#  define	FL_OS_DARWIN	1
# endif
#endif

#if defined(FLB_TARGET_CS)
# include	<flatlib/core/system/Platform_CS.h>
#endif
#if defined(FLB_TARGET_CS2)
# include	<flatlib/core/system/Platform_CS2.h>
#endif
#if defined(FLB_TARGET_CS3)
# include	<flatlib/core/system/Platform_CS3.h>
#endif


#if defined(__clang__)
# define	FL_CC_CLANG		1
# define	FL_CC_CLANG_GCC	1
#elif defined(__GNUC__)
# define	FL_CC_GCC		1
# define	FL_CC_CLANG_GCC	1
#endif

#if defined(FL_CC_CLANG_GCC) && FL_CC_CLANG_GCC
# if defined(__x86_64__)
#  define	FL_CPU_X64		1
#  define	FL_CPU_PTR64	1
# elif defined(__i386__)
#  define	FL_CPU_X86		1
# elif defined(__aarch64__)
#  define	FL_CPU_ARM64	1
#  define	FL_CPU_PTR64	1
# elif defined(__mips64__)
#  define	FL_CPU_MIPS64	1
#  define	FL_CPU_PTR64	1
# elif defined(__arm__)
#  define	FL_CPU_ARM7		1
# elif defined(__mips__)
#  define	FL_CPU_MIPS32	1
# elif defined(__wasm32__)
#  define	FL_CPU_WASM32	1
# elif defined(__wasm64__)
#  define	FL_CPU_WASM64	1
# else
#  define	FL_CPU_X86		1
# endif
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Windows

#if defined(_WIN32)
# define	FL_OS_WINDOWS	1
#endif


#if defined(_MSC_VER)
# define	FL_CC_VC		1

# if defined(_M_X64)
#  define	FL_CPU_X64		1
#  define	FL_CPU_PTR64	1
# elif defined(_M_IX86)
#  define	FL_CPU_X86		1
# elif defined(_M_ARM64)
#  define	FL_CPU_ARM64	1
#  define	FL_CPU_PTR64	1
# elif defined(_M_ARM)
#  define	FL_CPU_ARM7		1
# endif
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-------------------------------------
// OS
//-------------------------------------
#ifndef	FL_OS_WINDOWS
# define	FL_OS_WINDOWS	0
#endif

#ifndef	FL_OS_LINUX
# define	FL_OS_LINUX		0
#endif

#ifndef	FL_OS_ANDROID
# define	FL_OS_ANDROID	0
#endif

#ifndef	FL_OS_MACOS
# define	FL_OS_MACOS		0
#endif

#ifndef	FL_OS_IOS
# define	FL_OS_IOS		0
#endif

#ifndef	FL_OS_TVOS
# define	FL_OS_TVOS		0
#endif

#ifndef	FL_OS_WATCHOS
# define	FL_OS_WATCHOS	0
#endif

#ifndef	FL_OS_UNIX
# define	FL_OS_UNIX		0
#endif

#ifndef	FL_OS_DARWIN
# define	FL_OS_DARWIN	0
#endif

#ifndef	FL_OS_EMSCRIPTEN
# define	FL_OS_EMSCRIPTEN	0
#endif

#ifndef	FL_OS_WASI
# define	FL_OS_WASI		0
#endif

#ifndef	FL_OS_CS
# define	FL_OS_CS		0
#endif

#ifndef	FL_OS_CS2
# define	FL_OS_CS2		0
#endif

#ifndef	FL_OS_CS3
# define	FL_OS_CS3		0
#endif



//-------------------------------------
// CPU
//-------------------------------------

#ifndef	FL_CPU_X64
# define	FL_CPU_X64		0
#endif

#ifndef	FL_CPU_X86
# define	FL_CPU_X86		0
#endif

#ifndef	FL_CPU_ARM64
# define	FL_CPU_ARM64	0
#endif

#ifndef	FL_CPU_ARM7
# define	FL_CPU_ARM7		0
#endif

#ifndef	FL_CPU_MIPS64
# define	FL_CPU_MIPS64	0
#endif

#ifndef	FL_CPU_MIPS32
# define	FL_CPU_MIPS32	0
#endif

#ifndef	FL_CPU_WASM64
# define	FL_CPU_WASM64	0
#endif

#ifndef	FL_CPU_WASM32
# define	FL_CPU_WASM32	0
#endif

#ifndef	FL_CPU_PTR64
# define	FL_CPU_PTR64	0
#endif



//-------------------------------------
// CC
//-------------------------------------
#ifndef	FL_CC_VC
# define	FL_CC_VC		0
#endif

#ifndef	FL_CC_CLANG
# define	FL_CC_CLANG		0
#endif

#ifndef	FL_CC_GCC
# define	FL_CC_GCC		0
#endif

#ifndef	FL_CC_CLANG_GCC
# define	FL_CC_CLANG_GCC	0
#endif



//-------------------------------------
// debug
//-------------------------------------
#ifndef	FL_DEBUG
# if defined(NDEBUG)
#  define	FL_DEBUG		0
# elif defined(_DEBUG) || defined(DEBUG)
#  define	FL_DEBUG		1
# else
#  define	FL_DEBUG		0
# endif
#endif


#ifndef	FL_RETAIL
# if defined(FLB_RETAIL) && FLB_RETAIL == 1
#  define	FL_RETAIL		1
# else
#  define	FL_RETAIL		0
# endif
#endif

#ifndef	FL_LOGGING
# define	FL_LOGGING		0@@@
#endif



