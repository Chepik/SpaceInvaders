#pragma once

class Object
{
 public:
  Object() = default;

  bool operator == (Object const &) { return  false; };
};
