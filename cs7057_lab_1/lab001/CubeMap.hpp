#pragma once

#include <IL\il.h>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <GL\glew.h>
#include <GL\GL.h>


#include "ShaderManager.hpp"

class CubeMap{
private:

	GLuint texID;
	GLuint vbo; // skybox
	GLuint vao; // skybox

	glm::mat4 _modelMatrix;

	Shader* _shader;


	//static ILuint* imageIDs;
	//static GLuint* textureIDs;
	void create_cube_map (
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint* tex_cube);

	bool CubeMap::load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name);

public:
	CubeMap();

	void loadCubeMap(const char* mapFolder);
	void drawSkyBox();
	void use();
	glm::mat4 getModelMat();

	void setShader(Shader* s);
	void setShader(std::string shader);
	Shader* getShader();
};
