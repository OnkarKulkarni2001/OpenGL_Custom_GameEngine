#pragma once
#include "IPlayer.h"
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

class cPlayer : public IPlayer {
public:
	void MoveForward(float value) override;
	void MoveBackward(float value) override;
	void MoveLeft(float value) override;
	void MoveRight(float value) override;
	void Fire() override;
	void DecreaseHealth(float value) override;
	void IncreaseHealth(float value) override;
	bool IsDead() override;

	// Getters
	double GetPlayerHealth() override;
	glm::vec3 GetPlayerLocation() override;
	void GetPlayerWeapon() override;
	float GetPlayerSpeed() override;
	glm::vec3 GetPlayerForwardVector() override;
	glm::vec3 GetPlayerRightVector() override;
	glm::vec3 GetPlayerUpVector() override;

	// Setters
	void SetPlayerHealth(float value) override;
	void SetPlayerLocation(float x, float y, float z) override;
	void SetPlayerWeapon() override;									// Will need some struct to pass to this function!
	void SetPlayerSpeed(float value) override;
	void SetPlayerForwardVector(float x, float y, float z) override;
	void SetPlayerRightVector(float x, float y, float z) override;
	void SetPlayerUpVector(float x, float y, float z) override;

private:
	float playerHealth;
	float playerSpeed;

	glm::vec3 playerLocation;
	glm::vec3 playerForwardVector;		// will be looking in z direction
	glm::vec3 playerRightVector;		// will be in x direction
	glm::vec3 playerUpVector;			// will be in y direction

	//TODO: Add player weapon
};