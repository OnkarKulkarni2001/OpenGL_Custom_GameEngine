#pragma once
#include <BMP/BMP_ReaderWriter.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <string>
#include "cLoadModels.h"

class cTextureCreator {
public:
	sColor24Bit* p24BitImage;
	sColor32Bit* p32BitImage;

	cTextureCreator();
	~cTextureCreator();

	void FillImage24Bit(cBMPImage* image24Bit);		// fills the p24BitImage
	void FillImage32Bit(cBMPImage* image32Bit);		// fills the p32BitImage
	void CreateTextureFrom24BitBMP(std::string filePath, GLuint& textureID);
	void CreateTextureFrom32BitBMP(std::string filePath, GLuint& textureID);
	
	void LoadTextures24Bit(GLuint shaderProgram, cLoadModels& model, bool bUseTexture);
	void LoadTextures32Bit(GLuint shaderProgram, cLoadModels& model, bool bUseTexture);
};