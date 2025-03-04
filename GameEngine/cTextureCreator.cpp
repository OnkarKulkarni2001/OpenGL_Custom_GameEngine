#include "cTextureCreator.h"
#include <iostream>

cTextureCreator::cTextureCreator() : p24BitImage(0), p32BitImage(0)
{
}

cTextureCreator::~cTextureCreator()
{
}

void cTextureCreator::CreateTextureFrom24BitBMP(std::string filePath, GLuint& textureID)
{
	int clearAnyErrors = glGetError(); // Clearing any old errors

	cBMPImage newTexture(800, 800);		// Initialized with random value of 800,800 as we just want to read data from the bmp image, this newTexture will automatically
										// get the data of bmp image it reads
	newTexture.ReadBMP24Bit(filePath.c_str());
	
	glGenTextures(1, &textureID);		// this textureID is output param

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	// In case texture is oddly aligned, set the client alignment to 1 byte (default is 4)
	GLint GL_UNPACK_ALIGNMENT_old = 0;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);
	// Set alignment to 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FillImage24Bit(&newTexture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		newTexture.GetImageWidth(),
		newTexture.GetImageHeight(),
		0,
		GL_RGB,
		GL_FLOAT,
		this->p24BitImage
	);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, GL_UNPACK_ALIGNMENT_old);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void cTextureCreator::FillImage24Bit(cBMPImage* image24Bit)
{
	int pixelCount = 0;
	p24BitImage = new sColor24Bit[image24Bit->GetImageWidth() * image24Bit->GetImageHeight() * 3];
	for (int x = 0; x != image24Bit->GetImageWidth(); x++) {
		for (int y = 0; y != image24Bit->GetImageHeight(); y++) {
			p24BitImage[pixelCount] = image24Bit->GetColor24Bit(y, x);
			pixelCount++;
		}
	}
}

void cTextureCreator::FillImage32Bit(cBMPImage* image32Bit)
{
	int pixelCount = 0;
	p32BitImage = new sColor32Bit[image32Bit->GetImageWidth() * image32Bit->GetImageHeight() * 4];
	for (int x = 0; x != image32Bit->GetImageWidth(); x++) {
		for (int y = 0; y != image32Bit->GetImageHeight(); y++) {
			p32BitImage[pixelCount] = image32Bit->GetColor32Bit(y, x);
			pixelCount++;
		}
	}
}

void cTextureCreator::CreateTextureFrom32BitBMP(std::string filePath, GLuint& textureID)
{
	int clearAnyErrors = glGetError(); // Clearing any old errors

	cBMPImage newTexture(800, 800);		// Initialized with random value of 800,800 as we just want to read data from the bmp image, this newTexture will automatically
	// get the data of bmp image it reads
	newTexture.ReadBMP32Bit(filePath.c_str());

	glGenTextures(1, &textureID);		// this textureID is output param

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	// In case texture is oddly aligned, set the client alignment to 1 byte (default is 4)
	GLint GL_UNPACK_ALIGNMENT_old = 0;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);
	// Set alignment to 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FillImage32Bit(&newTexture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		newTexture.GetImageWidth(),
		newTexture.GetImageHeight(),
		0,
		GL_RGBA,
		GL_FLOAT,
		this->p32BitImage
	);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, GL_UNPACK_ALIGNMENT_old);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void cTextureCreator::CreateCubeTextureFrom24BitBMP(std::string posX_filePath, std::string negX_filePath,
													std::string posY_filePath, std::string negY_filePath,
													std::string posZ_filePath, std::string negZ_filePath,
													GLuint& cubeTextureID)
{
	int clearAnyErrors = glGetError(); // Clearing any old errors

	glGenTextures(1, &cubeTextureID);		// this textureID is output param

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	cBMPImage newCubePosXTexture(800, 800);	
	cBMPImage newCubeNegXTexture(800, 800);	
	cBMPImage newCubePosYTexture(800, 800);	
	cBMPImage newCubeNegYTexture(800, 800);	
	cBMPImage newCubePosZTexture(800, 800);	
	cBMPImage newCubeNegZTexture(800, 800);	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	newCubePosXTexture.ReadBMP24Bit(posX_filePath.c_str());
	FillImage24Bit(&newCubePosXTexture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGBA8, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight());
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);
	
	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	newCubeNegXTexture.ReadBMP24Bit(negX_filePath.c_str());
	FillImage24Bit(&newCubeNegXTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);

	newCubePosYTexture.ReadBMP24Bit(posY_filePath.c_str());
	FillImage24Bit(&newCubePosYTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);

	newCubeNegYTexture.ReadBMP24Bit(negY_filePath.c_str());
	FillImage24Bit(&newCubeNegYTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);

	newCubePosZTexture.ReadBMP24Bit(posZ_filePath.c_str());
	FillImage24Bit(&newCubePosZTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);

	newCubeNegZTexture.ReadBMP24Bit(negZ_filePath.c_str());
	FillImage24Bit(&newCubeNegZTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p24BitImage);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void cTextureCreator::LoadTextures24Bit(GLuint shaderProgram, cLoadModels& model, bool bUseTexture)
{	
	for (int textureIndex = 0; textureIndex != model.numberOfTextures; textureIndex++) {
		//GLuint textureID;
		CreateTextureFrom24BitBMP(model.textureFilePaths[textureIndex], model.textures[textureIndex]);

		glActiveTexture(GL_TEXTURE0 + textureIndex);   // 0 is texture unit
		glBindTexture(GL_TEXTURE_2D, model.textures[textureIndex]);
		std::string textureString = "textureSamplers[" + to_string(textureIndex) + "]";		// done this as I have array of texture units in shader
		glUniform1i(glGetUniformLocation(shaderProgram, textureString.c_str()), textureIndex);  // textureIndex is texture unit
		glUniform1i(glGetUniformLocation(shaderProgram, "bUseTexture"), bUseTexture);
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "numberOfTextures"), model.numberOfTextures);
}

