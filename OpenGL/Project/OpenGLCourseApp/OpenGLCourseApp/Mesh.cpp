#include "Mesh.h"



Mesh::Mesh() : VAO_{0}, VBO_{0}, IBO_{0}, indexCount_{0}
{}

void Mesh::createMesh(const GLfloat *vertices,
                      const unsigned int * indices,
                      const unsigned int numberOfVertices,
                      const unsigned int numberOfIndices)
{
  indexCount_ = numberOfIndices;

  // Creating a vertex array on the graphic card.
  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);

  // Index buffer object.
  glGenBuffers(1, &IBO_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

  // vertex buffer objects (VBO). Memory created on GPU for vertices.
  // Sending data to the graphics card from the CPU is relatively slow, so wherever we can we try
  // to send as much data as possible at once.
  // id = 1
  glGenBuffers(1, &VBO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
  // GL_STATIC_DRAW : the data is set only once and used many times.
  // GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
  // Copies the vertices data to the associated buffer.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

  // Take a look at the shader: location: 0
  // The vertex shader allows us to specify any input we want in the form of vertex attributes and 
  // while this allows for great flexibility, it does mean we have to manually specify what part of 
  // our input data goes to which vertex attribute in the vertex shader.This means we have to
  // specify how OpenGL should interpret the vertex data before rendering.

  // Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its
  // data from(you can have multiple VBOs) is determined by the VBO currently bound to 
  // GL_ARRAY_BUFFER when calling glVertexAttribPointer.Since the previously defined VBO is still
  // bound before calling glVertexAttribPointer vertex attribute 0 is now associated with its
  // vertex data.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // Unbinding.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::renderMesh()
{
  // Every shader and rendering call after glUseProgram will now use this program object 
  // (and thus the shaders).
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);

  // Draw by element ids
  // indexCount_ indices.
  glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::clearMesh()
{
  if (IBO_ != 0)
  {
    glDeleteBuffers(1, &IBO_);
    IBO_ = 0;
  }

  if (VBO_ != 0)
  {
    glDeleteBuffers(1, &VBO_);
    VBO_ = 0;
  }

  if (VAO_ != 0)
  {
    glDeleteVertexArrays(1, &VAO_);
    VAO_ = 0;
  }

  indexCount_ = 0;
}

Mesh::~Mesh()
{
  clearMesh();
}
