#include <logger.hpp>
#include "gtest/gtest.h"
#include "event_source.h"

class A : public EventListenerInterface
{
 public:
  void Notify() override
  {
    Logger::Instance(LogLevel::info) << "Notify from A!" << "\n";
  }
};

class B : public EventListenerInterface
{
 public:
  void Notify() override
  {
    Logger::Instance(LogLevel::info) << "Notify from B!" << "\n";
  }
};

TEST(event_source_test, test_register_listener)
{
  EventSource eventSource;
  std::shared_ptr<A> listener = std::make_shared<A>();

  eventSource.RegisterListener(listener);

  EXPECT_EQ(eventSource.Count(), 1);
}

TEST(event_source_test, test_notify_1)
{
  EventSource eventSource;
  std::shared_ptr<A> listener = std::make_shared<A>();

  eventSource.RegisterListener(listener);

  eventSource.NotifyListeners();

  EXPECT_EQ(eventSource.Count(), 1);
}

TEST(event_source_test, test_notify_2)
{
  EventSource eventSource;
  std::shared_ptr<A> listener_1 = std::make_shared<A>();
  std::shared_ptr<B> listener_2 = std::make_shared<B>();

  eventSource.RegisterListener(listener_1);
  eventSource.RegisterListener(listener_2);

  eventSource.NotifyListeners();

  EXPECT_EQ(eventSource.Count(), 2);
}