void cTextureCreator::CreateCubeTextureFrom32BitBMP(std::string posX_filePath, std::string negX_filePath,
													std::string posY_filePath, std::string negY_filePath,
													std::string posZ_filePath, std::string negZ_filePath,
													GLuint& cubeTextureID)
{
	int clearAnyErrors = glGetError(); // Clearing any old errors

	glGenTextures(1, &cubeTextureID);		// this textureID is output param

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	cBMPImage newCubePosXTexture(800, 800);
	cBMPImage newCubeNegXTexture(800, 800);
	cBMPImage newCubePosYTexture(800, 800);
	cBMPImage newCubeNegYTexture(800, 800);
	cBMPImage newCubePosZTexture(800, 800);
	cBMPImage newCubeNegZTexture(800, 800);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	newCubePosXTexture.ReadBMP32Bit(posX_filePath.c_str());
	FillImage32Bit(&newCubePosXTexture);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGBA8, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight());
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	newCubeNegXTexture.ReadBMP32Bit(negX_filePath.c_str());
	FillImage32Bit(&newCubeNegXTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	newCubePosYTexture.ReadBMP32Bit(posY_filePath.c_str());
	FillImage32Bit(&newCubePosYTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	newCubeNegYTexture.ReadBMP32Bit(negY_filePath.c_str());
	FillImage32Bit(&newCubeNegYTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	newCubePosZTexture.ReadBMP32Bit(posZ_filePath.c_str());
	FillImage32Bit(&newCubePosZTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	newCubeNegZTexture.ReadBMP32Bit(negZ_filePath.c_str());
	FillImage32Bit(&newCubeNegZTexture);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, newCubePosXTexture.GetImageWidth(), newCubePosXTexture.GetImageHeight(), GL_RGB, GL_FLOAT, this->p32BitImage);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void cTextureCreator::LoadTextures32Bit(GLuint shaderProgram, cLoadModels& model, bool bUseTexture)
{
	for (int textureIndex = 0; textureIndex != model.numberOfTextures; textureIndex++) {
		//GLuint textureID;
		CreateTextureFrom32BitBMP(model.textureFilePaths[textureIndex], model.textures[textureIndex]);

		glActiveTexture(GL_TEXTURE0 + textureIndex);   // 0 is texture unit
		glBindTexture(GL_TEXTURE_2D, model.textures[textureIndex]);
		glUniform1i(glGetUniformLocation(shaderProgram, "diffuseTexture"), textureIndex);  // 0 is texture unit
		glUniform1i(glGetUniformLocation(shaderProgram, "bUseTexture"), bUseTexture);     // 1 means bUseTexture is true
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "numberOfTextures"), model.numberOfTextures);
}

void cTextureCreator::LoadCubeMap24Bit(GLuint shaderProgram, bool bUseCubeMap, GLuint& cubeTextureID, std::string posX_filePath, std::string negX_filePath, std::string posY_filePath, std::string negY_filePath, std::string posZ_filePath, std::string negZ_filePath)
{
	CreateCubeTextureFrom24BitBMP(posX_filePath, negX_filePath,
								  posY_filePath, negY_filePath,
								  posZ_filePath, negZ_filePath, cubeTextureID);

	glActiveTexture(GL_TEXTURE0 + 40);			// texture unit 40 is used for skybox
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
	glUniform1i(glGetUniformLocation(shaderProgram, "cubeMap"), 40);
	glUniform1i(glGetUniformLocation(shaderProgram, "bUseCubeMap"), bUseCubeMap);     // 1 means bUseTexture is true
}

void cTextureCreator::LoadCubeMap32Bit(GLuint shaderProgram, bool bUseCubeMap, GLuint& cubeTextureID, std::string posX_filePath, std::string negX_filePath, std::string posY_filePath, std::string negY_filePath, std::string posZ_filePath, std::string negZ_filePath)
{
	CreateCubeTextureFrom32BitBMP(posX_filePath, negX_filePath,
								  posY_filePath, negY_filePath,
								  posZ_filePath, negZ_filePath, cubeTextureID);

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
	glUniform1i(glGetUniformLocation(shaderProgram, "cubeMap"), 40);
	glUniform1i(glGetUniformLocation(shaderProgram, "bUseCubeMap"), bUseCubeMap);     // 1 means bUseTexture is true
}
