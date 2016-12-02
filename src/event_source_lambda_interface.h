#pragma once

#include <functional>

class EventSourceLambdaInterface
{
 public:
  virtual ~EventSourceLambdaInterface();

  using TEventHandler = std::function<void()>;

  /// Register a listener.
  virtual void RegisterListener(
      TEventHandler const & listener) = 0;

  /// Notify all listeners.
  virtual void NotifyListeners() = 0;

  /// Return a number of listeners.
  virtual std::size_t Count() const = 0;
};
