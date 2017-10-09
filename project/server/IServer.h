#pragma once

namespace spider
{
  namespace server
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
    class IServer
    {
    public:
      virtual ~IServer() = default;
      virtual void run() = 0;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
