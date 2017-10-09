#pragma once

#include <boost/asio.hpp>
#include <functional>
#include "HTTPHeader.h"

namespace spider
{
  namespace http
  {
#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    class HTTPUserSession
    {
    public:
      HTTPUserSession(
          boost::asio::io_service &io_service,
          std::map<std::string,
                   std::function<void(std::uint32_t, std::uint32_t)>> const
              &               routes,
          std::uint32_t const id);
      ~HTTPUserSession();

      HTTPUserSession(HTTPUserSession const &) = delete;
      HTTPUserSession(HTTPUserSession &&) = delete;
      HTTPUserSession &operator=(HTTPUserSession const &) = delete;
      HTTPUserSession &operator=(HTTPUserSession &&) = delete;

      static void sessionStart(std::shared_ptr<HTTPUserSession> that);
      static void readBody(std::shared_ptr<HTTPUserSession> that);
      static void readNextLine(std::shared_ptr<HTTPUserSession> that);

      std::uint32_t getId() const;

      // non const because this is used by acceptor to set the socket
      boost::asio::ip::tcp::socket &getSocket();
      boost::asio::streambuf &      getBuff();
      HTTPHeader &                  getHeader();

    private:
      boost::asio::ip::tcp::socket m_socket;
      boost::asio::streambuf       m_buff;
      HTTPHeader                   m_header;
      std::uint32_t const          m_id;
    };
#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
