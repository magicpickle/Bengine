#include "GLSLProgram.h"
#include "ErrorHelper.h"

#include <vector>
#include <fstream>

namespace Bengine
{
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		//Get a program object
		_programID = glCreateProgram();
		//Get the vertex ID
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
			fatalError("Vertex shader failed to be created!");
		//Get the Fragment ID
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
			fatalError("Fragment shader failed to be created!");
		//Compile the SHADERS
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}

	void GLSLProgram::linkShaders()
	{
		//Attach the shaders to the program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);
		//Link our program
		glLinkProgram(_programID);
		//Note the different functions here: glGetProgram instead of glGetShader
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLsizei maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			//The Maxlength includes the null character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
			//Provide the infoLog in whatever manor you deem best/
			//Exit with Failure
			//We don't need the program
			glDeleteProgram(_programID);
			//Don't leak the shader
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			//Shut down and log
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Linking Gone Bad");
		}
		//Always Detach shaders after a successful link.
		glDetachShader(_programID, _fragmentShaderID);
		glDetachShader(_programID, _vertexShaderID);
		//Don't leak the shader
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		//Bind the attributes then up the num of  attributes
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}



	void GLSLProgram::compileShader(const std::string& filePath, GLuint shaderID)
	{
		//Open the VertexShafer file Path
		std::ifstream vertexFile(filePath);
		//ErrorChecking
		if (vertexFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed TO Open " + filePath);
		}
		//Read each line of the file and put it into a single string
		std::string fileContents = "", line;
		while (std::getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}
		//MAKE SURE TO CLOSE THE FILE WHEN YOU ARE DONE
		vertexFile.close();
		//Set up the GPU
		const char* contentsPtr = fileContents.c_str(); //Method doesn't like fileContents.c_str() 
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);
		//Compile it
		glCompileShader(shaderID);
		//Erorr Checking
		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		//0 == fail
		if (success == 0)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
			//The Maxlength includes the null character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);
			//Provide the infoLog in whatever manor you deem best/
			//Exit with Failure
			glDeleteShader(shaderID); //Don't leak the shader
			//Shut down and log
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " Failed to compile");
		}
	}

	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}


	void  GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}
}