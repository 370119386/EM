#ifndef EM_PLATFORM_H
#define EM_PLATFORM_H

#define EM_PLATFORM_WIN 1
#define EM_PLATFORM_LINUX 2
#define EM_PLATFORM_APPLE 3
#define EM_PLATFORM_SYMBIAN 4
#define EM_PLATFORM_APPLE_IOS 5
#define EM_PLATFORM_ANDROID 6

#define EM_COMPILER_MSVC 1
#define EM_COMPILER_GNUC 2
#define EM_COMPILER_BORL 3
#define EM_COMPILER_WINSCW 4
#define EM_COMPILER_GCCE 5

#define EM_ENDIAN_LITTLE 1
#define EM_ENDIAN_BIG 2

//#define EM_ENABLE_SSL 1

#if EM_ENABLE_SSL
#define EVENT__HAVE_OPENSSL
#endif

#define EM_ARCHITECTURE_32 1
#define EM_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if defined( __GCCE__ )
#   define EM_COMPILER EM_COMPILER_GCCE
#   define EM_COMP_VER _MSC_VER
//# include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE
#elif defined( __WINSCW__ )
#   define EM_COMPILER EM_COMPILER_WINSCW
#   define EM_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define EM_COMPILER EM_COMPILER_MSVC
#   define EM_COMP_VER _MSC_VER
#elif defined( __GNUC__ )
#   define EM_COMPILER EM_COMPILER_GNUC
#   define EM_COMP_VER (((__GNUC__)*100) + \
                        (__GNUC_MINOR__*10) + \
                        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define EM_COMPILER EM_COMPILER_BORL
#   define EM_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if EM_COMPILER == EM_COMPILER_MSVC
#   if EM_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* Finds the current platform */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
#   define EM_PLATFORM EM_PLATFORM_WIN
//////////////////////////////////////////////////////////////////////////
#elif defined( __APPLE_CC__) || defined(__APPLE__) || defined(__OSX__)
// Device                                                     Simulator
// Both requiring OS version 4.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define EM_PLATFORM EM_PLATFORM_APPLE_IOS
#   else
#       define EM_PLATFORM EM_PLATFORM_APPLE
#   endif
//////////////////////////////////////////////////////////////////////////
#elif defined(linux) && defined(__arm__)
// TODO: This is NOT the correct way to detect the Tegra 2 platform but it works for now.
// It doesn't appear that GCC defines any platform specific macros.
#   define EM_PLATFORM EM_PLATFORM_TEGRA2
#elif defined(__ANDROID__)
#   define EM_PLATFORM EM_PLATFORM_ANDROID
//////////////////////////////////////////////////////////////////////////
#elif defined( __native_client__ )
#   define EM_PLATFORM EM_PLATFORM_NACL
#   ifndef EM_STATIC_LIB
#       error EM must be built as static for NaCl (EM_STATIC=true in cmake)
#   endif
#   ifdef EM_BUILD_RENDERSYSTEM_D3D9
#       error d3d9 is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_D3D9 false in cmake)
#   endif
#   ifdef EM_BUILD_RENDERSYSTEM_GL
#       error gl is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_GL=false in cmake)
#   endif
#   ifndef EM_BUILD_RENDERSYSTEM_GLES2
#       error GLES2 render system is needed for NaCl (ONF_BUILD_RENDERSYSTEM_GLES2=false in cmake)
#   endif
#else
#   define EM_PLATFORM EM_PLATFORM_LINUX
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define EM_ARCH_TYPE EM_ARCHITECTURE_64
#else
#   define EM_ARCH_TYPE EM_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define EM_QUOTE_INPLACE(x) # x
#define EM_QUOTE(x) EM_QUOTE_INPLACE(x)
#define EM_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if EM_PLATFORM == EM_PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#pragma warning(disable:4091) 
#include <Windows.h>
#define EM_EXPORT extern "C"  __declspec(dllexport)

#include <Dbghelp.h>
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#   if defined( EM_STATIC_LIB )
// Linux compilers don't have symbol import/export directives.
#       define _EMExport
#       define _EMPrivate
#   else
#       if defined( EM_NONCLIENT_BUILD )
#           define _EMExport __declspec( dllexport )
#       else
#           if defined( __MINGW32__ )
#               define _EMExport
#           else
#               define _EMExport __declspec( dllimport )
#           endif
#       endif
#       define _EMPrivate
#   endif
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define EM_DEBUG_MODE 1
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if EM_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || EM_COMP_VER > 345
#    define EM_UNICODE_SUPPORT 1
#   else
#    define EM_UNICODE_SUPPORT 0
#   endif
#  else
#   define EM_UNICODE_SUPPORT 1
#  endif
# else
#  define EM_UNICODE_SUPPORT 1
# endif
#else
#  define EM_UNICODE_SUPPORT 1
#endif

