#include "Mesh.h"

// CONSTRUCTOR
Mesh::Mesh() {
  vao = 0;
  vbo = 0;
  ibo = 0;
  indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices) {
  indexCount = numOfIndices;

  // GENERATE AND BIND VERTEX ARRAY OBJECT
  glGenVertexArrays(1, &vao);                                                                                           	
  glBindVertexArray(vao);

  // GENERATE AND BIND INDEXED BUFFER OBJECT
  glGenBuffers(1, &ibo);                                                                                                	
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  // DEFINE IBO BUFFER DATA
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);                    	

  // GENERATE AND BIND VERTEX BUFFER OBJECT
  glGenBuffers(1, &vbo);                                                                                                	
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // DEFINE VBO BUFFER DATA
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);                         	

  // DEFINE ATTRIBUTE POINTER FOR VERTICES
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);                                          	
  glEnableVertexAttribArray(0);

  // DEFINE ATTRIBUTE POINTER FOR TEXTURE COORDINATES
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));             
  glEnableVertexAttribArray(1);

  // DEFINE ATTRIBUTE POINTER FOR NORMALS
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));             
  glEnableVertexAttribArray(2);

  // UNBIND VERTEX ARRAY OBJECT AND VERTEX BUFFER OBJECT 
  glBindBuffer(GL_ARRAY_BUFFER, 0);                                                                                     	
  glBindVertexArray(0);
  // UNBIND INDEXED BUFFER OBJECT AFTER UNBINDING VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);                                                                             	
}

void Mesh::RenderMesh() {
  // BIND
  glBindVertexArray(vao);                                                                                               	
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  // // CALL THE DRAW FUNCTION
  // BASIC DRAW CALL
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  // INDEXED DRAW CALL
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);                                                                 	

  // UNBIND
  glBindVertexArray(0);                                                                                                 	 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh() {
  if (ibo != 0) {
    glDeleteBuffers(1, &ibo);
    ibo = 0;
  }

  if (vbo != 0) {
    glDeleteBuffers(1, &vbo);
    vbo = 0;
  }

  if (vao != 0) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }

  indexCount = 0;
}

// DESTRUCTOR
Mesh::~Mesh() {
  ClearMesh();
}
