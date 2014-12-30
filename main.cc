#include <QApplication>
#include "view.hh"
#include "system.hh"

#include <QQuaternion>
#include <QDebug>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  System s;
  s.R = 2;
  s.m = 1;
  s.I = 2./5. * s.m * s.R*s.R;

  Ball b1;
  b1.L = Vec3(15,1,0);
  b1.p = Vec3(0,0,0);
  b1.q = Vec3(0,0,0);

  s.balls.push_back(b1);

  Ball b2 = b1;
  b2.L = Vec3(0,0,0);
  b2.q = Vec3(10, 0, 3);
  b2.p = Vec3(-4,0,0);

  s.balls.push_back(b2);

  Ball b3 = b1;
  b3.L = Vec3(0,0,0);
  b3.q = Vec3(-5, 0, 15);
  b3.p = Vec3(0,0,0);

  s.balls.push_back(b3);

  Wall w1;
  w1.n = Vec3(0,0,1);
  w1.p = Vec3(-100,-100,-5);
  w1.q = Vec3(+100,-100,-5);
  w1.r = Vec3(+100,+100,-5);
  w1.s = Vec3(-100,+100,-5);

  s.walls.push_back(w1);

  View w;
  w.system = &s;
  w.show();
  //w.showFullScreen();

  return a.exec();
}
