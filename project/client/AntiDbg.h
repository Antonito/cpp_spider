#pragma once

namespace spider
{
  namespace misc
  {
    class Debugger
    {
    public:
      static bool isBeingAV();
      static bool isDebuggerPresent();
    };
  }
}