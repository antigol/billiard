#ifndef VIEW_HH
#define VIEW_HH

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QTime>
#include "glsphere.hh"
#include "system.hh"

class View : public QGLWidget
{
  Q_OBJECT
public:
  explicit View(QWidget* parent = 0);

  System* system;

private:
  void initializeGL();
  void paintGL();
  void draw(const QMatrix4x4& VP);

  void timerEvent(QTimerEvent* e);

  QTime time;

  QMatrix4x4 P;
  QGLShaderProgram program;
  GLSphere sphere;
};

#endif // VIEW_HH
