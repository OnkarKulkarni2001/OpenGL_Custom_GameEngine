#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class cBasicFlyCamera
{
public:
	cBasicFlyCamera();

	void setEyeLocation(glm::vec3 newEyeLocation);
	void setEyeLocation(float newX, float newY, float newZ);

	glm::vec3 getEyeLocation(void);
	glm::vec3 getTargetLocation(void);
	// Normalized version of getTargetLocation()
	glm::vec3 getTargetRelativeToCamera(void);

	// Do we want an "up" vector as well?

	// We'll reference the yaw, pitch, and roll from airplanes:
	// https://howthingsfly.si.edu/sites/default/files/2020-06/pitch-roll-yaw_0.gif

	void moveForward(float distanceToMove);
	//void moveBack();	// or -ve for "forward"

	// Up is positive, down is negative
	void moveUpDown(float distanceToMove);
	// Left is negative, right is positive
	void moveLeftRight(float distanceToMove);

	// +ve goes "faster", -ve goes slower
	// We'll get this from the mouse wheel (which passes in an integer)
	void adjustMovementSpeed(float adjustment);
	float getMovementSpeed(void);

	// negative (-ve) is "turning to the left"
	// positive (+ve) is "turning to the right"
	void rotateLeftRight_Yaw(float yAngleAdjust);
	// Sets the angle with no "movement speed" scaling
	void rotateLeftRight_Yaw_NoScaling(float yAngleAdjust);

	// positive (+ve) is "looking up"
	// negative (-ve) is "looking down"
	void pitchUpDown(float xAngleAdjust);

private:
	float m_Yaw_Y_axis_rotation;
	float m_Pitch_X_axis_rotation;

	glm::vec3 m_eye;
	glm::vec3 m_target;

	// These can be adjusted to move the camera faster
	float m_movementSpeed;
	float m_turnSpeedScaling;


	// Assume that the "front" of the camera is facing positive (+ve) along the z.
	// So if the eye is at (0, 0, 0), then 
	//	the target ("at) is at (0, 0, +1)

	const glm::vec3 m_FRONT_OF_CAMERA =      glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 m_RIGHT_SIDE_OF_CAMERA = glm::vec3(1.0f, 0.0f, 0.0f);

	const float MAX_MOVEMENT_SPEED = 100.0f;
	const float MIN_MOVEMENT_SPEED = 0.01f;

};
