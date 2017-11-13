#pragma once

#include "OGLRenderer.h"

 enum MeshBuffer {
	 VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, MAX_BUFFER

};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);
	virtual void Draw();
	static Mesh * GenerateTriangle();
	void SetTexture(GLuint tex, int pos) { texture[pos] = tex; }
	GLuint GetTexture(int pos) { return texture[pos]; }
	static Mesh * GenerateQuad();
protected:
	void GenerateNormals();
	GLuint texture[3];
	Vector2 * textureCoords;
	void BufferData();
	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	GLuint numIndices;
	unsigned int* indices;
	Vector3 * vertices;
	Vector4 * colours;
	Vector3* normals;
};
