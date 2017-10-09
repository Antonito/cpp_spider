#pragma once

#include <cstdint>
#include <array>
#include <cctype>

namespace spider
{
  template <std::size_t S>
  class RingBuffer
  {
  public:
    RingBuffer() : m_data{}, m_ndxWrite(0), m_ndxRead(0)
    {
    }

    ~RingBuffer()
    {
    }

    RingBuffer(RingBuffer const &) = delete;
    RingBuffer(RingBuffer &&) = delete;
    RingBuffer &operator=(RingBuffer const &) = delete;
    RingBuffer &operator=(RingBuffer &&) = delete;

    void write(std::uint8_t const *data, std::size_t len)
    {
      if (data)
	{
	  for (std::size_t i = 0; i < len; ++i)
	    {
	      if (m_ndxWrite == sizeof(m_data))
		{
		  m_ndxWrite = 0;
		}
	      if (std::isprint(data[i]) || data[i] == '\r' || data[i] == '\n')
		{
		  m_data[m_ndxWrite] = data[i];
		}
	      else
		{
		  m_data[m_ndxWrite] = '?';
		}
	      ++m_ndxWrite;
	    }
	}
    }

    void read(std::uint8_t *data, std::size_t len)
    {
      if (data)
	{
	  for (std::size_t i = 0; i < len; ++i)
	    {
	      if (m_ndxRead == sizeof(m_data))
		{
		  m_ndxRead = 0;
		}
	      data[i] = m_data[m_ndxRead];
	      m_data[m_ndxRead] = '\0';
	      ++m_ndxRead;
	    }
	}
    }

    std::size_t hasCommand() const
    {
      std::size_t  i = m_ndxRead;
      std::int32_t len = 0;

      i = m_ndxRead;
      for (std::uint32_t j = 0; j < sizeof(m_data); ++j)
	{
	  if (i >= sizeof(m_data))
	    {
	      i = 0;
	    }
	  if (m_data[i] == '\n')
	    {
	      return (static_cast<std::size_t>(len) + 1);
	    }
	  ++i;
	  ++len;
	}
      return (0);
    }

  private:
    std::array<std::uint8_t, S> m_data;
    std::size_t m_ndxWrite;
    std::size_t m_ndxRead;
  };
};
