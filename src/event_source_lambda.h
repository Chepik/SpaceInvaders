#pragma once

#include <list>

#include "event_source_lambda_interface.h"

class EventSourceLambda : public EventSourceLambdaInterface
{
 public:
  void RegisterListener(
      TEventHandler const & listener) override;

  void NotifyListeners() override ;

  std::size_t Count() const override;

 private:
  std::list<TEventHandler> m_listeners;
};

