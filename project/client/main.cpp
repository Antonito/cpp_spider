#include <cstdlib>
#include "Logger.hpp"
#include "Core.h"

int main()
{
    try
    {
        nope::log::Logger::start("/tmp/.debug.log"); // TODO: rm
        nope::log::Logger::logLevel = nope::log::LogLevel::LOG_DEBUG;

        nope::log::Log(Info) << "Logger started";
    }
    catch (std::exception const &e)
    {
        std::cerr << "Fatal error while initializing the logger: " << e.what()
                  << std::endl;
        return (EXIT_FAILURE);
    }

    spider::client::core::Core core("/tmp/spider/");

    return EXIT_SUCCESS;
}
