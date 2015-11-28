//
//	gls2.h (Shape rendering functions in OpenGL) 
//
//	September, 2015
//

#pragma once

#include <windows.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <sstream>
#include <queue>
#include <map>
#include <set>

#include <iostream>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <LearnOpenGL/shader.h>

struct Vertex	// vertex format (e.g. PNT)
{
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
    // Tangent
    glm::vec3 Tangent;
    // Bitangent
    glm::vec3 Bitangent;
};

struct Texture 
{
    GLuint id;
    string type;
    //aiString path;
};

class CMesh 
{
public:
    //  CMesh Data  
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    GLuint VAO;
	bool hasMesh;

    /*  Functions  */
    // Constructor
	CMesh();
	CMesh(vector<Vertex> vertices, vector<GLuint> indices, unsigned int vpf = 3, bool setVBO = true);
    CMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
 
	// Delete VAO, VBO, and EBO from GPU
	void Delete();

	// Reset mesh
	void LoadVAO();

    // Render the mesh
    void Draw(); 
   

private:
    /*  Render data  */
    GLuint VBO, EBO;

	unsigned int verticesPerFace;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh();
};

//======================================================================================
//
//	Box functions
//
//======================================================================================

CMesh gls2Box (glm::vec3 dimension, glm::vec3 pos, unsigned int vpf = 3);
CMesh gls2OpenBox (glm::vec3 dimension, glm::vec3 pos, unsigned int vpf = 3);
CMesh gls2OpenBox2 (glm::vec3 dimension, glm::vec3 pos, unsigned int vpf);

CMesh gls2Dice(glm::vec3 dimension, glm::vec3 pos);
CMesh gls2Dice();
CMesh gls2Rect(float z, float _dx, float _dy, int _nx, int _ny);


CMesh gls2Sphere (glm::vec3 center, float radius, int n, int m);
CMesh gls2Torus (float innerR, float outerR, int rings = 16, int sides = 16);
CMesh gls2Sphere(float radius, int gridX = 16, int gridY = 16);
CMesh gls2Cylinder(float radius, float height, int sides = 16, bool top = true, bool bottom = true);
CMesh gls2Cone (float height, float radius, int sides = 16, bool bottom = true);







