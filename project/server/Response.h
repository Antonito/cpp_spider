#include <cstddef>
#include <string>

namespace spider
{
  namespace server
  {

#if defined   __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

    class Response
    {
    public:
      Response();
      ~Response();

      Response(Response const &);
      Response &operator=(Response const &);

      Response(Response &&) = delete;
      Response &operator=(Response &&) = delete;

      std::uint32_t      getNbClient() const;
      void               setNbClient(std::uint32_t);
      std::string const &getResponse() const;
      void               setResponse(std::string);

    private:
      std::string   m_response;
      std::uint32_t m_nbClient;
    };

#if defined   __clang__
#pragma clang diagnostic pop
#endif
  }
}
