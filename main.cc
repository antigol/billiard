#include <QApplication>
#include "view.hh"

#include <QQuaternion>
#include <QDebug>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  View w;
  w.show();

  QQuaternion x = QQuaternion::fromAxisAndAngle(2,0,0, 15);
  qDebug() << x;

  return a.exec();
}
