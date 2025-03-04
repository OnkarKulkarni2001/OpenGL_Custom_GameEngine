#include "sMesh.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

//static 
unsigned int sMesh::m_NextUniqueID = 100;	// Start unique IDs at 100


sMesh::sMesh()
{
	//for (unsigned int index = 0; index < MAX_NUM_TEXTURES; index++)
	//{
	//	this->blendRatio[MAX_NUM_TEXTURES] = 0.0f;
	//}
	this->blendRatio[0] = 1.0f;
	this->blendRatio[1] = 0.0f;
	this->blendRatio[2] = 0.0f;
	this->blendRatio[3] = 0.0f;
	this->blendRatio[4] = 0.0f;
	this->blendRatio[5] = 0.0f;
	this->blendRatio[6] = 0.0f;
	this->blendRatio[7] = 0.0f;

	this->positionXYZ = glm::vec3(0.0f);
	this->rotationEulerXYZ = glm::vec3(0.0f);
	this->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Assign a unique ID for each mesh
	this->m_uniqueID = sMesh::m_NextUniqueID;
	sMesh::m_NextUniqueID++;
}

unsigned int sMesh::getUniqueID(void)
{
	return this->m_uniqueID;
}

std::string sMesh::getState(void)
{
	// TODO: Make string representing state
	return "";
}

bool sMesh::loadState(std::string newState)
{
	// TODO: Take a string and decode state
	return true;
}

// This was taken from the DrawMesh() call:
glm::mat4 sMesh::calcMatModel(void)
{
	glm::mat4 matModel = glm::mat4(1.0f);

	// Translation (movement, position, placement...)
	glm::mat4 matTranslate
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(this->positionXYZ.x,
				this->positionXYZ.y,
				this->positionXYZ.z));

	// Rotation...
	// Caculate 3 Euler acix matrices...
	glm::mat4 matRotateX =
		glm::rotate(glm::mat4(1.0f),
			glm::radians(this->rotationEulerXYZ.x), // Angle in radians
			glm::vec3(1.0f, 0.0, 0.0f));

	glm::mat4 matRotateY =
		glm::rotate(glm::mat4(1.0f),
			glm::radians(this->rotationEulerXYZ.y), // Angle in radians
			glm::vec3(0.0f, 1.0, 0.0f));

	glm::mat4 matRotateZ =
		glm::rotate(glm::mat4(1.0f),
			glm::radians(this->rotationEulerXYZ.z), // Angle in radians
			glm::vec3(0.0f, 0.0, 1.0f));


	// Scale
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		glm::vec3(this->uniformScale,
			this->uniformScale,
			this->uniformScale));


	// Calculate the final model/world matrix
	matModel *= matTranslate;     // matModel = matModel * matTranslate;
	matModel *= matRotateX;
	matModel *= matRotateY;
	matModel *= matRotateZ;
	matModel *= matScale;

	return matModel;
}
