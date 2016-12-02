#pragma once

#include <memory>

/// Singleton template.
template <typename T>
class Singleton {
 public:
  template<typename... Args>
  static T & Instance(Args && ... args)
  {
    static T instance(std::forward<Args>(args)...);
    return instance;
  }

 protected:
  Singleton() = default;
  virtual ~Singleton() = default;

  Singleton(Singleton const &) = delete;
  Singleton & operator = (Singleton const &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton & operator = (Singleton &&) = delete;
};
