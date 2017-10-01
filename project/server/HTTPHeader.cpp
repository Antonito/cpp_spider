#include "HTTPHeader.h"
#include "Logger.hpp"

namespace spider
{
namespace http
{
HTTPHeader::HTTPHeader(std::map<std::string, std::function<std::string()>> const &routes)
  : m_method(), m_url(), m_version(), m_headers(), m_routes(routes)
{
}

HTTPHeader::~HTTPHeader()
{
}

std::string HTTPHeader::getResponse()
{
  std::stringstream res;
  auto search = m_routes.find(m_url);
  if(search != m_routes.end())
  {
    nope::log::Log(Info) << "response is: " << search->first;
    return (search->second());
  }
  else
  {
    std::string sHTML = "<html><body><h1>404 Not Found</h1><p>There's nothing here.</p></body></html>";
    res << "HTTP/1.1 404 Not Found" << std::endl;
    res << "content-type: text/html" << std::endl;
    res << "content-length: " << sHTML.length() << std::endl;
    res << std::endl;
    res << sHTML;
  }

  //TODO: insert in doc as example to create a route in plugin (potentially)
  /*if (m_url == "/test")
  {
    std::string sHTML = "<html><body><h1>Hello World</h1><p>This is a test web server in c++</p></body></html>";
    res << "HTTP/1.1 200 OK" << std::endl;
    res << "content-type: text/html" << std::endl;
    res << "content-length: " << sHTML.length() << std::endl;
    res << std::endl;
    res << sHTML;
  }*/
  return (res.str());
}

std::uint32_t HTTPHeader::contentLength()
{
  auto request = m_headers.find("content-length");
  if (request != m_headers.end())
  {
    std::stringstream ss(request->second);
    std::uint32_t len;
    ss >> len;
    return (len);
  }
  return (0);
}

void HTTPHeader::onReadHeader(std::string line)
{
  nope::log::Log(Debug) << "[HTTPHeader] header: " << line;

  std::stringstream ss(line);
  std::string headerName;
  std::getline(ss, headerName, ':');
  std::string value;
  std::getline(ss, value);
  m_headers[headerName] = value;
}

void HTTPHeader::onReadRequestLine(std::string line)
{
  std::stringstream ss(line);
  ss >> m_method;
  ss >> m_url;
  ss >> m_version;

  nope::log::Log(Info) << "request '" << m_method << "' on : " << m_url;
}
}
}
