#pragma once

#include <algorithm>
#include <list>
#include <memory>

#include "event_listener_interface.h"
#include "event_source_interface.h"

class EventSource : public EventSourceInterface
{
 public:
  ~EventSource() override;

  void RegisterListener(
      std::shared_ptr<EventListenerInterface> listener) override;

  void UnregisterListener(
      std::shared_ptr<EventListenerInterface> listener) override;

  void NotifyListeners() override;

  size_t Count() const override;

 private:
  std::list<std::shared_ptr<EventListenerInterface>> m_listeners;
};