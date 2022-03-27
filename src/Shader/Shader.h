#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;

class Shader
{
private:
	
	// Shader ID.
	unsigned int shaderID;

public:

	// constructor reads and builds shader programs.
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use shader program.
	void useShaderProgram() const;

	// Get current active Shader Program ID
	unsigned int getShaderID() const { return shaderID; }

private:

	// Check for shader program compilation error.
	void checkCompileErrors(unsigned int shader, string shaderType);
};
