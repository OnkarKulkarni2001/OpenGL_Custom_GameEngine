#include "cShaderManager.h"

#include "../GLCommon.h"	// For all the OpenGL calls, etc.

#include <fstream>
#include <sstream>		// "string stream"
#include <vector>

#include <algorithm>		// for the std::copy
#include <iterator>			// for the back_inserter

std::string unifromEnumTypeToString(GLenum enumType);


cShaderManager::cShaderManager()
{
	return;
}

cShaderManager::~cShaderManager()
{
	return;
}


bool cShaderManager::useShaderProgram( unsigned int ID )
{
	// Use the number directy... 
	// TODO: Might do a lookup to see if we really have that ID...
	glUseProgram(ID);
	return true;
}

bool cShaderManager::useShaderProgram( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	// Didn't find it
		// Maybe set glUseProgram(0)....?
		return false;
	}
	glUseProgram(itShad->second);

	return true;
}

unsigned int cShaderManager::getIDFromFriendlyName( std::string friendlyName )
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator 
			itShad = this->m_name_to_ID.find(friendlyName);

	if ( itShad == this->m_name_to_ID.end() )
	{	// Didn't find it
		return 0;
	}
	return itShad->second;
}

cShaderManager::cShaderProgram* 
	cShaderManager::pGetShaderProgramFromFriendlyName( std::string friendlyName )
{
	unsigned int shaderID = this->getIDFromFriendlyName(friendlyName);

	// Now get the actual shader
	
		std::map< unsigned int /*ID*/, cShaderProgram >::iterator
			itShad = this->m_ID_to_Shader.find(shaderID);

	if ( itShad == this->m_ID_to_Shader.end() )
	{	// Didn't find it
		return NULL;		// or 0 or nullptr
	}

	cShaderProgram* pShaderIFound = &(itShad->second);

	return pShaderIFound;
}


const unsigned int MAXLINELENGTH = 65536;		// 16x1024

void cShaderManager::setBasePath( std::string basepath )
{
	this->m_basepath = basepath;
	return;
}


// Returns bool if didn't load
bool cShaderManager::m_loadSourceFromFile( cShader &shader )
{
	std::string fullFileName = this->m_basepath + shader.fileName;

	std::ifstream theFile( fullFileName.c_str() );
	if ( ! theFile.is_open() )
	{
		return false;
	}

	//std::stringstream ssSource;
	//std::string temp;

	//while ( theFile >> temp )
	//{	// Add it to the string stream
	//	ssSource << temp << " ";
	//}

	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = {0};
	while ( theFile.getline( pLineTemp, MAXLINELENGTH ) )
	{
		std::string tempString(pLineTemp);
		//if ( tempString != "" )
		//{	// Line isn't empty, so add
			shader.vecSource.push_back( tempString );
		//}
	}
	
	theFile.close();
	return true;		// Return the string (from the sstream)
}

// Returns empty string if no error
// returns false if no error
bool cShaderManager::m_wasThereACompileError( unsigned int shaderID, 
											  std::string &errorText )
{
	errorText = "";	// No error

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;	// Oops

		return true;	// There WAS an error
	}
	return false; // There WASN'T an error
}

bool cShaderManager::m_wasThereALinkError( unsigned int programID, std::string &errorText )
{
	errorText = "";	// No error

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if(wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->m_lastError.append("\n");
		this->m_lastError.append( errorText );


		delete [] pLogText;	

		// There WAS an error
		return true;	
	}
	
	// There WASN'T an error
	return false; 
}


std::string cShaderManager::getLastError(void)
{
	std::string lastErrorTemp = this->m_lastError;
	this->m_lastError = "";
	return lastErrorTemp;
}

#include <iostream>

bool cShaderManager::m_compileShaderFromSource( cShaderManager::cShader &shader, std::string &error )
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char*[numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset( arraySource, 0, numberOfLines );	

	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset( arraySource[indexLine], 0, numCharacters + 2 );

		// Copy line of source into array
		for ( unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++ )
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}//for ( unsigned int indexChar = 0...
		
		// At a '\0' at end (just in case)
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

		// Or you could use the (unsecure) strcpy (string copy)
