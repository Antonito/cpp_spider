#include "HTTPHeader.h"
#include "Logger.hpp"

namespace spider
{
namespace http
{
HTTPHeader::HTTPHeader(std::map<std::string, std::function<void(std::uint32_t)>> const &routes)
  : m_method(), m_url(), m_version(), m_headers(), m_routes(routes)
{
}

HTTPHeader::~HTTPHeader()
{
}

void HTTPHeader::getResponse(std::uint32_t id)
{
  std::stringstream res;
  std::string realURL(m_url.substr(0, m_url.find("/", 1)));
  auto search = m_routes.find(realURL);
  if(search != m_routes.end())
  {
    nope::log::Log(Info) << "response is: " << search->first;
    return (search->second(id));
  }
  else
  {
    m_routes.at("/404")(id);
  }

  //TODO: insert in doc as example to create a route in plugin (potentially)
  // if (m_url == "/test")
  /*{
    std::string sHTML = "<html><body><h1>Hello World</h1><p>This is a test web server in c++</p></body></html>";
    res << "HTTP/1.1 200 OK" << std::endl;
    res << "content-type: text/html" << std::endl;
    res << "content-length: " << sHTML.length() << std::endl;
    res << std::endl;
    res << sHTML;
  }*/
  //return (res.str());
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
