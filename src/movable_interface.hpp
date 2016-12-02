#pragma once

class MovableInterface
{
public:
  virtual ~MovableInterface() = default;
  virtual void Move() = 0;
};
