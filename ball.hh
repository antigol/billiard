#ifndef BALL_HH
#define BALL_HH

#include "vec3.hh"
#include <QMatrix4x4>
#include <QQuaternion>

struct Ball
{
  Vec3 q;
  QQuaternion Qrot;

  Vec3 torque;
  Vec3 force;

  Vec3 L;
  Vec3 p;
};

#endif // BALL_HH
