#pragma once

#include <cstddef>
#include <string>

namespace spider
{
  namespace server
  {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

    class IClient
    {
    public:
      virtual ~IClient() = default;

      virtual std::string const &getOS() const = 0;
      virtual std::string const &getIP() const = 0;
      virtual std::string const &getGeo() const = 0;
      virtual std::string const &getName() const = 0;
      virtual void send(std::string const &buffer) = 0;
      virtual std::size_t receive() = 0;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
