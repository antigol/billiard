#ifndef WALL_HH
#define WALL_HH

#include <QVector3D>

class Wall
{
public:
  Wall();

  QVector3D point;
  QVector3D normal;
};

#endif // WALL_HH
