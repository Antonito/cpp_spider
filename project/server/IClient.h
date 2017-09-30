#pragma once

#include <cstddef>
#include <string>

namespace spider
{
namespace server
{
class IClient
{
public:
  virtual ~IClient() = default;

  virtual std::string const &getOS() = 0;
  virtual std::string const &getIP() = 0;
  virtual std::string const &getGeo() = 0;
  virtual std::string const &getName() = 0;
  virtual std::size_t send(std::string const &buffer) = 0;
  virtual std::size_t receive() = 0;
};
}
}
