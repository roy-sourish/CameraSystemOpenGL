#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex and fragment shader source code from file path.
	//-----------------------------------------------------------------------

	string vertexCode;
	string fragmentCode;

	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;

	// ensure ifstream objects can throw exceptions:
	vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// open files.
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		// read file's buffer contents into stream.
		stringstream vShaderStream, fShaderStream;
		
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers.
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert into string.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	// 2. Compile Shaders.
	//--------------------

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	// Vertex Shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX_SHADER");

	// Fragment Shader.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT_SHADER");

	// 3. Create Shader Program and link Vertex and Fragment Shader.
	//--------------------------------------------------------------

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	checkCompileErrors(shaderID, "SHADER_PROGRAM");

	// 4. Delete shaders after linking them to shader program.
	//--------------------------------------------------------
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


void Shader::useShaderProgram() const
{
	glUseProgram(shaderID);
}


void Shader::checkCompileErrors(unsigned int shader, string shaderType)
{
	int success;
	char infoLog[1024];

	if (shaderType != "SHADER_PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR:: SHADER COMPILATION FAILED OF TYPE: " << shaderType << std::endl;
			cout << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR OF TYPE: " << shaderType << std::endl;
			cout << infoLog << std::endl;
		}
	}
}
