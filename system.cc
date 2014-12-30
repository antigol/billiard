#include "system.hh"
#include <QDebug>
#include <iostream>

using namespace std;

System::System()
{
}

void System::step(double dt)
{
  for (uint i = 0; i < balls.size(); ++i) {
    Ball* a = &balls[i];

    Vec3 g = Vec3(0, 0, -10);

    for (uint j = 0; j < walls.size(); ++j) {
      Wall* w = &walls[j];
      Vec3 d = a->q - w->p;
      if (Vec3::dot(d, w->n) - R < 1e-5) {
        Vec3 d = a->q - w->p;
        d -= Vec3::dot(d, w->n) * w->n;
        a->q = w->p + d + w->n * R;

        Vec3 v = a->p / m + Vec3::cross(a->L / I, -w->n * R);
        //std::cout << a->p/m << std::endl;
        //std::cout << v << std::endl;
        if (Vec3::length(v) < 5e-3) {
          // static force

        } else {
          //qDebug() << "cinetic force";
          Vec3 f = 1.2 * Vec3::dot(m * g, -w->n) * (-v / Vec3::length(v));
          a->force += f;
          a->torque += Vec3::cross(-w->n * R, f);
        }

        // slowing
        if (Vec3::length(a->L) > 0) {
          //qDebug() << "slowing force";
          //Vec3 f = 0.05 * Vec3::dot(m * g, -w->n) * (-a->p / Vec3::length(a->p));
          //a->force += f;
          //a->torque += Vec3::cross(w->n * R, f); // to maintain v
          a->torque += -0.2 * Vec3::dot(m * g, -w->n) * a->L / Vec3::length(a->L);
        }
      }
    }
    a->force += m * g;

    a->p += dt * a->force;
    a->L += dt * a->torque;

    a->q += a->p / m * dt;

    double angle = Vec3::length(a->L) / I * dt * 180./M_PI;
    a->Qrot = QQuaternion::fromAxisAndAngle(a->L[0], a->L[1], a->L[2], angle) * a->Qrot;

    a->force.setNull();
    a->torque.setNull();
  }
}

void System::simulate(double dt)
{

  double ddt = 1e-5 * dt;

  for (uint i = 0; i < balls.size(); ++i) {
    Ball* a = &balls[i];
    for (uint j = i+1; j < balls.size(); ++j) {
      Ball* b = &balls[j];
      while (is_collision_ball(a, b)) {
        collision_ball(a, b);
        step(ddt);
        dt -= ddt;
      }
    }
    for (uint j = 0; j < walls.size(); ++j) {
      Wall* w = &walls[j];
      if (is_collision_wall(a, w)) {
        collision_wall(a, w);
      }
    }
  }

  step(dt);
}

bool System::is_collision_ball(Ball* a, Ball* b)
{
  Vec3 n = a->q - b->q;
  return Vec3::length(a->q - b->q) < 2.*R && Vec3::dot(n, a->p * m - b->p * m) < 0.0;
}

bool System::is_collision_wall(Ball* a, Wall* w)
{
  Vec3 d = a->q - w->p;
  return Vec3::dot(d, w->n) < R && Vec3::dot(a->p, w->n) < 0.;
}

void System::collision_ball(Ball* a, Ball* b)
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

void System::collision_wall(Ball* a, Wall* w)
{
  static int count = 0;
  count++;
  //if (count >= 2) return;
  qDebug() << "collision wall" << count;

  double p_perp = -Vec3::dot(a->p, w->n);

  cout << p_perp << endl;

  double cor = 0.5;
  if (p_perp < 0.15) {
    a->p = a->p + p_perp * w->n;
    Vec3 d = a->q - w->p;
    d -= Vec3::dot(d, w->n) * w->n;
    a->q = w->p + d + w->n * R;
  } else {
    a->p = a->p + (1. + cor) * p_perp * w->n;
  }
}
