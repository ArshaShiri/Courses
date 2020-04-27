#pragma once

#include <GL\glew.h>

class Mesh
{
public:
  Mesh();

  void createMesh(const GLfloat *vertices,
                  const unsigned int * indices,
                  const unsigned int numberOfVertices,
                  const unsigned int numberOfIndices);
  void renderMesh() const;
  void clearMesh();

  ~Mesh();

private:
  GLuint VAO_;
  GLuint VBO_;
  GLuint IBO_;
  GLsizei indexCount_;
};