#endif // EM_PLATFORM == EM_PLATFORM_WIN
//----------------------------------------------------------------------------
// Android Settings
/*
#if NF_PLATFORM == NF_PLATFORM_ANDROID
#   define _NFExport
#   define NF_UNICODE_SUPPORT 1
#   define NF_DEBUG_MODE 0
#   define _NFPrivate
#     define CLOCKS_PER_SEC  1000
//  pragma def were found here: http://www.inf.pucrs.br/~eduardob/disciplinas/SistEmbarcados/Mobile/Nokia/Tools/Carbide_vs/WINSCW/Help/PDF/C_Compilers_Reference_3.2.pdf
#     pragma warn_unusedarg off
#     pragma warn_emptydecl off
#     pragma warn_possunwant off
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define NF_DEBUG_MODE 1
#   else
#       define NF_DEBUG_MODE 0
#   endif
#endif
*/
//----------------------------------------------------------------------------
// Linux/Apple/iOs/Android/Symbian/Tegra2/NaCl Settings
#if EM_PLATFORM == EM_PLATFORM_LINUX || EM_PLATFORM == EM_PLATFORM_APPLE || EM_PLATFORM == EM_PLATFORM_APPLE_IOS || EM_PLATFORM == EM_PLATFORM_ANDROID || EM_PLATFORM == EM_PLATFORM_TEGRA2 || EM_PLATFORM == EM_PLATFORM_NACL

//#include <syswait.h>

// Enable GCC symbol visibility
#   if defined( EM_GCC_VISIBILITY )
#       define _EMExport  __attribute__ ((visibility("default")))
#       define _EMPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _EMExport
#       define _EMPrivate
#   endif

// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   if defined(_DEBUG) || defined(DEBUG)
#       define EM_DEBUG_MODE 1
#   endif

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define EM_UNICODE_SUPPORT 1
#define MAX_PATH 255

#define EM_EXPORT extern "C" __attribute ((visibility("default")))

#endif


//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set NF_ENDIAN correctly
#ifdef EM_CONFIG_BIG_ENDIAN
#    define EM_ENDIAN EM_ENDIAN_BIG
#else
#    define EM_ENDIAN EM_ENDIAN_LITTLE
#endif

#include <stdint.h>
#include <chrono>
#include <functional>
#include <assert.h>
#include <string>
#include <map>
#include <iostream>

// Integer formats of fixed bit width
typedef uint32_t NFUINT32;
typedef uint16_t NFUINT16;
typedef uint8_t NFUINT8;
typedef int32_t NFINT32;
typedef int16_t NFINT16;
typedef int8_t NFINT8;
typedef uint64_t NFUINT64;
typedef int64_t NFINT64;
typedef int64_t NFSOCK;

#if EM_PLATFORM == EM_PLATFORM_WIN
#include <crtdbg.h>
#define EMASSERT(exp_, msg_, file_, func_)  \
    std::string strInfo("Message:");        \
    strInfo += msg_ + std::string(" don't exist or some warning") + std::string("\n\nFile:") + std::string(file_) + std::string("\n Function:") + func_; \
    MessageBoxA(0, strInfo.c_str(), ("Error_"#exp_), MB_RETRYCANCEL | MB_ICONERROR); \
    assert(0);
#else
#define EMASSERT(exp_, msg_, file_, func_)
#endif


//#define GOOGLE_GLOG_DLL_DECL=

///////////////////////////////////////////////////////////////
#include <string>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <sstream>
#include <stdio.h>
//#include <common/lexical_cast.hpp>

#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#define EPOCHFILETIME 11644473600000000ULL
#else
#include <WinSock2.h>
#include <windows.h>
#include <time.h>
#include <process.h>
#define EPOCHFILETIME 11644473600000000Ui64
#endif

#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING

#if EM_PLATFORM == EM_PLATFORM_WIN
#define EMSPRINTF sprintf_s
#define EMSTRICMP stricmp
#define EMSLEEP(s) Sleep(s) //millisecond
#define EMGetPID() lexical_cast<std::string>(getpid())
#else
#define EMSPRINTF snprintf
#define EMSTRICMP strcasecmp
#define EMSLEEP(s) usleep(s*1000) //millisecond
#define EMGetPID() lexical_cast<std::string>(getpid())
#endif

//#if EM_PLATFORM == EM_PLATFORM_WIN
#ifndef EM_DYNAMIC_PLUGIN
#define EM_DYNAMIC_PLUGIN 1
#endif
//#endif

#if EM_PLATFORM != EM_PLATFORM_WIN
#ifndef EM_USE_COROUTINE
//#define EM_USE_COROUTINE 1
#endif
#endif

//using tcmalloc
//#define EM_USE_TCMALLOC 1

//using actor mode--begin
#define EM_ACTOR_THREAD_COUNT 2
#define EM_USE_ACTOR

//use actor mode--end

#define GET_CLASS_NAME(className) (#className)

#define EM_SHARE_PTR std::shared_ptr
#define EM_NEW new

//�����ڴ�й¶���
#define EM_MEM_LEACK_CHECK		(1)

/*
template<typename DTYPE>
bool EM_StrTo(const std::string& strValue, DTYPE& nValue)
{
	try
	{
		nValue = lexical_cast<DTYPE>(strValue);
		return true;
	}
	catch (...)
	{
		return false;
	}

	return false;
}*/

inline bool IsZeroFloat(const float fValue, float epsilon = 1e-6)
{
	return std::abs(fValue) <= epsilon;
}

inline bool IsZeroDouble(const double dValue, double epsilon = 1e-15)
{
	return std::abs(dValue) <= epsilon;
}

inline int64_t EMGetTimeMSEx()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//millisecond
inline int64_t EMGetTimeMS()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//second
inline int64_t EMGetTimeS()
{
	return EMGetTimeMS() / 1000;
}

//Protobuf Using Dlls
/*
#if EM_PLATFORM == EM_PLATFORM_WIN
#ifndef PROTOBUF_USE_DLLS
#define PROTOBUF_USE_DLLS
#endif
#endif
*/

#endif//EM_PLATFORM_H