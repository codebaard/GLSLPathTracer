#include <Framebuffer.h>

Framebuffer::Framebuffer(unsigned int bufferType, unsigned int attachmentUnit) {

	_bufferType = bufferType;
	_attachmentUnit = attachmentUnit;

	glGenFramebuffers(1, &_FBO);

	switch (bufferType) {
	case GL_READ_FRAMEBUFFER:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	case GL_DRAW_BUFFER:
		glBindFramebuffer(GL_DRAW_BUFFER, _FBO);
	default: //GL_FRAMEBUFFER:
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	}

	_attachTexture();
	_initQuadArray();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		jLog::Instance()->Log(INFO, "Framebuffer created successfully.");
	}	
	else 
		jLog::Instance()->Log(WARNING, "Framebuffer creation failed.");
}

void Framebuffer::DisableRenderToTexture() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::EnableRenderToTexture() {
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Do not use the stencil buffer
	glEnable(GL_DEPTH_TEST);
	
	//Framebuffer is set up.
	//Every following render-call writes to this framebuffer now  
	//until framebuffer is disabled by calling DisableRenderToTexture()!
}

void Framebuffer::ShowRenderedTexture() {

	_renderer->use();
	_renderer->setInt("ScreenTexture", 0);

	glBindVertexArray(_quadVAO);

	glDisable(GL_DEPTH_TEST); //no depth testing, when rendering fullscreen quad

	glActiveTexture(GL_TEXTURE0); //set to default but explicitly
	glBindTexture(GL_TEXTURE_2D, _targetTextureID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::_attachTexture() {
	//using default GL_TEXTURE0 tex-unit here. 
	
	glGenTextures(1, &_targetTextureID);

	glActiveTexture(GL_TEXTURE0); //set to default but explicitly
	glBindTexture(GL_TEXTURE_2D, _targetTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RENDERBUFFER_SIZE_WIDTH, RENDERBUFFER_SIZE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	//memory allocated. Unbind for predictable behavior.
	glBindTexture(GL_TEXTURE_2D, 0); //unbdind from internal texture unit (default: GL_TEXTURE0)

	glFramebufferTexture2D(_bufferType, _attachmentUnit, GL_TEXTURE_2D, _targetTextureID, 0);

	//creating depth and stencil buffer
	glGenRenderbuffers(1, &_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RENDERBUFFER_SIZE_WIDTH, RENDERBUFFER_SIZE_HEIGHT);
	
	//memory allocated. Unbind for predictable behavior.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//last steps
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		jLog::Instance()->Error("Framebuffer initialization failed!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::ActivateImageTexture() {
	//glBindImageTexture(0, _targetTextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindImageTexture(0, _targetTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Framebuffer::_initQuadArray() {
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertexArray), &_quadVertexArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Framebuffer::SetShaderProgram(std::string cwd, const char* vertexShaderProgram, const char* fragmentShaderProgram) {
	_renderer = new Shader();
	_renderer->AddShaderToPipeline(cwd, vertexShaderProgram, VERTEX);
	_renderer->AddShaderToPipeline(cwd, fragmentShaderProgram, FRAGMENT);

	try{
		_renderer->InitShader();
		jLog::Instance()->Log(INFO, "Framebuffer shader built successfully.");
	}
	catch (std::exception e)
	{
		jLog::Instance()->Error(std::string("Building fsQuadShader failed: " + std::string(e.what())));
		throw; //up the stack!
	}
}

void Framebuffer::FetchTexture() {
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &_FBO);
}