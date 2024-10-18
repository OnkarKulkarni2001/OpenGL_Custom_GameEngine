#pragma once
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

class IPlayer {
public:
	virtual void MoveForward() = 0;
	virtual void MoveBackward() = 0;
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
	virtual void Jump() = 0;
	virtual void Fire() = 0;
	virtual void DecreaseHealth(float value) = 0;
	virtual void IncreaseHealth(float value) = 0;
	virtual bool IsDead() = 0;

	// Getters
	virtual double GetHealth() = 0;
	virtual glm::vec3 GetLocation() = 0;
	virtual void GetWeapon() = 0;
	virtual float GetSpeed() = 0;
	virtual glm::vec3 GetForwardVector() = 0;
	virtual glm::vec3 GetRightVector() = 0;
	virtual glm::vec3 GetUpVector() = 0;								// Maybe for jumping!

	// Setters
	virtual void SetHealth(float value) = 0;
	virtual void SetLocation(float x, float y, float z) = 0;
	virtual void SetWeapon() = 0;									// Will need some struct to pass to this function!
	virtual void SetSpeed(float value) = 0;
	virtual void SetForwardVector(float x, float y, float z) = 0;
	virtual void SetRightVector(float x, float y, float z) = 0;
	virtual void SetUpVector(float x, float y, float z) = 0;
};