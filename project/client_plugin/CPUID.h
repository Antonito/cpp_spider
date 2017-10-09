#pragma once

#include <cstdint>
#include <array>

#ifdef _WIN32
#include <intrin.h>
#endif

class CPUID
{
public:
  explicit CPUID(std::uint32_t i) : m_regs{}
  {
#ifdef _WIN32
    __cpuid(reinterpret_cast<int *>(m_regs.data()), static_cast<int>(i));

#else
    __asm__ __volatile__("cpuid"
                         : "=a"(m_regs[0]), "=b"(m_regs[1]), "=c"(m_regs[2]),
                           "=d"(m_regs[3])
                         : "a"(i), "c"(0));
// ECX is set to zero for CPUID function 4
#endif
  }

  std::uint32_t const &EAX() const
  {
    return m_regs[0];
  }
  std::uint32_t const &EBX() const
  {
    return m_regs[1];
  }
  std::uint32_t const &ECX() const
  {
    return m_regs[2];
  }
  std::uint32_t const &EDX() const
  {
    return m_regs[3];
  }

private:
  std::array<std::uint32_t, 4> m_regs;
};
