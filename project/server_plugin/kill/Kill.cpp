#if defined _WIN32
#include <windows.h>
#endif

#include <iostream>
#include "Kill.h"
#include "IClient.h"

Kill::Kill() : m_name("kill"), m_description("Stop the execution of the virus")
{
}

std::string const &Kill::getName() const
{
    return m_name;
}

std::string const &Kill::getDescription() const
{
    return m_description;
}

void Kill::command(spider::server::IClient *cli, void const *)
{
    static std::string const cmd = "/kill\r\n";
    cli->send(cmd);
}

static Kill plugin;

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