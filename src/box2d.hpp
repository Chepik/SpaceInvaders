#pragma once

#include "point2d.hpp"

class Box2D
{
public:
  Box2D() = default;

  Box2D(Box2D const & obj)
    : m_point1(obj.m_point1),
      m_point2(obj.m_point2)
  {}

  Box2D(Box2D && obj);

  Box2D(Point2D x, Point2D y)
    : m_point1(x), m_point2(y)
  {}

  Point2D & point1() { return m_point1; }
  Point2D & point2() { return m_point2; }

  Point2D const & point1() const { return m_point1; }
  Point2D const & point2() const { return m_point2; }

  Box2D & operator = (Box2D const & obj);
  Box2D & operator = (Box2D && obj);

  bool operator == (Box2D const & obj) const;

  void swap(Box2D & obj);
  ///
  /// Check if two boxes are intersected with each other.
  ///
  static bool checkBoxes(Box2D const & box1,
                         Box2D const & box2);

  ///
  /// Check if a point inside a box.
  ///
  static bool checkInside(Box2D const & box,
                          Point2D const & point);

private:
  Point2D m_point1 = { 0.0f, 0.0f };
  Point2D m_point2 = { 1.0f, 1.0f };
};


