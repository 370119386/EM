#include "Common/Platform.h"

#ifdef EM_DEBUG_MODE

#if EM_PLATFORM == EM_PLATFORM_WIN
//#pragma comment( lib, "EMCore_d.lib" )
//#pragma comment( lib, "EMMessageDefine_d.lib" )
//#pragma comment( lib, "libprotobuf_d.lib" )
#elif EM_PLATFORM == EM_PLATFORM_LINUX || EM_PLATFORM == EM_PLATFORM_ANDROID
//#pragma comment( lib, "EMCore_d.a" )
//#pragma comment( lib, "EMMessageDefine_d.a" )
//#pragma comment( lib, "libprotobuf_d.a" )
#elif EM_PLATFORM == EM_PLATFORM_APPLE || EM_PLATFORM == EM_PLATFORM_APPLE_IOS
#endif

#else

#if EM_PLATFORM == EM_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
//#pragma comment( lib, "EMCore.lib" )
//#pragma comment( lib, "EMMessageDefine.lib" )
//#pragma comment( lib, "libprotobuf.lib" )
#elif EM_PLATFORM == EM_PLATFORM_LINUX || EM_PLATFORM == EM_PLATFORM_ANDROID
//#pragma comment( lib, "EMCore.a" )
//#pragma comment( lib, "EMMessageDefine.a" )
#elif EM_PLATFORM == EM_PLATFORM_APPLE || EM_PLATFORM == EM_PLATFORM_APPLE_IOS
#endif

#endif
