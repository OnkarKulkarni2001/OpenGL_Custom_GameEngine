#include "cPlayer.h"

void cPlayer::MoveForward(float value)
{
    playerLocation += value * playerForwardVector;
}

void cPlayer::MoveBackward(float value)
{
    playerLocation -= value * playerForwardVector;
}

void cPlayer::MoveLeft(float value)
{
    playerLocation -= value * playerRightVector;
}

void cPlayer::MoveRight(float value)
{
    playerLocation += value * playerRightVector;
}

void cPlayer::Fire()
{
    // TODO: Implement firelogic
}

void cPlayer::DecreaseHealth(float value)
{
    playerHealth -= value;
}

void cPlayer::IncreaseHealth(float value)
{
    playerHealth += value;
}


// Getters
double cPlayer::GetPlayerHealth()
{
    return playerHealth;
}

glm::vec3 cPlayer::GetPlayerLocation()
{
    return playerLocation;
}

void cPlayer::GetPlayerWeapon()
{
    // TODO: Implement struct
}

float cPlayer::GetPlayerSpeed()
{
    return playerSpeed;
}

glm::vec3 cPlayer::GetPlayerForwardVector()
{
    return playerForwardVector;
}

glm::vec3 cPlayer::GetPlayerRightVector()
{
    return playerRightVector;
}

glm::vec3 cPlayer::GetPlayerUpVector()
{
    return playerUpVector;
}


// Setters
void cPlayer::SetPlayerHealth(float value)
{
    playerHealth = value;
}

void cPlayer::SetPlayerLocation(float x, float y, float z)
{
    playerLocation = glm::vec3(x, y, z);
}

void cPlayer::SetPlayerWeapon()
{
    // TODO: Implement struct for this
}

void cPlayer::SetPlayerSpeed(float value)
{
    playerSpeed = value;
}

void cPlayer::SetPlayerForwardVector(float x, float y, float z)
{
    playerForwardVector = glm::vec3(x, y, z);
}

void cPlayer::SetPlayerRightVector(float x, float y, float z)
{
    playerRightVector = glm::vec3(x, y, z);
}

void cPlayer::SetPlayerUpVector(float x, float y, float z)
{
    playerUpVector = glm::vec3(x, y, z);
}

bool cPlayer::IsDead()
{
    return false;
}
