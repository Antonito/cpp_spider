#include "EventStorage.h"

namespace spider
{
  namespace server
  {
    EventStorage::EventStorage() : header(), ev(), mov()
    {
    }

    Storage::Storage() : m_storage{}
    {
    }

    Storage::~Storage()
    {
    }

    void Storage::push(EventStorage const &ev)
    {
      m_storage.push(ev);
    }
  }
}