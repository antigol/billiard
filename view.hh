#ifndef VIEW_HH
#define VIEW_HH

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>

class View : public QGLWidget
{
  Q_OBJECT
public:
  explicit View(QWidget* parent = 0);

private:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  QMatrix4x4 P;
  QGLShaderProgram program;
};

#endif // VIEW_HH
