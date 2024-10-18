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
#include "cLoadModels.h"

class cAiEnemy : public IPlayer {
public:
	cLoadModels* enemyModel;
	
	cAiEnemy(cLoadModels& enemyModel);
	~cAiEnemy();

	void MoveForward() override;
	void MoveBackward() override;
	void MoveLeft() override;
	void MoveRight() override;
	void Jump() override;
	void Fire() override;
	void DecreaseHealth(float value) override;
	void IncreaseHealth(float value) override;
	bool IsDead() override;

	// Getters
	double GetHealth() override;
	glm::vec3 GetLocation() override;
	void GetWeapon() override;
	float GetSpeed() override;
	glm::vec3 GetForwardVector() override;
	glm::vec3 GetRightVector() override;
	glm::vec3 GetUpVector() override;

	// Setters
	void SetHealth(float value) override;
	void SetLocation(float x, float y, float z) override;
	void SetWeapon() override;									// Will need some struct to pass to this function!
	void SetSpeed(float value) override;
	void SetForwardVector(float x, float y, float z) override;
	void SetRightVector(float x, float y, float z) override;
	void SetUpVector(float x, float y, float z) override;

private:
	float enemyHealth;
	float enemySpeed;

	glm::vec3 enemyLocation;
	glm::vec3 enemyForwardVector;		// will be looking in z direction
	glm::vec3 enemyRightVector;		// will be in x direction
	glm::vec3 enemyUpVector;			// will be in y direction

	//TODO: Add player weapon
};