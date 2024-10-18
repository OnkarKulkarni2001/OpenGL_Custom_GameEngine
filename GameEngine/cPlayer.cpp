#include "cPlayer.h"

void cPlayer::MoveForward()
{
    playerLocation += playerSpeed * playerForwardVector;
}

void cPlayer::MoveBackward()
{
    playerLocation -= playerSpeed * playerForwardVector;
}

void cPlayer::MoveLeft()
{
    playerLocation -= playerSpeed * playerRightVector;
}

void cPlayer::MoveRight()
{
    playerLocation += playerSpeed * playerRightVector;
}

void cPlayer::Jump()
{
    // TODO
}

void cPlayer::Fire()
{
    // TODO: Implement fire logic
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
double cPlayer::GetHealth()
{
    return playerHealth;
}

glm::vec3 cPlayer::GetLocation()
{
    return playerLocation;
}

void cPlayer::GetWeapon()
{
    // TODO: Implement struct
}

float cPlayer::GetSpeed()
{
    return playerSpeed;
}

glm::vec3 cPlayer::GetForwardVector()
{
    return playerForwardVector;
}

glm::vec3 cPlayer::GetRightVector()
{
    return playerRightVector;
}

glm::vec3 cPlayer::GetUpVector()
{
    return playerUpVector;
}


// Setters
void cPlayer::SetHealth(float value)
{
    playerHealth = value;
}

void cPlayer::SetLocation(float x, float y, float z)
{
    playerLocation = glm::vec3(x, y, z);
}

void cPlayer::SetWeapon()
{
    // TODO: Implement struct for this
}

void cPlayer::SetSpeed(float value)
{
    playerSpeed = value;
}

void cPlayer::SetForwardVector(float x, float y, float z)
{
    playerForwardVector = glm::vec3(x, y, z);
}

void cPlayer::SetRightVector(float x, float y, float z)
{
    playerRightVector = glm::vec3(x, y, z);
}

void cPlayer::SetUpVector(float x, float y, float z)
{
    playerUpVector = glm::vec3(x, y, z);
}

bool cPlayer::IsDead()
{
    if (playerHealth <= 0)
        return true;
    return false;
}