#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>
#include "PluginManager.h"

//#include "NFComm/NFCore/NFException.h"
//#include "NFComm/NFPluginModule/NFPlatform.h"
//#include "NFComm/NFLogPlugin/easylogging++.h"

#if EM_PLATFORM != EM_PLATFORM_WIN
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <execinfo.h>
#include <setjmp.h>

#if EM_PLATFORM == EM_PLATFORM_LINUX
#include <sys/prctl.h>
#endif//EM_PLATFORM == EM_PLATFORM_LINUX

#endif//EM_PLATFORM != EM_PLATFORM_WIN

using namespace EM;

bool bExitApp = false;
void MainExecute();

#if EM_PLATFORM == EM_PLATFORM_WIN
#pragma comment( lib, "DbgHelp" )
bool ApplicationCtrlHandler(DWORD fdwctrltype);
void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException);
long ApplicationCrashHandler(EXCEPTION_POINTERS* pException);
#endif

int main()
{
	PluginManager::Instance()->LoadPlugin();
	PluginManager::Instance()->Awake();
	PluginManager::Instance()->Init();
	PluginManager::Instance()->AfterInit();
	PluginManager::Instance()->CheckConfig();
	PluginManager::Instance()->ReadyExecute();
	MainExecute();
	PluginManager::Instance()->BeforeShut();
	PluginManager::Instance()->Shut();
	PluginManager::Instance()->Finalize();
	PluginManager::Instance()->Release();
    return 0;
}

void MainExecute()
{
	uint64_t nIndex = 0;
	while (!bExitApp)
	{
		nIndex++;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

#if EM_PLATFORM == EM_PLATFORM_WIN
		__try
#else
		try
#endif
		{
#ifdef EM_COROUTINE
			PluginManager::Instance()->ExecuteCoScheduler();
#else
			PluginManager::Instance()->Execute();
#endif
		}
#if EM_PLATFORM == EM_PLATFORM_WIN
		__except (ApplicationCrashHandler(GetExceptionInformation()))
		{

		}
#else
		catch (const std::exception& e)
		{
			//NFException::StackTrace(11);
		}
		catch (...)
		{
			//NFException::StackTrace(11);
		}
#endif
	}
}

#if EM_PLATFORM == EM_PLATFORM_WIN
bool ApplicationCtrlHandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	{
		bExitApp = true;
	}
	return true;
	default:
		return false;
	}
}

void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException)
{
	//Dump
	HANDLE hDumpFile = CreateFile(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	time_t t = time(0);
	char szDmupName[MAX_PATH];
	tm* ptm = localtime(&t);

	sprintf_s(szDmupName, "%04d_%02d_%02d_%02d_%02d_%02d.dmp", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	CreateDumpFile(szDmupName, pException);

	FatalAppExit(-1, szDmupName);

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif