#include "system.hh"
#include <QDebug>

using namespace std;

System::System()
{
}

void System::step(double dt)
{
  for (uint i = 0; i < balls.size(); ++i) {
    Ball& b = balls[i];

    b.q += b.p / m * dt;

    double angle = Vec3::length(b.L) / I * dt * 180./M_PI;
    b.Qrot = QQuaternion::fromAxisAndAngle(b.L[0], b.L[1], b.L[2], angle) * b.Qrot;
  }
}

void System::simulate(double dt)
{
  double ddt = 1e-5 * dt;

  for (uint i = 0; i < balls.size(); ++i) {
    Ball* a = &balls[i];
    for (uint j = i+1; j < balls.size(); ++j) {
      Ball* b = &balls[j];
      while (isCollision(a, b)) {
        collision(a, b);
        step(ddt);
        dt -= ddt;
      }
    }
  }

  step(dt);
}

bool System::isCollision(Ball* a, Ball* b)
{
  Vec3 n = a->q - b->q;
  return Vec3::length(a->q - b->q) < 2.*R && Vec3::dot(n, a->p * m - b->p * m) < 0.0;
}

void System::collision(Ball* a, Ball* b)
{
  static int count = 0;
  count++;
  //if (count >= 2) return;
  qDebug() << "collision" << count;

  Vec3 vg = (a->p + b->p) / (2.0 * m);
  Vec3 p = a->p - m * vg;
  Vec3 r = (b->q - a->q) / 2.0;

  // hypothèses
  // - choc instantané
  // - force appliquée au point de l'impact uniquement
  // - q. de mvt / moment cinétique conservés
  // - choc élastique
  // - force colinéaire au vecteur suivant :
  Vec3 v = 2.*p/m + 1./I * Vec3::cross(a->L + b->L, r);
  v /= Vec3::length(v);
  v = r / Vec3::length(r) + 0.2 * v;

  // dp = \int Force dt = alpha v, avec alpha tel que les hypothèses soient vérifiées
  double alpha = (2.*I/m * Vec3::dot(p, v) + Vec3::dot(a->L+b->L, Vec3::cross(r, v)))
                 / (I/m * Vec3::norm(v) + Vec3::norm(Vec3::cross(r, v)));

  Vec3 dp = alpha * v;

  Vec3 p_prim = p - dp;
  a->p = m * vg + p_prim;
  b->p = m * vg - p_prim;

  Vec3 dL = Vec3::cross(r, dp);
  a->L = a->L - dL;
  b->L = b->L - dL;
}
