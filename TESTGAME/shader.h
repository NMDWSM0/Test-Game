#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
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
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    //\param vertexPath 顶点着色器文件路径
    //\param fragmentPath 片元着色器文件路径
    Shader(const char* vertexPath, const char* fragmentPath);

    // 使用/激活程序
    void use();

    // uniform工具函数

    // 设置着色器程序中bool类型uniform变量的值
    //\param name uniform变量名，以string类型输入
    //\param value 将要设置的值
    void setBool(const std::string& name, bool value) const;
    void setBool(const std::string& name, bool value1, bool value2) const;
    void setBool(const std::string& name, bool value1, bool value2, bool value3) const;
    void setBool(const std::string& name, bool value1, bool value2, bool value3, bool value4) const;

    // 设置着色器程序中int类型uniform变量的值
    //\param name uniform变量名，以string类型输入
    //\param value 将要设置的值
    void setInt(const std::string& name, int value) const;
    void setInt(const std::string& name, int value1, int value2) const;
    void setInt(const std::string& name, int value1, int value2, int value3) const;
    void setInt(const std::string& name, int value1, int value2, int value3, int value4) const;

    // 设置着色器程序中float类型uniform变量的值
    //\param name uniform变量名，以string类型输入
    //\param value 将要设置的值
    void setFloat(const std::string& name, float value) const;
    void setFloat(const std::string& name, float value1, float value2) const;
    void setFloat(const std::string& name, float value1, float value2, float value3) const;
    void setFloat(const std::string& name, float value1, float value2, float value3, float value4) const;

    // 设置着色器程序中vec类型uniform变量的值
    //\param name uniform变量名，以string类型输入
    //\param value 将要设置的向量，无需先进行value_ptr转换
    template <class T>
    void setVec(const std::string& name, const T& vec) const;

    // 设置着色器程序中mat类型uniform变量的值
    //\param name uniform变量名，以string类型输入
    //\param value 将要设置的矩阵，无需先进行value_ptr转换
    template <class T>
    void setMat(const std::string& name, const T& value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. 编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片元着色器
    fragment= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
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