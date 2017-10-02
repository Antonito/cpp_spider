#pragma once

#include "IPayload.h"

namespace spider
{
namespace client
{
namespace library
{
class SpiderPlugin : public IPayload
{
  public:
    SpiderPlugin();
    virtual ~SpiderPlugin();

    // Should initialize the payload, if needed
    virtual bool init();

    // Should de-initialize the payload, if needed
    virtual bool deinit();

    virtual SystemInfos getInfos() const;

  private:
// SpiderPluginWindows.cpp
#if defined _WIN32
    bool initWindows();
    bool hideThreadWindows(void *handleThread); // Specific to Windows
    bool deinitWindows();
    SystemInfos getInfosWindows() const;
    Architecture detectArchWindows() const;
    std::uint64_t getRAMWindows() const;
// SpiderPluginOSX.cpp
#elif defined __APPLE__
    bool initOSX();
    bool deinitOSX();
    std::uint32_t getPageSize() const;
    std::uint64_t getRAMOSX() const;
    SystemInfos getInfosOSX() const;
// SpiderPluginLinux.cpp
#elif defined __linux__
    bool initLinux();
    bool deinitLinux();
    SystemInfos getInfosLinux() const;
#else
#error "Plateform not supported"
#endif
};
}
}
}