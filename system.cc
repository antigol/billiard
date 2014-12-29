#include "system.hh"
#include <QDebug>

using namespace std;

System::System()
{
}

void System::simulate(double dt)
{
  for (uint i = 0; i < balls.size(); ++i) {
    Ball& b = balls[i];

    b.q += b.p / m * dt;
    QMatrix4x4 rot;
    rot.rotate(dt/I * 180./M_PI, b.L[0], b.L[1], b.L[2]);
    b.Mrot = rot * b.Mrot;

    double angle = Vec3::length(b.L) / I * dt * 180./M_PI;
    b.Qrot = QQuaternion::fromAxisAndAngle(b.L[0], b.L[1], b.L[2], angle) * b.Qrot;
  }

  for (uint i = 0; i < balls.size(); ++i) {
    Ball* a = &balls[i];
    for (uint j = i+1; j < balls.size(); ++j) {
      Ball* b = &balls[j];
      Vec3 n = a->q - b->q;
      if (Vec3::length(balls[i].q - balls[j].q) < 2.*R &&
          Vec3::dot(n, a->p * m - b->p * m) < 0.0) {
        collision(a, b);
      }
    }
  }
}

void System::collision(Ball* a, Ball* b)
{
  static bool first = true;
  if (!first) return;
  //first = false;

  qDebug() << "collision";

  Vec3 vg = (a->p + b->p) / (2.0 * m);
  Vec3 p = a->p - m * vg;
  Vec3 r = (b->q - a->q) / 2.0;

  // vitesse du point d'impact
  Vec3 v = 2.*p/m + 1./I * Vec3::cross(a->L + b->L, r);

  // coeff alpha
  double alpha = (2.*I/m * Vec3::dot(p, v) + Vec3::dot(a->L+b->L, Vec3::cross(r, v)))
                 / (I/m * Vec3::norm(v) + Vec3::norm(Vec3::cross(r, v)));

  Vec3 p_prim = p - alpha * v;
  a->p = m * vg + p_prim;
  b->p = m * vg - p_prim;

  Vec3 L_prim = alpha * Vec3::cross(r, v);
  a->L = a->L - L_prim;
  b->L = b->L - L_prim;
}
