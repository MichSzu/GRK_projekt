#include "Shader_Loader.h"

Core::Shader_Loader shaderLoader;
GLuint program;

int WIDTH = 500, HEIGHT = 500;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}