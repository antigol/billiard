#ifndef WALL_HH
#define WALL_HH

#include "vec3.hh"

struct Wall
{
  Wall(Vec3 p, Vec3 q, Vec3 r);

  Vec3 p;
  Vec3 q;
  Vec3 r;
  Vec3 s;


  Vec3 n;
};

#endif // WALL_HH
