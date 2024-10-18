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