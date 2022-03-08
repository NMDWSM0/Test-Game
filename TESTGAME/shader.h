#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // ����glad����ȡ���еı���OpenGLͷ�ļ�
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // ����ID
    unsigned int ID;

    // ��������ȡ��������ɫ��
    //\param vertexPath ������ɫ���ļ�·��
    //\param fragmentPath ƬԪ��ɫ���ļ�·��
    Shader(const char* vertexPath, const char* fragmentPath);

    // ʹ��/�������
    void use();

    // uniform���ߺ���

    // ������ɫ��������bool����uniform������ֵ
    //\param name uniform����������string��������
    //\param value ��Ҫ���õ�ֵ
    void setBool(const std::string& name, bool value) const;
    void setBool(const std::string& name, bool value1, bool value2) const;
    void setBool(const std::string& name, bool value1, bool value2, bool value3) const;
    void setBool(const std::string& name, bool value1, bool value2, bool value3, bool value4) const;

    // ������ɫ��������int����uniform������ֵ
    //\param name uniform����������string��������
    //\param value ��Ҫ���õ�ֵ
    void setInt(const std::string& name, int value) const;
    void setInt(const std::string& name, int value1, int value2) const;
    void setInt(const std::string& name, int value1, int value2, int value3) const;
    void setInt(const std::string& name, int value1, int value2, int value3, int value4) const;

    // ������ɫ��������float����uniform������ֵ
    //\param name uniform����������string��������
    //\param value ��Ҫ���õ�ֵ
    void setFloat(const std::string& name, float value) const;
    void setFloat(const std::string& name, float value1, float value2) const;
    void setFloat(const std::string& name, float value1, float value2, float value3) const;
    void setFloat(const std::string& name, float value1, float value2, float value3, float value4) const;

    // ������ɫ��������vec����uniform������ֵ
    //\param name uniform����������string��������
    //\param value ��Ҫ���õ������������Ƚ���value_ptrת��
    template <class T>
    void setVec(const std::string& name, const T& vec) const;

    // ������ɫ��������mat����uniform������ֵ
    //\param name uniform����������string��������
    //\param value ��Ҫ���õľ��������Ƚ���value_ptrת��
    template <class T>
    void setMat(const std::string& name, const T& value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // ���ļ�
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�������
        vShaderFile.close();
        fShaderFile.close();
        // ת����������string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. ������ɫ��
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // ������ɫ��
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    // ��ӡ�����������еĻ���
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // ƬԪ��ɫ��
    fragment= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    // ��ӡ�����������еĻ���
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // ��ɫ������
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // ��ӡ���Ӵ�������еĻ���
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    use();
}

//
void Shader::use()
{
    glUseProgram(ID);
}

//
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setBool(const std::string& name, bool value1, bool value2) const
{
    glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value1, (int)value2);
}
void Shader::setBool(const std::string& name, bool value1, bool value2, bool value3) const
{
    glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value1, (int)value2, (int)value3);
}
void Shader::setBool(const std::string& name, bool value1, bool value2, bool value3, bool value4) const
{
    glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value1, (int)value2, (int)value3, (int)value4);
}

//
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setInt(const std::string& name, int value1, int value2) const
{
    glUniform2i(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setInt(const std::string& name, int value1, int value2, int value3) const
{
    glUniform3i(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setInt(const std::string& name, int value1, int value2, int value3, int value4) const
{
    glUniform4i(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

//
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value1, float value2) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void Shader::setFloat(const std::string& name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void Shader::setFloat(const std::string& name, float value1, float value2, float value3, float value4) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

//
template <class T>
void Shader::setMat(const std::string& name, const T& value) const
{
    /*if (typeid(T) == typeid(glm::mat))
        glUniformMatrix1fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    else*/ if (typeid(T) == typeid(glm::mat2))
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    else if (typeid(T) == typeid(glm::mat3))
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    else if (typeid(T) == typeid(glm::mat4))
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    //??
    else
        std::cout << "This is Other Matrix Type" << std::endl;
}

//
template <class T>
void Shader::setVec(const std::string& name, const T& vec) const
{
    /*if (typeid(T) == typeid(glm::vec))
        glUniform1f(glGetUniformLocation(ID, name.c_str()), vec.x);
    else*/ if (typeid(T) == typeid(glm::vec2))
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
    else if (typeid(T) == typeid(glm::vec3))
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
    else if (typeid(T) == typeid(glm::vec4))
        glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    //??
    else
        std::cout << "This is Other Vector Type" << std::endl;
}

#endif