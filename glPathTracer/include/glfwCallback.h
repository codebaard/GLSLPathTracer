#ifndef GLFWCALLBACK_H
#define GLFWCALLBACK_H

#include <glfwHandler.h>
#include <jLog.h>

void APIENTRY _glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::string err = "";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             err += "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   err += "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: err += "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     err += "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     err += "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           err += "Source: Other"; break;
	} err += " : ";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               err += "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: err += "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  err += "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         err += "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         err += "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              err += "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          err += "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           err += "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               err += "Type: Other"; break;
	} err += " : ";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         err += "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       err += "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          err += "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: err += "Severity: notification"; break;
	}

	jLog::Instance()->glCallbackError(err, id);
	jLog::Instance()->glError(message);

}

#endif 


