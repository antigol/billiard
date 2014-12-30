#include "wall.hh"


Wall::Wall(Vec3 p, Vec3 q, Vec3 r)
{
  Vec3 a = r - q;
  Vec3 b = p - q;
  this->p = p;
  this->q = q;
  this->r = r;
  this->s = p + a;
  this->n = Vec3::cross(a, b);
  this->n /= Vec3::length(this->n);
}
