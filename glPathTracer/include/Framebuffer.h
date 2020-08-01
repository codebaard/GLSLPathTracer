/*
The purpose for this class is handle the framebuffer state and data within openGL.
Every Instance represents a single framebuffer with a arbitrary settings and purposes.

written by Julius Neudecker
v0.1 30.07.2020 - created.
v0.2 31.07.2020 - Added Functionality for fetching Compute Shader Buffer

*/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glfwHandler.h>
#include <jLog.h>
#include <Shader.h>

#define RENDERBUFFER_SIZE_WIDTH 512
#define RENDERBUFFER_SIZE_HEIGHT 512


class Framebuffer {
public:
	//set default values since the framebuffer is normally intended to render texture output.
	Framebuffer(unsigned int bufferType = GL_FRAMEBUFFER, unsigned int attachmentUnit = GL_COLOR_ATTACHMENT0);
	
	void EnableRenderToTexture();
	void DisableRenderToTexture(); //Disable after Rendering!
	void ShowRenderedTexture();
	void SetShaderProgram(const char* cwd, const char* vertexShader, const char* fragmentShader);
	
	void InitImageTexture();
	void ActivateImageTexture();
	void FetchTexture();


	~Framebuffer();

private:
	void _attachTexture();
	void _initQuadArray();
	unsigned int _FBO;
	unsigned int _RBO;
	unsigned int _bufferType;
	unsigned int _attachmentUnit;
	unsigned int _targetTextureID;
	unsigned int _computeImageTextureID;
	Shader* _renderer;

	//for rendering fullscreen quad
	unsigned int _quadVAO, _quadVBO;
	float _quadVertexArray[24] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
};

#endif
