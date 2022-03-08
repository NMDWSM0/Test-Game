#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static bool isPowerOf2(unsigned int value) 
{
	return (value & (value - 1)) == 0;
}

enum TextureType {
	TEXTURE,
	DEPTHBUFFER,
	COLORBUFFER
};

class Texture
{
protected:
	GLuint texture;
	TextureType texturetype;
	GLuint depthdrawbuffer = 0;
public:
    //创建纹理对象，以及是否是一个缓冲区纹理
	Texture(const char* imagename, TextureType type = TEXTURE, unsigned int resolution = 2048);

	//将纹理对象启用到UNIT所指向的TEXTURE中
	//\param UNIT: 要启用的TEXTURE，如GL_TEXTURE0
	void Activate(GLenum UNIT);

	//将纹理绑定到帧缓冲区（数据源）
	/*void BindToFrameBuffer()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	}*/
};

Texture::Texture(const char* imagename, TextureType type, unsigned int resolution):
	texturetype(type)
{
	//创建纹理对象
	glGenTextures(1, &texture);

	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);

	if (type == DEPTHBUFFER)
	{
		if (!isPowerOf2(resolution))
			resolution = 2048;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		//绑定到帧缓冲区上
		glDrawBuffer(NULL);
		glReadBuffer(NULL);
		return;
	}
	else if (type == COLORBUFFER)
	{
		if (!isPowerOf2(resolution))
			resolution = 2048;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//使用临近采样方式，不要插值

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		//绑定到帧缓冲区上
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		//给颜色缓冲区分配一个深度缓冲
		glGenTextures(1, &depthdrawbuffer);
		glBindTexture(GL_TEXTURE_2D, depthdrawbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resolution, resolution, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // 预分配空间
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//把深度缓冲绑在当前的颜色缓冲上
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthdrawbuffer, 0);

		return;
	}

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagename, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		if (nrChannels == 3)//rgb 适用于jpg图像
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//后面一个是RGBA
		else if (nrChannels == 4)//rgba 适用于png图像
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//注意，两个都是RGBA
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture:" << imagename << std::endl;
	}
	stbi_image_free(data);
}

void Texture::Activate(GLenum UNIT)
{
	glActiveTexture(UNIT);
	glBindTexture(GL_TEXTURE_2D, texture);
}

/*************************************************************************/

//class RSMTexture
//{
//	//分别是存储 worldCoord , normal 和 flux 的纹理
//	GLuint texture1, texture2, texture3;
//	GLuint depthdrawbuffer;
//public:
//	RSMTexture(unsigned int resolution = 2048);
//
//	void Activate(GLenum UNIT1, GLenum UNIT2, GLenum UNIT3);
//};
//
//RSMTexture::RSMTexture(unsigned int resolution)
//{
//	if (!isPowerOf2(resolution))
//		resolution = 2048;
//
//	glGenTextures(1, &texture1);
//	glGenTextures(1, &texture2);
//	glGenTextures(1, &texture3);
//
//	std::vector<GLuint> textures = { texture1 , texture2, texture3 };
//
//	for (auto t : textures)
//	{
//		glBindTexture(GL_TEXTURE_2D, t);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_FLOAT, NULL);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//	}
//
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture2, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, texture3, 0);
//
//	//绑定到帧缓冲区上
//	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, attachments);
//
//	//给颜色缓冲区分配一个深度缓冲
//	glGenTextures(1, &depthdrawbuffer);
//	glBindTexture(GL_TEXTURE_2D, depthdrawbuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resolution, resolution, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // 预分配空间
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//把深度缓冲绑在当前的颜色缓冲上
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthdrawbuffer, 0);
//}
//
//void RSMTexture::Activate(GLenum UNIT1, GLenum UNIT2, GLenum UNIT3)
//{
//	glActiveTexture(UNIT1);
//	glBindTexture(GL_TEXTURE_2D, texture1);
//	glActiveTexture(UNIT2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	glActiveTexture(UNIT3);
//	glBindTexture(GL_TEXTURE_2D, texture3);
//}

#endif