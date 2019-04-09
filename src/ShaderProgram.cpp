#include "ShaderProgram.h"

#include <iostream>
#include <cassert>

#include "GLSL.h"
#include "SOIL.h"

using namespace std;

ShaderProgram::ShaderProgram() : ShaderProgram("", "") {
}

ShaderProgram::ShaderProgram(string vShaderName, string fShaderName) :
	vShaderName(vShaderName),
	fShaderName(fShaderName),
	pid(0),
	verbose(true)
{
	init();
}

ShaderProgram::~ShaderProgram()
{
	
}

void ShaderProgram::setShaderNames(const string &v, const string &f)
{
	vShaderName = v;
	fShaderName = f;
}

bool ShaderProgram::init()
{
	GLint rc;
	
	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	if(!rc) {
		if(isVerbose()) {
			GLSL::printShaderInfoLog(VS);
			cout << "Error compiling vertex shader " << vShaderName << endl;
		}
		return false;
	}
	
	// Compile fragment shader
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	if(!rc) {
		if(isVerbose()) {
			GLSL::printShaderInfoLog(FS);
			cout << "Error compiling fragment shader " << fShaderName << endl;
		}
		return false;
	}
	
	// Create the program and link
	pid = glCreateProgram();
	glAttachShader(pid, VS);
	glAttachShader(pid, FS);
	glLinkProgram(pid);
	glGetProgramiv(pid, GL_LINK_STATUS, &rc);
	if(!rc) {
		if(isVerbose()) {
			GLSL::printProgramInfoLog(pid);
			cout << "Error linking shaders " << vShaderName << " and " << fShaderName << endl;
		}
		return false;
	}
	
	// Add common GameObject Attributes/Uniforms
	addAttribute("aPos");
	addAttribute("aNor");
	addAttribute("aTex");
	addUniform("P");
	addUniform("MV");

	GLSL::checkError(GET_FILE_LINE);
	return true;
}

void ShaderProgram::bind()
{
	glUseProgram(pid);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}

void ShaderProgram::addAttribute(const string &name)
{
	attributes[name] = glGetAttribLocation(pid, name.c_str());
}

void ShaderProgram::addUniform(const string &name)
{
	uniforms[name] = glGetUniformLocation(pid, name.c_str());
}

GLint ShaderProgram::getAttribute(const string &name) const
{
	map<string,GLint>::const_iterator attribute = attributes.find(name.c_str());
	if(attribute == attributes.end()) {
		if(isVerbose()) {
			cout << name << " is not an attribute variable" << endl;
		}
		return -1;
	}
	return attribute->second;
}

GLint ShaderProgram::getUniform(const string &name) const
{
	map<string,GLint>::const_iterator uniform = uniforms.find(name.c_str());
	if(uniform == uniforms.end()) {
		if(isVerbose()) {
			cout << name << " is not a uniform variable" << endl;
		}
		return -1;
	}
	return uniform->second;
}

GLuint ShaderProgram::generateTexture(const std::string &texPath, int width, int height, unsigned char ** image) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	*image = SOIL_load_image(texPath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}
