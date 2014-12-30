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
  program.bindAttributeLocation("vertex_normal", 1);
  program.link();

  sphere.initialize(30, 30);

  time.start();
  startTimer(0);
}

void View::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  program.bind();

  double aspect = 0.5*width() / height();
  P.setToIdentity();
  //P.perspective(40., 0.5*width() / height(), 0.01, 1000);
  P.ortho(-10*aspect, 10*aspect, -10, 10, 0.01, 1000);

  program.setUniformValue("l", 0, 0, 1);

  QMatrix4x4 V;

  glViewport(0, 0, 0.5*width(), height());
  V.lookAt(QVector3D(0,-10,0), QVector3D(0,0,0), QVector3D(0,0,1));
  draw(P*V);

  glViewport(0.5*width(), 0, 0.5*width(), height());
  V.rotate(90, 1,0,0);
  draw(P*V);

  glViewport(0, 0, width(), height());
  program.setUniformValue("MVP", QMatrix4x4());
  program.setUniformValue("Mn", QMatrix4x4().normalMatrix());
  glBegin(GL_LINES);
  program.setAttributeValue(1, 0, 0, 1);
  program.setAttributeValue(0, 0, -1);
  program.setAttributeValue(0, 0, 1);
  glEnd();

  /*
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_MULTISAMPLE);
  QPainter p;
  p.begin(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::white);
  p.setFont(QFont(QString::fromUtf8("DejaVu Sans Mono")));
  p.drawText(QPointF(10.0, 20.0), "FRONT");
  p.drawText(QPointF(10.0+0.5*width(), 20.0), "TOP");
  p.end();
  */
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
    program.setUniformValue("Mn", M.normalMatrix());
    sphere.bind();
    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    program.setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);
    program.enableAttributeArray(0);
    program.enableAttributeArray(1);
    sphere.drawElements();
    program.disableAttributeArray(0);
    program.disableAttributeArray(1);
    sphere.release();  }
}

void View::timerEvent(QTimerEvent*)
{
  double dt = time.restart() / 1000.;
  system->simulate(dt);

  updateGL();
}
