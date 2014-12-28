#include "ball.hh"
#include <cmath>

Ball::Ball()
{
}

void Ball::move(double dt)
{
  position += velocity * dt;
  rotation = QQuaternion::fromAxisAndAngle(omega, omega.length() * dt * 180 / M_PI) * rotation;
}
