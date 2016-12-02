#pragma once

#include <memory>
#include "event_listener_interface.h"

class EventSourceInterface
{
 public:
  virtual ~EventSourceInterface();

  /// Register a listener.
  virtual void RegisterListener(
      std::shared_ptr<EventListenerInterface> listener) = 0;

  /// Deregister a listener.
  virtual void UnregisterListener(
      std::shared_ptr<EventListenerInterface> listener) = 0;

  /// Notify all listeners.
  virtual void NotifyListeners() = 0;

  /// Return a number of listeners.
  virtual std::size_t Count() const = 0;
};
