#ifndef GLSPHERE_HH
#define GLSPHERE_HH

#include <QGLBuffer>
#include <QGLShaderProgram>

class GLSphere
{
public:
  GLSphere();

  void initialize();
  void initialize(GLint slices , GLint stacks);

  void draw(QGLShaderProgram& program, int attributeLocation);

  void bind();
  void release();
  void drawElements();

private:
  QGLBuffer vbo, ibo;
  GLuint vbo_sz;
  GLuint ibo_sz[3];
};

#endif // GLSPHERE_HH
