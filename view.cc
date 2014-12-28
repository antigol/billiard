#include "view.hh"

View::View(QWidget *parent) :
  QGLWidget(parent)
{
}

void View::initializeGL()
{
  program.addShaderFromSourceFile(QGLShader::Vertex, ":/simple.vert");
  program.addShaderFromSourceFile(QGLShader::Fragment, ":/simple.frag");
  program.bindAttributeLocation("vertex_position", 0);
  program.bindAttributeLocation("vertex_normal", 1);
  program.link();
}

void View::resizeGL(int w, int h)
{
  P.setToIdentity();
  P.perspective(70., h?w/h:1., 0.01, 10);
}

void View::paintGL()
{
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
