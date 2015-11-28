//
//	CTexture.cpp
//


#include "stdafx.h"


#include "CTexture.h"

//#pragma comment(lib, "FreeImage.lib")

GLuint OpenCVMatToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);

CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

//===================================================================
//
//	Name:		LoadTexture2D
//
//	Params:	file - path to the texture image
//			flags - image file format:
//						CV_LOAD_IMAGE_COLOR
//						CV_LOAD_IMAGE_GRAYSCALE
//						CV_LOAD_IMAGE_ANYDEPTH
//
//	Result:	Loads texture using OpenCV from a file, supports most
//			graphics formats.
//
//=====================================================================

bool CTexture::LoadTexture2D(string file, int flags, GLenum minFilter, 
	GLenum magFilter, GLenum wrapFilter)
{
	cv::Mat mat;
	mat = cv::imread(file, flags);
	uiTexture = OpenCVMatToTexture (mat, minFilter, magFilter, wrapFilter);
	tfMinification = minFilter;
	tfMagnification = magFilter;

	return true; // Success
}

/*-----------------------------------------------

Name:		setFiltering

Params:	tfMagnification - mag. filter, must be from
									ETextureFiltering enum
			tfMinification - min. filter, must be from
									ETextureFiltering enum

Result:	Sets magnification and minification
			texture filter.

/*---------------------------------------------*/

void CTexture::SetFiltering(int a_tfMagnification, int a_tfMinification)
{
	// Set magnification filter
	if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}

/*-----------------------------------------------

Name:		bindTexture

Params:	iTextureUnit - texture unit to bind texture to

Result:	Guess what it does :)

/*---------------------------------------------*/

void CTexture::BindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(iTextureUnit, uiSampler);
}

/*-----------------------------------------------

Name:		releaseTexture

Params:	none

Result:	Frees all memory used by texture.

/*---------------------------------------------*/

void CTexture::ReleaseTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}

int CTexture::GetMinificationFilter()
{
	return tfMinification;
}

int CTexture::GetMagnificationFilter()
{
	return tfMagnification;
}

void CTexture::SetAutoTexMapping()
{
	//
	//	Initialize texture mapping parameters
	//
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
		GL_MODULATE); // or GL_DECAL (i.e. no lighting)

	//
	//	Set automatic texture coordinates generation if desired.
	//
	GLfloat splane[] = {1.0f, 0.0f, 0.0f, 0.0f}; 
	GLfloat tplane[] = {0.0f, 1.0f, 0.0f, 0.0f};

	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, splane);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, tplane);

	//
	//	Enable automatic texture coordinates generation and texture mapping
	//
    glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);

}

// 
//	Function turn a cv::Mat into a texture, and return the texture ID as a GLuint for use
//
GLuint OpenCVMatToTexture(cv::Mat &mat, GLenum minFilter = GL_LINEAR, 
										GLenum magFilter = GL_LINEAR, 
										GLenum wrapFilter = GL_REPEAT)
{
	// Generate a number for our textureID's unique handle
	GLuint textureID;
	glGenTextures(1, &textureID);
 
	// Bind to our texture handle
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Catch silly-mistake texture interpolation method for magnification
	if (magFilter == GL_LINEAR_MIPMAP_LINEAR  ||
		magFilter == GL_LINEAR_MIPMAP_NEAREST ||
		magFilter == GL_NEAREST_MIPMAP_LINEAR ||
		magFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
		magFilter = GL_LINEAR;
	}
 
	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
 
	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);
 
	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_BGR;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}
 
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
             0,						// Pyramid level (for mip-mapping) - 0 is the top level
             GL_RGB,				// Internal colour format to convert to
             mat.cols,				// Image width  i.e. 640 for Kinect in standard mode
             mat.rows,				// Image height i.e. 480 for Kinect in standard mode
             0,						// Border width in pixels (can either be 1 or 0)
             inputColourFormat,		// Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
             GL_UNSIGNED_BYTE,		// Image data type
             mat.ptr());			// The actual image data itself
 
	// If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
 
	return textureID;
}


/*

//-----------------------------------------------
//
// Name:		LoadTexture2D
//
// Params:	a_sPath - path to the texture image
//
// Result:	Loads texture using FreeImage from a file, supports most
//			graphics formats.
//
//---------------------------------------------

bool CTexture::LoadTexture2D(string a_sPath, bool bGenerateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unkown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if(!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	
	iWidth = FreeImage_GetWidth(dib); // Get the image width and height
	iHeight = FreeImage_GetHeight(dib);
	iBPP = FreeImage_GetBPP(dib);

	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || iWidth == 0 || iHeight == 0)
		return false;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	
	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT; 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	glGenSamplers(1, &uiSampler);

	sPath = a_sPath;
	bMipMapsGenerated = bGenerateMipMaps;

	return true; // Success
}



*/