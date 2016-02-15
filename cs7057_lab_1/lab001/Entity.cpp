#include "Entity.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Entity::Entity(glm::vec3 startPos)
{
	position = startPos;
	orientation = glm::quat();
}

bool Entity::loadFromFile(std::string file)
{
	ModelImporter* importer = new ModelImporter();
	importer->importModel(file);
	this->meshes = importer->getMeshes();
	return true;
}
	
void Entity::draw()
{
	/* If this entity has a shader, use it */
	if (this->_shader != NULL)
	{
		this->_shader->enableShader();
		this->_shader->setUniformMatrix4fv("modelMat", getModelMat());
	}

	for(int i = 0; i < this->meshes.size(); ++i)
	{
		this->meshes[i]->renderGL();
	}
}
	
glm::mat4 Entity::getModelMat()
{
	return glm::translate(glm::mat4(1), position) * glm::toMat4(orientation);
}

void Entity::setShader(Shader* s)
{
	this->_shader = s;
}

void Entity::setShader(std::string shader)
{
	Shader* s = ShaderManager::getShader(shader);
	if (s != NULL)
	{
		this->setShader(s);
	}
}

Shader* Entity::getShader()
{
	return this->_shader;
}
