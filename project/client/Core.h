#pragma once

#include "GenLibrary.h"

namespace spider
{
namespace client
{
namespace library
{
class IPayload;
}

namespace core
{
class Core final
{
public:
  // The folder where the foo dynamic libraries are located
  explicit Core(std::string const &path);
  ~Core();

  Core(Core const &) = delete;
  Core(Core &&) = delete;
  Core &operator=(Core const &) = delete;
  Core &operator=(Core &&) = delete;

  int run();

private:
  GenLibrary m_lib;
  library::IPayload *m_payload;
};
}
}
}