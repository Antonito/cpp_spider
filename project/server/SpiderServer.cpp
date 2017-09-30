#include "SpiderServer.h"
#include "Logger.hpp"

namespace spider
{
namespace server
{
SpiderServer::SpiderServer(CommandCenter &cmdCenter, volatile bool const &running) : m_controllers(), m_clients(), m_cmdCenter(cmdCenter), m_running(running)
{
}

void SpiderServer::addController(AControl &controller)
{
    m_controllers.push_back(&controller);
}

void SpiderServer::run()
{
    nope::log::Log(Info) << "Starting Spider server";
    while (m_running)
    {
        multiplex();
        processEvent();
    }
    nope::log::Log(Info) << "Stopping Spider server";
}

void SpiderServer::processEvent()
{
}

bool SpiderServer::acceptClient()
{
    return true;
}

void SpiderServer::multiplex()
{
}
}
}