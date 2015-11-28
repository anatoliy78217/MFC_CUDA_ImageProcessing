//
//	CTexture.h
//

#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <learnopengl/Shader.h>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
using namespace std;

enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

//=============================================================
//
//	Class:	CTexture
//
//	Purpose: Wraps OpenGL texture object and performs their loading.
//
//============================================================

class CTexture
{
public:

	GLuint uiTexture; // OpenGL Texture object handle

	bool CTexture::LoadTexture2D(string file, int flags = CV_LOAD_IMAGE_COLOR,
			GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR, GLenum wrapFilter = GL_REPEAT);

	void BindTexture(int iTextureUnit = 0);

	void SetFiltering(int a_tfMagnification, int a_tfMinification);
	int GetMinificationFilter();
	int GetMagnificationFilter();

	void ReleaseTexture();

	static void SetAutoTexMapping();

	CTexture();

private:

	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	//GLuint uiTexture; // Texture name
	GLuint uiSampler; // Sampler name
	bool bMipMapsGenerated;

	int tfMinification, tfMagnification;

	string sPath;
};

