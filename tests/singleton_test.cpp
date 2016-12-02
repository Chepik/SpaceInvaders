#include "gtest/gtest.h"
#include "singleton.h"

// Test without parameters in a constructor.
TEST(singleton_test, test_instance_1)
{
  class A
  {
   public:
    int m_a = 10;
  };

  A const & instance_1 = Singleton<A>::Instance();
  A const & instance_2 = Singleton<A>::Instance();

  EXPECT_EQ(instance_1.m_a, instance_2.m_a);
}

// Test with parameters in a constructor.
TEST(singleton_test, test_instance_2)
{
  class A
  {
   public:
    A(int a) : m_a(a) {}

    int m_a = 0;
  };

  int tmp = 10;

  A const & instance_1 = Singleton<A>::Instance(tmp);
  A const & instance_2 = Singleton<A>::Instance(tmp);

  EXPECT_EQ(instance_1.m_a, instance_2.m_a);
}
