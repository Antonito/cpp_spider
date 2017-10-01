#include "Core.h"
#include "IPayload.h"

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
                                      m_lib(path + "foo.dylib"),
#elif defined __linux__
                                      m_lib(path + "foo.so"),
#else
#error "Plateform not supported"
#endif
                                      m_payload(nullptr)
{
    auto getPayload = m_lib.getFunction<library::IPayload *()>("getPayload");
    m_payload = getPayload();
    if (!m_payload)
    {
        throw std::runtime_error("Invalid payload");
    }
    if (!m_payload->init())
    {
        throw std::runtime_error("Cannot initialize payload");
    }
}

Core::~Core()
{
}
}
}
}