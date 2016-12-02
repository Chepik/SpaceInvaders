#include "event_source.h"


void EventSource::RegisterListener(
    std::shared_ptr<EventListenerInterface> listener)
{
  if (std::find(begin(m_listeners),
                end(m_listeners),
                listener) == end(m_listeners))
  {
    m_listeners.push_back(listener);
  }
}

void EventSource::UnregisterListener(
    std::shared_ptr<EventListenerInterface> listener)
{
  if (std::find(begin(m_listeners),
                end(m_listeners),
                listener) != end(m_listeners))
  {
    m_listeners.remove(listener);
  }
}

void EventSource::NotifyListeners()
{
  for (auto & listener : m_listeners)
  {
    listener->Notify();
  }
}

size_t EventSource::Count() const
{
  return  m_listeners.size();
}

EventSource::~EventSource()
{}
