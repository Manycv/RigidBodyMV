#version 330 core

in vec3 position;
in vec3 normal;
in vec3 camPos;

out vec3 EyeDir;
out vec3 transformedPosition;
out vec3 transformedNormal;

uniform samplerCube cubeMap;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

//uniform vec4 camPos;

void main()
{	
	mat4 MVP = projectionMat * viewMat * modelMat;
	gl_Position = MVP * vec4(position.xyz, 1.0);
	
	
	mat3 mvMat3 = mat3( MVP );
	
	// find world space normal.
	transformedNormal = normalize( mvMat3 * normal ); 
	
	// find world space eye vector.
	EyeDir = normalize(gl_Position - vec4(camPos.xyz, 1.0f)).xyz;
}