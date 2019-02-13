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

	glGenVertexArrays(1, &vao);                                                                                           	// GENERATE AND BIND VERTEX ARRAY OBJECT
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);                                                                                                	// GENERATE AND BIND INDEXED BUFFER OBJECT
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);                    	// DEFINE IBO BUFFER DATA

	glGenBuffers(1, &vbo);                                                                                                	// GENERATE AND BIND VERTEX BUFFER OBJECT
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);                         	// DEFINE VBO BUFFER DATA

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);                                          	// DEFINE ATTRIBUTE POINTER FOR VERTICES
	glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));             // DEFINE ATTRIBUTE POINTER FOR TEXTURE COORDINATES
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));             // DEFINE ATTRIBUTE POINTER FOR NORMALS
  glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);                                                                                     	// UNBIND VERTEX ARRAY OBJECT AND VERTEX BUFFER OBJECT 
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);                                                                             	// UNBIND INDEXED BUFFER OBJECT AFTER UNBINDING VAO
}

void Mesh::RenderMesh() {
	glBindVertexArray(vao);                                                                                               	// BIND
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// // CALL THE DRAW FUNCTION
	//glDrawArrays(GL_TRIANGLES, 0, 3);                                                                                   	// BASIC DRAW CALL
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);                                                                 	// INDEXED DRAW CALL

	glBindVertexArray(0);                                                                                                 	// UNBIND 
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
