#include "box2d.hpp"

Box2D::Box2D(Box2D && obj)
{
  swap(obj);
  std::cout << "Move copy constructor" <<std::endl;
}

Box2D & Box2D::operator = (Box2D const & obj)
{
  if (this == &obj) return *this;
  Box2D tmp(obj);
  swap(tmp);
  return *this;
}

Box2D & Box2D::operator = (Box2D && obj)
{
  swap(obj);
  std::cout << "Move operator=" <<std::endl;
  return *this;
}

bool Box2D::operator == (Box2D const & obj) const
{
  return m_point1 == obj.m_point1
      && m_point2 == obj.m_point2;
}

void Box2D::swap(Box2D & obj)
{
  std::swap(m_point1, obj.m_point1);
  std::swap(m_point2, obj.m_point2);
}

bool Box2D::checkInside(const Box2D &box, const Point2D &point)
{
  if ((point.x() >= box.m_point1.x() &&
       point.x() <= box.m_point2.x() &&
       point.y() >= box.m_point1.y() &&
       point.y() <= box.m_point2.y()) ||
      (point.x() >= box.m_point2.x() &&
       point.x() <= box.m_point1.x() &&
       point.y() >= box.m_point2.y() &&
       point.y() <= box.m_point1.y())
      )
  {
    return true;
  }

  return false;
}



bool Box2D::checkBoxes(const Box2D &box1, const Box2D &box2)
{
  // If two boxes are not intersected with each other
  // then return false.
  if( box1.point2().x() <= box2.point1().x() ||
      box2.point2().x() <= box1.point1().x() ||
      box1.point1().y() >= box2.point2().y() ||
      box2.point1().y() >= box1.point2().y() )
  {
    return false;
  }

  return true;
}
