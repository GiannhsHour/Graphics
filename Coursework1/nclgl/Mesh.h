#pragma once
#include "OGLRenderer.h"
/*
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, MAX_BUFFER
};
*/

// texture
/*enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, MAX_BUFFER
};
*/

/* Index Buffer
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER,
	TEXTURE_BUFFER, INDEX_BUFFER,
	MAX_BUFFER
};
*/
/* Lighting A
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER,
	NORMAL_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
*/

// lighting B
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER,
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, // Tangents !
	MAX_BUFFER
};

class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw(bool texture0 = true);
	static Mesh * GenerateTriangle();

	// textures
	void SetTexture(GLuint tex, int pos) { texture[pos] = tex; }
	GLuint GetTexture(int pos)			{ return texture[pos]; }

	// transparency
	static Mesh* GenerateQuad();

	// lighting B
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }

	void GenerateNormals();
	int getNumVertices() { return numVertices; }


protected:
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3 * vertices;
	Vector4 * colours;

	// textures
	GLuint texture[5];
	Vector2* textureCoords;

	// Index buffer
	GLuint numIndices;
	unsigned int *indices;

	// lighting A
	
	Vector3* normals;

	// lighting B
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 & ta,
		const Vector2 & tb, const Vector2 & tc);
	
	Vector3* tangents;
	GLuint bumpTexture;


};