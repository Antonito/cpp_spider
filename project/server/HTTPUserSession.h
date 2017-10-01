#pragma once

#include <boost/asio.hpp>
#include "HTTPHeader.h"

namespace spider
{
namespace http
{
class HTTPUserSession
{
  public:
    HTTPUserSession(boost::asio::io_service &io_service, std::map<std::string, std::function<std::string()>> const &routes);
    ~HTTPUserSession();

    HTTPUserSession(HTTPUserSession const &) = delete;
    HTTPUserSession(HTTPUserSession &&) = delete;
    HTTPUserSession &operator=(HTTPUserSession const &) = delete;
    HTTPUserSession &operator=(HTTPUserSession &&) = delete;

    static void sessionStart(std::shared_ptr<HTTPUserSession> that);
    static void readBody(std::shared_ptr<HTTPUserSession> that);
    static void readNextLine(std::shared_ptr<HTTPUserSession> that);

    //non const because this is used by acceptor to set the socket
    boost::asio::ip::tcp::socket &getSocket();
    boost::asio::streambuf &getBuff();
    HTTPHeader &getHeader();

  private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buff;
    HTTPHeader m_header;
};
}
}
