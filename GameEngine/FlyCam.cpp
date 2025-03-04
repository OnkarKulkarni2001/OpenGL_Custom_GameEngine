#include "FlyCam.h"

FlyCam::FlyCam(int camWidth, int camHeight, glm::vec3 camLocation, float yawAngle) {
    this->camWidth = camWidth;
    this->camHeight = camHeight;
    this->camLocation = camLocation;
    this->camYaw = yawAngle;
}

void FlyCam::cameraMatrix(float FOVdeg, float nearPlane, float farPlane, GLuint shaderProgram, const char* uniform, GLFWwindow* window) {
    glm::mat4 view = glm::lookAt(camLocation, camLocation + camForwardVector, camUpVector);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)camWidth / camHeight, nearPlane, farPlane);
    // Send the camera matrix (view * projection) to the shader
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniform);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection * view));
    glUniform3fv(glGetUniformLocation(shaderProgram, "camLocation"), 1, glm::value_ptr(camLocation));

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        camControls(window);
    }
}

void FlyCam::camControls(GLFWwindow* window) {

    // Handle WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camLocation += camSpeed * camForwardVector; // Move forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camLocation -= camSpeed * camForwardVector; // Move backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camLocation -= camSpeed * camRightVector; // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camLocation += camSpeed * camRightVector; // Move right
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camLocation += camSpeed * camUpVector; // Move up
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camLocation -= camSpeed * camUpVector; // Move down
    }

    // Handle mouse input for camera rotation
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstClick) {
        glfwSetCursorPos(window, camWidth / 2, camHeight / 2); // Center the cursor
        firstClick = false;
    }

    float rotX = camSensi * (float)(mouseX - camWidth / 2) / camWidth;
    float rotY = camSensi * (float)(mouseY - camHeight / 2) / camHeight;

    // Update pitch and yaw
    camPitch += rotY;
    camYaw += rotX;

    // Limit the pitch
    if (camPitch > 89.0f)
        camPitch = 89.0f;
    if (camPitch < -89.0f)
        camPitch = -89.0f;

    // Handle roll input (e.g., Q/E keys for rolling)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camRoll += camSpeed * 5.0f; // Adjust roll speed as needed
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camRoll -= camSpeed * 5.0f;
    }

    // Calculate the new forward vector
    camForwardVector.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
    camForwardVector.y = sin(glm::radians(camPitch));
    camForwardVector.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
    camForwardVector = glm::normalize(camForwardVector);

    // Calculate the new right and up vectors, including roll
    glm::vec3 right = glm::normalize(glm::cross(camForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, camForwardVector));

    // Apply the roll transformation
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camRoll), camForwardVector);
    camRightVector = glm::mat3(rollMatrix) * right;
    camUpVector = glm::mat3(rollMatrix) * up;

    // Reset cursor position to the center
    glfwSetCursorPos(window, camWidth / 2, camHeight / 2);
}
