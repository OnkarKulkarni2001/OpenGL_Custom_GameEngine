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
			p24BitImage[pixelCount] = image24Bit->GetColor24Bit(x, y);
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
			p32BitImage[pixelCount] = image32Bit->GetColor32Bit(x, y);
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
