#if defined _WIN32
#include <windows.h>
#endif

#include <iostream>
#include "GetInfo.h"
#include "IClient.h"

GetInfo::GetInfo() : m_name("GetInfo"), m_description("get Client's information like OS, IP geolocalisation, etc...")
{
}

std::string const &GetInfo::getName() const
{
  return m_name;
}

std::string const &GetInfo::getDescription() const
{
  return m_description;
}

void GetInfo::command(spider::server::IClient *cli, void const *)
{
  static std::string const cmd = "/getInfo\r\n";
  cli->send(cmd);
  //
  //on Client's side:
  //response("Windows 10; 165.4.67.84; FRANCE; DESKTOP-09876");
}

static GetInfo plugin;

// The entry point called by the server
extern "C" {
SPIDER_API spider::server::IPlugin *getPlugin()
{
  return static_cast<spider::server::IPlugin *>(&plugin);
}

#if defined _WIN32
BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
  return (true);
}
#endif
}