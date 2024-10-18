#pragma once
class IPlayer {
public:
	virtual void MoveForward(float value) = 0;
	virtual void MoveBackward(float value) = 0;
	virtual void MoveLeft(float value) = 0;
	virtual void MoveRight(float value) = 0;
	virtual void Fire() = 0;
	virtual void DecreaseHealth(float value) = 0;
	virtual void IncreaseHealth(float value) = 0;
	virtual bool IsDead() = 0;

	// Getters
	virtual double GetPlayerHealth() = 0;
	virtual glm::vec3 GetPlayerLocation() = 0;
	virtual void GetPlayerWeapon() = 0;
	virtual float GetPlayerSpeed() = 0;
	virtual glm::vec3 GetPlayerForwardVector() = 0;
	virtual glm::vec3 GetPlayerRightVector() = 0;
	virtual glm::vec3 GetPlayerUpVector() = 0;								// Maybe for jumping!

	// Setters
	virtual void SetPlayerHealth(float value) = 0;
	virtual void SetPlayerLocation(float x, float y, float z) = 0;
	virtual void SetPlayerWeapon() = 0;									// Will need some struct to pass to this function!
	virtual void SetPlayerSpeed(float value) = 0;
	virtual void SetPlayerForwardVector(float x, float y, float z) = 0;
	virtual void SetPlayerRightVector(float x, float y, float z) = 0;
	virtual void SetPlayerUpVector(float x, float y, float z) = 0;
};