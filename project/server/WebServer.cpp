#include "WebServer.h"
#include "Logger.hpp"

namespace spider
{
namespace http
{
WebServer::WebServer(server::CommandCenter const &cmdCenter, volatile bool const &running) : AControl(cmdCenter, running)
{
    nope::log::Log(Info) << "Creating WebServer";
}

bool WebServer::pollEvent(server::Event &ev)
{
    return false;
}

void WebServer::sendResponse(server::Event const &ev)
{
}

void WebServer::sendEvent(server::Event &ev)
{
}

void WebServer::run()
{
    nope::log::Log(Info) << "Starting WebServer";
    while (m_running)
    {
    }
    nope::log::Log(Info) << "Stopping WebServer";
}
}
}
