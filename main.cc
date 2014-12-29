#include <QApplication>
#include "view.hh"
#include "system.hh"

#include <QQuaternion>
#include <QDebug>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  System s;
  s.R = 1;
  s.m = 1;
  s.I = 2./5. * s.m * s.R*s.R;

  Ball b1;
  b1.L = Vec3(0,0,0);
  b1.p = Vec3(0,0,0);
  b1.q = Vec3(0,0,0);

  s.balls.push_back(b1);

  Ball b2 = b1;
  b2.L = Vec3(0,0,0);
  b2.q = Vec3(3,1.5,0);
  b2.p = Vec3(-2,0,0);

  s.balls.push_back(b2);

  View w;
  w.system = &s;
  w.show();

  return a.exec();
}
