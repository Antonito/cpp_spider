#include "WebServer.h"

namespace spider
{
namespace http
{
WebServer::WebServer(server::CommandCenter const &cmdCenter) : AControl(cmdCenter)
{
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
}

void WebServer::processEvent()
{
}

bool WebServer::acceptClient()
{
    return false;
}

void WebServer::multiplex() {}
}
}