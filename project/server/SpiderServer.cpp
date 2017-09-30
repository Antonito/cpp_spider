#include "SpiderServer.h"

namespace spider
{
namespace server
{
SpiderServer::SpiderServer(CommandCenter &cmdCenter) : m_controllers(), m_clients(), m_cmdCenter(cmdCenter)
{
}

void SpiderServer::addController(AControl &controller)
{
    m_controllers.push_back(&controller);
}

void SpiderServer::run()
{
    // TODO: allow to stop server
    while (1)
    {
        multiplex();
        processEvent();
    }
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