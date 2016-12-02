#pragma once

#include "object.h"

class EventListenerInterface : public Object
{
 public:
  virtual ~EventListenerInterface() = default;

  /// Callback.
  virtual void Notify() = 0;
};
