#include "GLSLProgram.h"

#include <iostream>
#include <fstream>
#include <vector>


namespace gines
{

	GLSLProgram::~GLSLProgram()
	{
	}
	GLSLProgram::GLSLProgram() : numberOfAttributes(0), programID(0), vertexShaderID(0), fragmentShaderID(0)
	{

	}


	void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		programID = glCreateProgram();

		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertexShaderID == 0)
		{
			Message("glCreateShader(GL_VERTEX_SHADER) failed!", gines::Message::Error);
			return;
		}
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0)
		{
			Message("glCreateShader(GL_FRAGMENT_SHADER) failed!", gines::Message::Error);
			return;
		}

		compileShader(vertexShaderPath, vertexShaderID);
		compileShader(fragmentShaderPath, fragmentShaderID);


	}
	void GLSLProgram::compileShader(const std::string& filePath, GLuint id)
	{
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail())
		{
			Message("std::ifstream vertexFile(vertexShaderPath) failed!", gines::Message::Error);
			return;
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}
		vertexFile.close();

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);


		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			if (errorLog.size() > 0)
			{
				glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			}

			glDeleteShader(id);

			if (errorLog.size() > 0){ Message(&errorLog[0], gines::Message::Error); }
			Message(("glGetShaderiv(id, GL_COMPILE_STATUS, &success) failed! (" + filePath + ")").c_str(), gines::Message::Error);
		}
	}
	void GLSLProgram::linkShaders()
	{

		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);

		glLinkProgram(programID);



		GLint linkStatus = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, (int*)&linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			if (errorLog.size() > 0){ glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]); }

			glDeleteProgram(programID);

			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			if (errorLog.size() > 0){ Message(&(errorLog[0]), gines::Message::Error); }
			Message("Shaders failed to link!", gines::Message::Warning);
		}


		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(programID, numberOfAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			Message(("Uniform " + uniformName + " not found!").c_str(), gines::Message::Error);
			return 0;
		}
		return location;
	}

	void GLSLProgram::use()
	{
		glUseProgram(programID);
		for (int i = 0; i < numberOfAttributes; i++){ glEnableVertexAttribArray(i); }
	}
	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < numberOfAttributes; i++){ glDisableVertexAttribArray(i); }
	}

}