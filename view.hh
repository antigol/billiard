#ifndef VIEW_HH
#define VIEW_HH

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QTime>
#include <QGLFunctions>
#include "glsphere.hh"
#include "system.hh"

class View : public QGLWidget, protected QGLFunctions
{
  Q_OBJECT
public:
  explicit View(QWidget* parent = 0);
  virtual ~View();

  System* system;

private:
  void initializeGL();
  void paintGL();
  void draw(QGLShaderProgram& program, const QMatrix4x4& VP, const QMatrix4x4& VPdepth = QMatrix4x4());
  void drawShadow(const QMatrix4x4& VP);

  void timerEvent(QTimerEvent* e);

  QTime time;

  QMatrix4x4 P;
  QGLShaderProgram program;
  GLSphere sphere;

  // for the shadow
  GLuint frameBuffer;
  GLuint depthTexture;
  QGLShaderProgram program_rtt;
};

#endif // VIEW_HH
