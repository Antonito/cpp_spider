#if defined _WIN32
#include <windows.h>
#endif

#include <iostream>
#include "Mouse.h"
#include "IClient.h"

Mouse::Mouse() : m_name("mouse"), m_description("Toggle mouse monitoring")
{
}

std::string const &Mouse::getName() const
{
    return m_name;
}

std::string const &Mouse::getDescription() const
{
    return m_description;
}

void Mouse::command(spider::server::IClient *cli, void const *)
{
    static std::string const cmd = "/setMouseEmission\r\n";
    cli->send(cmd);
}

static Mouse plugin;

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