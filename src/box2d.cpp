#include "box2d.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

Box2D::Box2D(Box2D && obj)
{
  swap(obj);
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
  return *this;
}

bool Box2D::operator == (Box2D const & obj) const
{
  return m_boxMin == obj.m_boxMin
      && m_boxMax == obj.m_boxMax;
}

void Box2D::swap(Box2D & obj)
{
  std::swap(m_boxMin, obj.m_boxMin);
  std::swap(m_boxMax, obj.m_boxMax);
}

bool Box2D::checkInside(Box2D const & box,
                        Point2D const & point)
{
  return ! (point.x() < box.boxMin().x() ||
              point.x() > box.boxMax().x() ||
              point.y() < box.boxMin().y() ||
              point.y() > box.boxMax().y());
}



bool Box2D::checkBoxes(Box2D const & box1,
                       Box2D const & box2)
{
  // If two boxes are not intersected with each other
  // then return false.
  return ! ( box1.boxMax().x() <= box2.boxMin().x() ||
               box2.boxMax().x() <= box1.boxMin().x() ||
               box1.boxMin().y() >= box2.boxMax().y() ||
               box2.boxMin().y() >= box1.boxMax().y() );
}

Box2D Box2D::createBox(Point2D const & minPoint,
                       Point2D const & maxPoint)
{
  Point2D min(minPoint);
  Point2D max(maxPoint);
  if (min.x() > max.x())
  {
    std::swap(min.x(),max.x());
  }
  if(min.y() > max.y())
  {
    std::swap(min.y(),max.y());
  }
  if(min.x() == max.x() || max.y() == min.y())
  {
    throw std::invalid_argument("You must create a correct box!");
  }
  return Box2D(min, max);
}

std::ostream & operator << (std::ostream & os,
                            Box2D const & obj)
{
  os << "Box2D [BoxMin: " << obj.boxMin() << "; "
     << "BoxMax: " << obj.boxMax() << "]";
  return os;
}
