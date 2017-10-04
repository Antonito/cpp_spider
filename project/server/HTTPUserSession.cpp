#include "HTTPUserSession.h"
#include "Logger.hpp"

namespace spider
{
namespace http
{
HTTPUserSession::HTTPUserSession(boost::asio::io_service &io_service,
    std::map<std::string, std::function<void(std::uint32_t)>> const &routes, std::uint32_t const id) : m_socket(io_service), m_buff(), m_header(routes), m_id(id)
{
}

HTTPUserSession::~HTTPUserSession()
{
}

void HTTPUserSession::sessionStart(std::shared_ptr<HTTPUserSession> that)
{
  boost::asio::async_read_until(that->getSocket(), that->getBuff(), '\r', [that](const boost::system::error_code &e, std::size_t size)
      {
      std::string line;
      std::string trash;
      std::istream stream(&that->getBuff());
      std::getline(stream, line, '\r'); //http requests end with /r/n
      std::getline(stream, trash, '\n'); // ^
      that->getHeader().onReadRequestLine(line); // TODO check if client needed inside
      readNextLine(that);
      });
}

void HTTPUserSession::readNextLine(std::shared_ptr<HTTPUserSession> that)
{
  boost::asio::async_read_until(that->getSocket(), that->getBuff(), '\r', [that](const boost::system::error_code &e, std::size_t size)
      {
      std::string line;
      std::string trash;
      std::istream stream(&that->getBuff());
      std::getline(stream, line, '\r'); //http requests end with /r/n
      std::getline(stream, trash, '\n'); // ^
      that->getHeader().onReadHeader(line);

      if (line.length() == 0)
      {
      if (that->getHeader().contentLength() == 0)
      {
      nope::log::Log(Info) << "end reading AControl's client";
      that->getHeader().getResponse(that->getId()); //will find the route requested
      /*std::shared_ptr<std::string> str = std::make_shared<std::string>(that->getHeader().getResponse());
      boost::asio::async_write(that->getSocket(), boost::asio::buffer(str->c_str(), str->length()),
          [that, str](boost::system::error_code const &e, std::size_t n)
          {
            nope::log::Log(Debug) << "wrtting on socket...";
          });*/
      }
      else
      {
        readBody(that);
      }
      }
      else
      {
        readNextLine(that);
      }
      });
}

void HTTPUserSession::readBody(std::shared_ptr<HTTPUserSession> that)
{
  std::uint32_t nbuff = 1024;
  std::shared_ptr<std::vector<char>> buff = std::make_shared<std::vector<char>>(nbuff);
  boost::asio::async_read(that->getSocket(), boost::asio::buffer(*buff, nbuff),
      [that](boost::system::error_code const &e, std::size_t n)
      {
        nope::log::Log(Debug) << "reading on socket...";
      });
}

std::uint32_t const HTTPUserSession::getId() const
{
  return (m_id);
}

boost::asio::ip::tcp::socket &HTTPUserSession::getSocket()
{
  return (m_socket);
}

HTTPHeader &HTTPUserSession::getHeader()
{
  return (m_header);
}

boost::asio::streambuf &HTTPUserSession::getBuff()
{
  return (m_buff);
}
}
}
