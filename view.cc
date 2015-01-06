#include "view.hh"

using namespace std;

View::View(QWidget *parent) :
  QGLWidget(parent), system(nullptr)
{
  setAutoFillBackground(false);
}

View::~View()
{
  glDeleteFramebuffers(1, &frameBuffer);
  glDeleteTextures(1, &depthTexture);
}

void View::initializeGL()
{
  initializeGLFunctions();

  program.addShaderFromSourceFile(QGLShader::Vertex, ":/simple.vert");
  program.addShaderFromSourceFile(QGLShader::Fragment, ":/simple.frag");
  program.bindAttributeLocation("vertex_position", 0);
  program.bindAttributeLocation("vertex_normal", 1);
  program.link();

  sphere.initialize(30, 30);


  program_rtt.addShaderFromSourceFile(QGLShader::Vertex, ":/depthRTT.vert");
  program_rtt.addShaderFromSourceFile(QGLShader::Fragment, ":/depthRTT.frag");
  program_rtt.bindAttributeLocation("vertex_position", 0);
  program_rtt.link();

  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
  glDrawBuffer(GL_NONE);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    qDebug("Erreur avec le frame buffer");


  time.start();
  startTimer(0);
}

void View::paintGL()
{
  if (!system) return;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  program.bind();

  double aspect = 1.*width() / height();
  P.setToIdentity();
  P.perspective(40., aspect, 0.01, 1000);

  //P.ortho(-10*aspect, 10*aspect, -10, 10, 0, 1000);

  QMatrix4x4 V;

  glViewport(0, 0, 0.5*width(), height());
  V.lookAt(QVector3D(0,-60,0), QVector3D(0,0,0), QVector3D(0,0,1));

  drawShadow(P*V);
  /*
  draw(P*V);

  glViewport(0.5*width(), 0, 0.5*width(), height());
  V.rotate(90, 1,0,0);
  draw(P*V);

  glViewport(0, 0, width(), height());
  program.setUniformValue("MVP", QMatrix4x4());
  program.setUniformValue("Mn", QMatrix4x4().normalMatrix());
  program.setUniformValue("phong", false);
  program.setUniformValue("main_color", 0, 1, 0);
  glBegin(GL_LINES);
  program.setAttributeValue(1, 0, 0, 1);
  program.setAttributeValue(0, 0, -1);
  program.setAttributeValue(0, 0, 1);
  glEnd();
  */
}

void View::draw(QGLShaderProgram& program, const QMatrix4x4& VP, const QMatrix4x4& VPdepth, bool wall)
{
  QMatrix4x4 M;

  program.setUniformValue("lw", 0.2f);
  program.setUniformValue("line_color", 1, 0, 0);
  program.setUniformValue("main_color", 1, 1, 0.8);
  program.setUniformValue("phong", true);
  for (uint i = 0; i < system->balls.size(); ++i) {
    Ball* b = &system->balls[i];

    M.setToIdentity();
    M.translate(b->q[0], b->q[1], b->q[2]);
    M.rotate(b->Qrot);
    M.scale(system->R);

    program.setUniformValue("DepthBiasMVP", VPdepth*M);
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
    sphere.release();
  }

  if (wall) {
  program.setUniformValue("lw", -1.f);
  program.setUniformValue("main_color", 0.7, 0.6, 0.2);
  program.setUniformValue("DepthBiasMVP", VPdepth);
  program.setUniformValue("MVP", VP);
  program.setUniformValue("Mn", QMatrix4x4().normalMatrix());
  for (uint i = 0; i < system->walls.size(); ++i) {
    Wall* w = &system->walls[i];

    glBegin(GL_QUADS);
    program.setAttributeValue(1, w->n[0], w->n[1], w->n[2]);
    program.setAttributeValue(0, w->p[0], w->p[1], w->p[2]);
    program.setAttributeValue(0, w->q[0], w->q[1], w->q[2]);
    program.setAttributeValue(0, w->r[0], w->r[1], w->r[2]);
    program.setAttributeValue(0, w->s[0], w->s[1], w->s[2]);
    glEnd();
  }
  }
}

void View::drawShadow(const QMatrix4x4& VP)
{
  /*********************************************/
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glViewport(0, 0, 2048, 2048);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  program_rtt.bind();

  QVector3D light(-2, 2, 10);
  light.normalize();

  QMatrix4x4 depthVP;
  depthVP.ortho(-50, 50, -50, 50, 0, 1000);
  depthVP.lookAt(light * 100, QVector3D(0,0,0), QVector3D(0,1,0));

  glCullFace(GL_FRONT);
  draw(program_rtt, depthVP, QMatrix4x4(), false);

  /*********************************************/
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  program.bind();
  program.setUniformValue("l", light);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  program.setUniformValue("shadowMap", 0);

  QMatrix4x4 bias(0.5, 0.0, 0.0, 0.5,
                  0.0, 0.5, 0.0, 0.5,
                  0.0, 0.0, 0.5, 0.5,
                  0.0, 0.0, 0.0, 1.0);

  glCullFace(GL_BACK);
  draw(program, VP, bias * depthVP);
}

void View::timerEvent(QTimerEvent*)
{
  double dt = time.restart() / 1000.;
  system->simulate(dt);

  updateGL();
}
