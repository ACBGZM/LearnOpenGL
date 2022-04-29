// ʹ��Ԥ����ָ���֪��������ֻ����û��������������²Ű����ͱ������ͷ�ļ�
// ��ʹ����ļ��������������ɫ��ͷ�ļ���Ҳֻ����һ�Ρ�����������ֹ���ӳ�ͻ��
#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	unsigned int ID;	// shader program ID

	// ����������ȡ������shader program
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment shader source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		// ensure ifstream objects can throw exceptions
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			// read file's buffer contents into streams
			std::stringstream vertexShaderStream, fragmentShaderStream;
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();
			// close file handlers
			vertexShaderFile.close();
			fragmentShaderFile.close();
			// convert stream into string
			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		// ��ȡָ���ַ�������ͷ��ָ��
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;	// shader ID
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// link shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into the program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// ʹ��(����)shader program
	void use()
	{
		glUseProgram(ID);
	}

	// uniform ���ߺ�������ѯuniformȫ�ֱ�����λ�ã�������
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}


private:
	// utility function for checking shader compilation/linking errorss
	// ---------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};


#endif // !SHADER_H

