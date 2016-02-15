#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

#include "Mesh.hpp"
#include "Entity.hpp"

class RigidBody
{
public:
	
	Entity* entity;

	//AABB aabb;
	//BoundingSphere sphere;

	glm::vec3 linearMomentum;
	glm::vec3 angularMomentum;

	glm::vec3 force;
	glm::vec3 torque;

	glm::mat3 inverseI;

	float mass;

	RigidBody(Entity* entity);

	glm::vec3 GetLinearVelocity() const { return linearMomentum * 1.0f/mass; }
	glm::vec3 GetAngularVelocity() const { return (glm::toMat3(entity->orientation) * inverseI * glm::transpose(glm::toMat3(entity->orientation))) * 
		angularMomentum; }

	/*inline void ApplyGravity(const float g) 
	{ 
		if(massInverse > 0) 
			force += glm::vec3(0, 1.0f/massInverse, 0) * g; 
	}*/

	void ApplyImpulse(const glm::vec3 &point, const glm::vec3 &f);
	void Update(float deltaTime);

protected:

	
};

#endif