#include "view.hh"

using namespace std;

View::View(QWidget *parent) :
  QGLWidget(parent), system(nullptr)
{
  setAutoFillBackground(false);
}

void View::initializeGL()
{
  program.addShaderFromSourceFile(QGLShader::Vertex, ":/simple.vert");
  program.addShaderFromSourceFile(QGLShader::Fragment, ":/simple.frag");
  program.bindAttributeLocation("vertex_position", 0);
  //program.bindAttributeLocation("vertex_normal", 1);
  program.link();

  sphere.initialize();

  time.start();
  startTimer(0);
}

void View::resizeGL(int w, int h)
{
}

void View::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  program.bind();

  P.setToIdentity();
  P.perspective(40., 0.5*width() / height(), 0.01, 1000);

  QMatrix4x4 V;

  glViewport(0, 0, 0.5*width(), height());
  V.lookAt(QVector3D(0,-15,0), QVector3D(0,0,0), QVector3D(0,0,1));
  draw(P*V);

  glViewport(0.5*width(), 0, 0.5*width(), height());
  V.rotate(90, 1,0,0);
  draw(P*V);

  QPainter p;
  p.begin(this);
  p.setPen(Qt::white);
  p.drawLine(0.5 * width(), 0, 0.5 * width(), height());
  p.end();
}

void View::draw(const QMatrix4x4& VP)
{
  if (!system) return;

  QMatrix4x4 M;
  for (uint i = 0; i < system->balls.size(); ++i) {
    Ball* b = &system->balls[i];

    M.setToIdentity();
    M.translate(b->q[0], b->q[1], b->q[2]);
    M.rotate(b->Qrot);
    M.scale(system->R);

    program.setUniformValue("MVP", VP*M);
    sphere.draw(program, 0);
  }
}

void View::timerEvent(QTimerEvent*)
{
  double dt = time.restart() / 1000.;
  system->simulate(dt);

  updateGL();
}
