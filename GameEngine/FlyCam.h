#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <conio.h>
#include "cScene.h"

class FlyCam {
public:
	/*glm::vec3 camLocation;
	glm::vec3 camOrientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 camUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick;

	int camWidth;
	int camHeight;

	float camSpeed = 0.1f;
	float camSensi = 1.0f;

	FlyCam(int camWidth, int camHeight, glm::vec3 camLocation);

	void cameraMatrix(float FOVdeg, float nearPlane, float farPlane, GLuint shaderProgram, const char* uniform);
	void camControls(GLFWwindow* window);*/
	
	glm::vec3 camLocation;

	glm::vec3 camForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 camRightVector = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 camUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

	float camSpeed = 0.1f;
	float camSensi = 1.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
	float camRoll = 0.0f;

	int camWidth;
	int camHeight;

	FlyCam(int camWidth, int camHeight, glm::vec3 camLocation, float yawAngle);

	void cameraMatrix(float FOVdeg, float nearPlane, float farPlane, GLuint shaderProgram, const char* uniform, GLFWwindow* window);
	void camControls(GLFWwindow* window);
};

#endif