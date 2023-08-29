#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPenGl Error](" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}