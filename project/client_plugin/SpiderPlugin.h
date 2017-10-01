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

  private:
// SpiderPluginWindows.cpp
#if defined _WIN32
    bool initWindows();
    bool deinitWindows();
// SpiderPluginOSX.cpp
#elif defined __APPLE__
    bool initOSX();
    bool deinitOSX();
// SpiderPluginLinux.cpp
#elif defined __linux__
    bool initLinux();
    bool deinitLinux();
#else
#error "Plateform not supported"
#endif
};
}
}
}