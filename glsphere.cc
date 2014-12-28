#include <cmath>
#include "glsphere.hh"

GLSphere::GLSphere() :
  vbo(QGLBuffer::VertexBuffer),
  ibo(QGLBuffer::IndexBuffer)
{
}

void GLSphere::initialize()
{
  initialize(15, 15);
}

void GLSphere::initialize(GLint slices, GLint stacks)
{
  QVector<GLfloat> positions;

  QVector<GLuint> indices0;
  QVector<GLuint> indices1;
  QVector<GLuint> indices2;



  unsigned int size(2 + slices * (stacks - 1));

  positions.reserve(3 * size);

  double alpha(M_PI / double(stacks));
  double beta(2.0*M_PI / double(slices));

  positions << 0.0 << 0.0 << 1.0;

  for (int i(1); i < stacks; ++i) {
    for (int j(0); j < slices; ++j) {
      float r = sin(i*alpha);
      float z = cos(i*alpha);
      float y = sin(j*beta) * r;
      float x = cos(j*beta) * r;

      positions << x << y << z;
    }
  }

  positions << 0.0 << 0.0 << -1.0;

  indices0.reserve(slices+2);
  for (int i(0); i <= slices; ++i)
    indices0 << i;
  indices0 << 1;

  indices1.reserve((stacks-2) * 2 * (slices+1));
  for (int i(0); i < stacks-2; ++i) {
    for (int j = 0; j < slices; ++j) {
      indices1 << 1+i*slices+j;
      indices1 << 1+(i+1)*slices+j;
    }
    indices1 << 1+i*slices;
    indices1 << 1+(i+1)*slices;
  }

  indices2.reserve(slices+2);
  for (int i(1); i <= slices+1; ++i)
    indices2 << size-i;
  indices2 << size-2;

  vbo_sz = 3 * size * sizeof (GLfloat);
  vbo.create();
  vbo.bind();
  vbo.allocate(positions.constData(), vbo_sz);
  vbo.release();

  ibo_sz[0] = indices0.size() * sizeof (GLuint);
  ibo_sz[1] = indices1.size() * sizeof (GLuint);
  ibo_sz[2] = indices2.size() * sizeof (GLuint);
  ibo.create();
  ibo.bind();
  ibo.allocate(ibo_sz[0] + ibo_sz[1] + ibo_sz[2]);
  ibo.write(0, indices0.constData(), ibo_sz[0]);
  ibo.write(ibo_sz[0], indices1.constData(), ibo_sz[1]);
  ibo.write(ibo_sz[0] + ibo_sz[1], indices2.constData(), ibo_sz[2]);
  ibo.release();
}

void GLSphere::draw(QGLShaderProgram& program, int attributeLocation)
{
  bind();

  program.setAttributeBuffer(attributeLocation, GL_FLOAT, 0, 3);
  program.enableAttributeArray(attributeLocation);

  drawElements();

  program.disableAttributeArray(attributeLocation);

  release();
}

void GLSphere::bind()
{
  vbo.bind();
  ibo.bind();
}

void GLSphere::release()
{
  ibo.release();
  vbo.release();
}

#define BUFFER_OFFSET(a) ((char*)nullptr + (a))

void GLSphere::drawElements()
{
  glDrawElements(GL_TRIANGLE_FAN, ibo_sz[0] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_QUAD_STRIP,   ibo_sz[1] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(ibo_sz[0]));
  glDrawElements(GL_TRIANGLE_FAN, ibo_sz[2] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(ibo_sz[0] + ibo_sz[1]));
}
