#ifndef BALL_HH
#define BALL_HH

#include <QVector3D>
#include <QQuaternion>

class Ball
{
public:
  Ball();

  void move(double dt);

  QVector3D position;
  QQuaternion rotation;

  QVector3D torque;
  QVector3D force;

  QVector3D omega;
  QVector3D velocity;
};

#endif // BALL_HH
