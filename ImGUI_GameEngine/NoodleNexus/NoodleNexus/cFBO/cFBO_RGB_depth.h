#ifndef _FBO_HG_
#define _FBO_HG_

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cFBO_RGB_depth
{
public:
	cFBO_RGB_depth() :
		ID(0), 
		colourTexture_0_ID(0),
		depthTexture_ID(0), 
		width(-1), height(-1) {};

	GLuint ID;						// = 0;
	GLuint colourTexture_0_ID;		// = 0;
//	GLuint colourTexture_1_ID;		// = 0;
//	GLuint colourTexture_2_ID;		// = 0;
//	...etc.
//	GLuint TBDTexture_1_ID;
//	GLuint TBDTexture_2_ID;

	GLuint depthTexture_ID;		// = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);

	void clearColourBuffer(int bufferindex);
	void clearAllColourBuffers(void);
	void clearDepthBuffer(void);
	void clearStencilBuffer( int clearColour, int mask = 0xFF );

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif
