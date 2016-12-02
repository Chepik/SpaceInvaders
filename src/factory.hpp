#pragma once

#include <memory>

class Factory
{
public:
  template<typename T, typename... Args>
  std::shared_ptr<T> Create(Args && ... args)
  {
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
  }
};
