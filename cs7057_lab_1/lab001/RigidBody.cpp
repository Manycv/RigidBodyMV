#include "RigidBody.h"

#include "Entity.hpp"

#include "glm\gtx\transform.hpp"
#include "glm\gtx\quaternion.hpp"

#include <algorithm>

using namespace glm;
using namespace std;

RigidBody::RigidBody(Entity* e)
{
	entity = e;

	linearMomentum = vec3();
	force = vec3();

	angularMomentum = vec3();
	torque = vec3();

	glm::mat3 I = mass/12.0f * mat3(
		vec3(1, 0, 0),
		vec3(0, 1, 0),
		vec3(0, 0, 1)
		);
	inverseI = glm::inverse(I);

	mass = 1;


	//create AABB (e->vertices)
	//create sphere (e->vertices)
}

void RigidBody::ApplyImpulse(const glm::vec3 &point, const glm::vec3 &f)
{
	force += f;
	torque += glm::cross(point - entity->position, f);
	//std::cout << "kasfd";
}

void RigidBody::Update(float deltaTime)
{	
	// linear
	linearMomentum += force * deltaTime;
	//entity->position += (linearMomentum * (1.0f/mass))  * deltaTime;

	force = vec3();

	// angular
	mat3 currentInverseI = glm::toMat3(entity->orientation) * inverseI * glm::transpose(glm::toMat3(entity->orientation));

	angularMomentum += torque * deltaTime;
	entity->orientation = normalize(toQuat(currentInverseI * toMat3(quat(angularMomentum * deltaTime))) * entity->orientation); //this line wrong

	std::stringstream ss;
	ss << "Ang momentum: ";
	ss << angularMomentum.x << ", " << angularMomentum.y << ", " << angularMomentum.z << "\n";
	std::cout << ss.str();

	torque = vec3();
}

