#version 330 core

in vec3 transformedNormal;
in vec3 EyeDir;

uniform samplerCube cubeMap;

out vec4 color;

 void main(void)
 {
    vec3 reflectedDirection = -normalize(reflect(-EyeDir, transformedNormal));
    color = texture(cubeMap, reflectedDirection);
    //color = fragColor;
	//color = vec4(1.0f,0,0,1.0f);
	//color = vec4(reflectedDirection.xyz, 1.0f);
	//color = vec4(transformedNormal.xyz, 1.0f);
	//color = gl_FragCoord * 0.01f;
}