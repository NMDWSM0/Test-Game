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
    //������������Լ��Ƿ���һ������������
	Texture(const char* imagename, TextureType type = TEXTURE, unsigned int resolution = 2048);

	//������������õ�UNIT��ָ���TEXTURE��
	//\param UNIT: Ҫ���õ�TEXTURE����GL_TEXTURE0
	void Activate(GLenum UNIT);

	//������󶨵�֡������������Դ��
	/*void BindToFrameBuffer()
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	}*/
};

Texture::Texture(const char* imagename, TextureType type, unsigned int resolution):
	texturetype(type)
{
	//�����������
	glGenTextures(1, &texture);

	//������
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
		//�󶨵�֡��������
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
		//ʹ���ٽ�������ʽ����Ҫ��ֵ

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		//�󶨵�֡��������
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		//����ɫ����������һ����Ȼ���
		glGenTextures(1, &depthdrawbuffer);
		glBindTexture(GL_TEXTURE_2D, depthdrawbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resolution, resolution, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // Ԥ����ռ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//����Ȼ�����ڵ�ǰ����ɫ������
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthdrawbuffer, 0);

		return;
	}

	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagename, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		if (nrChannels == 3)//rgb ������jpgͼ��
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//����һ����RGBA
		else if (nrChannels == 4)//rgba ������pngͼ��
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//ע�⣬��������RGBA
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
//	//�ֱ��Ǵ洢 worldCoord , normal �� flux ������
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
//	//�󶨵�֡��������
//	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, attachments);
//
//	//����ɫ����������һ����Ȼ���
//	glGenTextures(1, &depthdrawbuffer);
//	glBindTexture(GL_TEXTURE_2D, depthdrawbuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, resolution, resolution, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // Ԥ����ռ�
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//����Ȼ�����ڵ�ǰ����ɫ������
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