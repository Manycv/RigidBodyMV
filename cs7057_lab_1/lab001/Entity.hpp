#ifndef SGE_ENTITY_HPP
#define SGE_ENTITY_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"

#include "Mesh.hpp"
#include "ModelImporter.hpp"
#include "ShaderManager.hpp"

class Entity
{
private:
	std::vector<Mesh*> meshes;
	Shader* _shader;

public:

	glm::vec3 position;
	glm::quat orientation;
	//scale

	Entity(glm::vec3 startPos);
		
	bool loadFromFile(std::string file);
	void draw();
	glm::mat4 getModelMat();

	void setShader(Shader* s);
	void setShader(std::string shader);
	Shader* getShader();
};

#endif
