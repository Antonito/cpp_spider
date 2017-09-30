#include <chrono>
#include <thread>
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
    return (AControl::pollEvent(ev));
}

void WebServer::sendResponse(server::Event const &ev)
{
    AControl::sendResponse(ev);
}

void WebServer::sendEvent(server::Event &ev)
{
    AControl::sendEvent(ev);
}

void WebServer::run()
{
    nope::log::Log(Info) << "Starting WebServer";
    while (m_running)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2ms);
    }
    nope::log::Log(Info) << "Stopping WebServer";
}
}
}
