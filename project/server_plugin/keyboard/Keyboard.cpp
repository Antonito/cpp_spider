#if defined _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include "Keyboard.h"
#include "IClient.h"

Keyboard::Keyboard()
    : m_name("keyboard"), m_description("Toggle keyboard monitoring")
{
}

std::string const &Keyboard::getName() const
{
  return m_name;
}

std::string const &Keyboard::getDescription() const
{
  return m_description;
}

void Keyboard::command(spider::server::IClient *cli, void const *)
{
  static std::string const cmd = "/setKeyboardEmission\r\n";
  cli->send(cmd);
}

static Keyboard plugin;

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
