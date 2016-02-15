#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "SOIL.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderManager.hpp"
#include "Entity.hpp"
#include "CubeMap.hpp"
#include "maths_funcs.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "RigidBody.h"



//mat4 view_mat;
glm::mat4 viewMatrix;
glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
//void do_rotation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;



// Camera settings
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  5.0f); //camera position in world space
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //defines the target
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f); //represents the up vector in world space

//glm::vec3 camera = glm::cross(cameraFront, cameraUp);


GLfloat yaw    = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch  =  0.0f;
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
GLfloat fov =  45.0f;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame



GLFWwindow* window;



std::vector<RigidBody*> rigidBodies;

bool initWindow()
{
	if (!glfwInit())
	{
		std::cout << "ERROR: Could not initialise GLFW...";
		std::cin;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 ", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "ERROR: Could not create winodw...";
		std::cin;
		return false;
	}

	glfwMakeContextCurrent(window);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//Set the required callback functions
   glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "ERROR: Problem initialising GLEW: " << glewGetErrorString(err);
		std::cin;
		return false;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return true;
}

int main()
{
	/* Create GL Window */
	if (!initWindow())
		return -1;

	/* Initialise vertex buffers for cube */
	CubeMap cubeMap;
	cubeMap.loadCubeMap("../textures/cubemaps/Yokohama/");


	/* Load shaders needed */

	Entity* box = new Entity(glm::vec3(0));
	box->setShader(ShaderManager::loadShader("simpleReflect"));
	box->loadFromFile("../models/Cube.3ds");
	rigidBodies.push_back(new RigidBody(box));

	box = new Entity(glm::vec3(0,2,0));
	box->setShader(ShaderManager::loadShader("simpleReflect"));
	box->loadFromFile("../models/Cube.3ds");
	rigidBodies.push_back(new RigidBody(box));

	std::cout << "some amazing feature has been added";


	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		/* Rendering Code */

		// Calculate deltatime of current frame
       GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
       do_movement();
		//do_rotation();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm::mat4 viewMatrix = glm::lookAt (
		cameraPos,  //where you are
		cameraPos + cameraFront, // what you are looking at, however we move, the camera keeps looking at the target direction
		cameraUp 
		);

	

		glm::mat4 projectionMatrix = glm::perspective(
		fov,
		(GLfloat)WIDTH/(GLfloat)HEIGHT,
		0.1f,
		1000.0f
		);


		for(int i =0; i < rigidBodies.size(); i++)
		{
			//for(int j = 0; j < rigidBodies.size(); j++)
			//{
			//	if (i ==j)
			//		continue;

			//	//if(rigidBodies[i]->aabb->collies(rigidBodies[j]->aabb))
			//		//do something with shaders to indicate collision in broadphase
			//}


			rigidBodies[i]->Update(deltaTime);
		
			rigidBodies[i]->entity->getShader()->setUniformMatrix4fv("projectionMat", projectionMatrix);
			//cubeMap.getShader()->setUniformMatrix4fv("projectionMat", projectionMatrix);
			rigidBodies[i]->entity->getShader()->setUniformMatrix4fv("viewMat", viewMatrix);
			//cubeMap.getShader()->setUniformMatrix4fv("viewMat", viewMatrix);
			//dragonModel.getShader()->setUniformMatrix4fv("viewMat", view_mat);
			rigidBodies[i]->entity->getShader()->setUniformVector4fv("camPos", glm::vec4(cameraPos, 1.0f));
			//glm:: vec3 cam_pos= glm::vec3 (cam_pos);
			//dragonModel.getShader()->setUniformVector4fv("camPos", glm::vec4(cam_pos, 1.0f));

			//rigidBodies[i]->entity->getShader()->setUniformVector4fv("modelMat", rigidBodies[i]->entity->getModelMat());
		
			rigidBodies[i]->entity->draw();

			//rigidBodies[i]->ApplyGravity();
		}
		
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);

		
		//cubeMap.drawSkyBox();

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);

		glfwSwapBuffers(window);
		glfwPollEvents();

		

}
	glfwTerminate();
	return 0;

}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
	
	if (keys[GLFW_KEY_F])
		rigidBodies[0]->ApplyImpulse(glm::vec3(1,0,0), glm::vec3(0,0,-10));
   
	



	if (keys[GLFW_KEY_UP])
		Rotation += glm::vec3(0.1f, 0.0f, 0.0f); //x
	/*else if(action == GLFW_RELEASE)
		Rotation = glm::vec3(0.0f, 0.0f, 0.0f);*/
	if (keys[GLFW_KEY_DOWN])
		Rotation += glm::vec3(-0.1f, 0.0f, 0.0f); //-x
	if (keys[GLFW_KEY_LEFT])
		Rotation += glm::vec3(0.0f, 0.1f, 0.0f); //y
	if (keys[GLFW_KEY_RIGHT])
		Rotation += glm::vec3(0.0f, -0.1f, 0.0f); //-y
	if (keys[GLFW_KEY_C])
		Rotation += glm::vec3(0.0f, 0.0f, 0.1f); //z
	if (keys[GLFW_KEY_Z])
		Rotation += glm::vec3(0.0f, 0.0f, -0.1f); //-z
	
 


	/*if (keys[GLFW_KEY_UP])
		Rotation = glm::vec3(0.01f, 0.0f, 0.0f);
	else if(action == GLFW_RELEASE)
		Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_DOWN])
		Rotation = glm::vec3(-0.01f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_LEFT])
		Rotation = glm::vec3(0.0f, 0.01f, 0.0f);
	if (keys[GLFW_KEY_RIGHT])
		Rotation = glm::vec3(0.0f, -0.01f, 0.0f);
	if (keys[GLFW_KEY_C])
		Rotation = glm::vec3(0.0f, 0.0f, 0.01f); 
	if (keys[GLFW_KEY_Z])
		Rotation = glm::vec3(0.0f, 0.0f, -0.01f); */
	
}


void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_Q])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_E])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_W])
        cameraPos += glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp),cameraFront)) * cameraSpeed;
    if (keys[GLFW_KEY_S])
        cameraPos -= glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp),cameraFront)) * cameraSpeed;
}



bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.03;	// Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}