//		strcpy( arraySource[indexLine], shader.vecSource[indexLine].c_str() );

		// Or the "secure" version, where you specify the number of chars to copy
//		strcpy_s( arraySource[indexLine], 
//				  strlen( shader.vecSource[indexLine].c_str() + 1 ),	// Number of char in copy + 0 terminator
//				  shader.vecSource[indexLine].c_str() );

	}//for ( unsigned int indexLine = 0...

//******************************************************************************************
//	// Print it out (for debug)
//	std::cout << "Source (from char** array):" << std::endl;
//	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
//	{
//		std::cout << "(" << indexLine << "):\t" << arraySource[indexLine] << std::endl;
//	}
//	std::cout << "END" << std::endl;
//******************************************************************************************

	//const char* tempVertChar = "Hey there;";

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
    glCompileShader(shader.ID); 

	// Get rid of the temp source "c" style array
	for ( unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++ )
	{	// Delete this line
		delete [] arraySource[indexLine];
	}
	// And delete the original char** array
	delete [] arraySource;

	// Did it work? 
	std::string errorText = "";
	if ( this->m_wasThereACompileError( shader.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << shader.getShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}

	return true;
}



bool cShaderManager::createProgramFromFile( 
	        std::string friendlyName,
			cShader &vertexShad, 
			cShader &fragShader )
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = cShader::VERTEX_SHADER;
	//  char* vertex_shader_text = "wewherlkherlkh";
	// Load some text from a file...
	if ( ! this->m_loadSourceFromFile( vertexShad ) )
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	errorText = "";
	if ( ! this->m_compileShaderFromSource( vertexShad, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...



    fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = cShader::FRAGMENT_SHADER;
	if ( ! this->m_loadSourceFromFile( fragShader ) )
	{
		return false;
	}//if ( ! this->m_loadSourceFromFile(...

	if ( ! this->m_compileShaderFromSource( fragShader, errorText ) )
	{
		this->m_lastError = errorText;
		return false;
	}//if ( this->m_compileShaderFromSource(...


	cShaderProgram curProgram;
    curProgram.ID = glCreateProgram();

    glAttachShader(curProgram.ID, vertexShad.ID);
    glAttachShader(curProgram.ID, fragShader.ID);
    glLinkProgram(curProgram.ID);

	// Was there a link error? 
	errorText = "";
	if ( this->m_wasThereALinkError( curProgram.ID, errorText ) )
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}

	// At this point, shaders are compiled and linked into a program

	curProgram.friendlyName = friendlyName;

	// Add the shader to the map
	this->m_ID_to_Shader[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_name_to_ID[curProgram.friendlyName] = curProgram.ID;

	return true;
}


// Calls this: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
std::string cShaderManager::cShaderProgram::getActiveUniformSummary(void)
{
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetProgram.xhtml
	std::stringstream ssAUSummary;

	GLint numActiveUniforms = 0;
	glGetProgramiv(this->ID,	// Shader program ID ("name")
		GL_ACTIVE_UNIFORMS,		// Constant: how many active uniforms are there?
		&numActiveUniforms);	// Place to load this value

	// Go through each one and get info. about each uniform
	for (GLuint uniformIndex = 0; uniformIndex != (GLuint)numActiveUniforms; uniformIndex++)
	{
//		void glGetActiveUniform(GLuint program,
//			GLuint index,
//			GLsizei bufSize,
//			GLsizei * length,
//			GLint * size,
//			GLenum * type,
//			GLchar * name);

		GLsizei numCharactersInUniformName = 0; // GLsizei * length,
		GLint sizeOfUniformInBytes = 0;			// GLint * size,
		GLenum enumTypeOfUniform = 0;			// GLenum * type
		// Maximum number of characters we can hold
		const GLsizei NAME_BUFFER_SIZE = 4096;			// GLsizei bufSize
		GLchar uniformName[NAME_BUFFER_SIZE] = { 0 };			// Buffer to hold name string

		glGetActiveUniform(this->ID,
			uniformIndex,			// 
			NAME_BUFFER_SIZE,
			&numCharactersInUniformName,
			&sizeOfUniformInBytes,
			&enumTypeOfUniform,
			uniformName);

		ssAUSummary << uniformName
			<< " UL: " << uniformIndex
			<< " is " << sizeOfUniformInBytes
			<< " " << unifromEnumTypeToString(enumTypeOfUniform) << std::endl;

		this->mapUniformName_to_UniformLocation[std::string(uniformName)] = uniformIndex;

	}



	return ssAUSummary.str();
}

std::string unifromEnumTypeToString(GLenum enumType)
{
	switch (enumType)
	{
	case GL_FLOAT:			// 	float
		return "GL_FLOAT (float)";
		break;
	case GL_FLOAT_VEC2:		// 	vec2
		return "GL_FLOAT_VEC2 (vec2)";
		break;
	case GL_FLOAT_VEC3:		// 	vec3
		return "GL_FLOAT_VEC3 (vec3)";
		break;
	case GL_FLOAT_VEC4:		// 	vec4
		return "GL_FLOAT_VEC4 (vec4)";
		break;
		//	GL_DOUBLE	double
		//	GL_DOUBLE_VEC2	dvec2
		//	GL_DOUBLE_VEC3	dvec3
		//	GL_DOUBLE_VEC4	dvec4
		//	GL_INT	int
		//	GL_INT_VEC2	ivec2
		//	GL_INT_VEC3	ivec3
		//	GL_INT_VEC4	ivec4
		//	GL_UNSIGNED_INT	unsigned int
		//	GL_UNSIGNED_INT_VEC2	uvec2
		//	GL_UNSIGNED_INT_VEC3	uvec3
		//	GL_UNSIGNED_INT_VEC4	uvec4
		//	GL_BOOL	bool
		//	GL_BOOL_VEC2	bvec2
		//	GL_BOOL_VEC3	bvec3
		//	GL_BOOL_VEC4	bvec4
		//	GL_FLOAT_MAT2	mat2
		//	GL_FLOAT_MAT3	mat3
	case GL_FLOAT_MAT4:		// 	mat4
		return "GL_FLOAT_MAT4 (mat4)";
		break;
		//	GL_FLOAT_MAT2x3	mat2x3
		//	GL_FLOAT_MAT2x4	mat2x4
		//	GL_FLOAT_MAT3x2	mat3x2
		//	GL_FLOAT_MAT3x4	mat3x4
		//	GL_FLOAT_MAT4x2	mat4x2
		//	GL_FLOAT_MAT4x3	mat4x3
		//	GL_DOUBLE_MAT2	dmat2
		//	GL_DOUBLE_MAT3	dmat3
		//	GL_DOUBLE_MAT4	dmat4
		//	GL_DOUBLE_MAT2x3	dmat2x3
		//	GL_DOUBLE_MAT2x4	dmat2x4
		//	GL_DOUBLE_MAT3x2	dmat3x2
		//	GL_DOUBLE_MAT3x4	dmat3x4
		//	GL_DOUBLE_MAT4x2	dmat4x2
		//	GL_DOUBLE_MAT4x3	dmat4x3
		//	GL_SAMPLER_1D	sampler1D
	case GL_SAMPLER_2D:			// 	sampler2D
		return "GL_SAMPLER_2D (sampler2D)";
		break;
		//	GL_SAMPLER_3D	sampler3D
		//	GL_SAMPLER_CUBE	samplerCube
		//	GL_SAMPLER_1D_SHADOW	sampler1DShadow
		//	GL_SAMPLER_2D_SHADOW	sampler2DShadow
		//	GL_SAMPLER_1D_ARRAY	sampler1DArray
		//	GL_SAMPLER_2D_ARRAY	sampler2DArray
		//	GL_SAMPLER_1D_ARRAY_SHADOW	sampler1DArrayShadow
		//	GL_SAMPLER_2D_ARRAY_SHADOW	sampler2DArrayShadow
		//	GL_SAMPLER_2D_MULTISAMPLE	sampler2DMS
		//	GL_SAMPLER_2D_MULTISAMPLE_ARRAY	sampler2DMSArray
		//	GL_SAMPLER_CUBE_SHADOW	samplerCubeShadow
		//	GL_SAMPLER_BUFFER	samplerBuffer
		//	GL_SAMPLER_2D_RECT	sampler2DRect
		//	GL_SAMPLER_2D_RECT_SHADOW	sampler2DRectShadow
		//	GL_INT_SAMPLER_1D	isampler1D
		//	GL_INT_SAMPLER_2D	isampler2D
		//	GL_INT_SAMPLER_3D	isampler3D
		//	GL_INT_SAMPLER_CUBE	isamplerCube
		//	GL_INT_SAMPLER_1D_ARRAY	isampler1DArray
		//	GL_INT_SAMPLER_2D_ARRAY	isampler2DArray
		//	GL_INT_SAMPLER_2D_MULTISAMPLE	isampler2DMS
		//	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	isampler2DMSArray
		//	GL_INT_SAMPLER_BUFFER	isamplerBuffer
		//	GL_INT_SAMPLER_2D_RECT	isampler2DRect
		//	GL_UNSIGNED_INT_SAMPLER_1D	usampler1D
		//	GL_UNSIGNED_INT_SAMPLER_2D	usampler2D
		//	GL_UNSIGNED_INT_SAMPLER_3D	usampler3D
		//	GL_UNSIGNED_INT_SAMPLER_CUBE	usamplerCube
		//	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY	usampler2DArray
		//	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY	usampler2DArray
		//	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE	usampler2DMS
		//	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	usampler2DMSArray
		//	GL_UNSIGNED_INT_SAMPLER_BUFFER	usamplerBuffer
		//	GL_UNSIGNED_INT_SAMPLER_2D_RECT	usampler2DRect
		//	GL_IMAGE_1D	image1D
		//	GL_IMAGE_2D	image2D
		//	GL_IMAGE_3D	image3D
		//	GL_IMAGE_2D_RECT	image2DRect
		//	GL_IMAGE_CUBE	imageCube
		//	GL_IMAGE_BUFFER	imageBuffer
		//	GL_IMAGE_1D_ARRAY	image1DArray
		//	GL_IMAGE_2D_ARRAY	image2DArray
		//	GL_IMAGE_2D_MULTISAMPLE	image2DMS
		//	GL_IMAGE_2D_MULTISAMPLE_ARRAY	image2DMSArray
		//	GL_INT_IMAGE_1D	iimage1D
		//	GL_INT_IMAGE_2D	iimage2D
		//	GL_INT_IMAGE_3D	iimage3D
		//	GL_INT_IMAGE_2D_RECT	iimage2DRect
		//	GL_INT_IMAGE_CUBE	iimageCube
		//	GL_INT_IMAGE_BUFFER	iimageBuffer
		//	GL_INT_IMAGE_1D_ARRAY	iimage1DArray
		//	GL_INT_IMAGE_2D_ARRAY	iimage2DArray
		//	GL_INT_IMAGE_2D_MULTISAMPLE	iimage2DMS
		//	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY	iimage2DMSArray
		//	GL_UNSIGNED_INT_IMAGE_1D	uimage1D
		//	GL_UNSIGNED_INT_IMAGE_2D	uimage2D
		//	GL_UNSIGNED_INT_IMAGE_3D	uimage3D
		//	GL_UNSIGNED_INT_IMAGE_2D_RECT	uimage2DRect
		//	GL_UNSIGNED_INT_IMAGE_CUBE	uimageCube
		//	GL_UNSIGNED_INT_IMAGE_BUFFER	uimageBuffer
		//	GL_UNSIGNED_INT_IMAGE_1D_ARRAY	uimage1DArray
		//	GL_UNSIGNED_INT_IMAGE_2D_ARRAY	uimage2DArray
		//	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE	uimage2DMS
		//	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY	uimage2DMSArray
		//	GL_UNSIGNED_INT_ATOMIC_COUNTER	atomic_uint

//	default:
//		return "UNKNOWN";
	}//switch (enumType)

	return "UNKNOWN";
}