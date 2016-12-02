#include "event_source_lambda.h"

void EventSourceLambda::RegisterListener(
    TEventHandler const & listener)
{
  m_listeners.push_back(listener);
}

void EventSourceLambda::NotifyListeners()
{
  for (auto & listener : m_listeners)
  {
    listener();
  }
}

std::size_t EventSourceLambda::Count() const
{
  return m_listeners.size();
}
