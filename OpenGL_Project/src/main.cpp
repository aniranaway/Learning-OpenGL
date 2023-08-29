#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}

		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}
 
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	int success;
	char infoLog[512];

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main()
{
	/* Initialize GLFW */
	if (!glfwInit())
		return -1;



	/* Provide Versions and profile for  OpenGL 3.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create window with error handling */
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	/*Set window as current context*/
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	/* Initialize GLEW */

	if (glewInit() != GLEW_OK)
		std::cout << "Error\n";

	std::cout << glGetString(GL_VERSION) << std::endl;

	


	/* Provide data for drawing*/
	{
		float vertices[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,

		};

		unsigned int indices[] = { 0, 1, 2,
								  2, 3, 0 };

		unsigned int VAO;
		

		VertexBuffer vb(vertices, sizeof(vertices));

		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);


		IndexBuffer ib(indices, 6);

		ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
		std::cout << "VERTEX" << std::endl;
		std::cout << source.VertexSource << std::endl;

		std::cout << "FRAGMENT" << std::endl;
		std::cout << source.FragmentSource << std::endl;

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader);

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1)
			GLCall(glUniform4f(location, 1.0f, 0.0f, 0.0f, 0.0f));

		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		va.Unbind();



		float r = 0.0f;
		float increment = 0.05f;
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);
			GLCall(glUniform4f(location, r, 0.8f, 0.6f, 0.0f));

			va.Bind();
			ib.Bind();



			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

			if (r > 1.0f)
				increment = -0.05f;

			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/*Swap front and back buffers*/
			glfwSwapBuffers(window);
			/* Polls and processes events*/
			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}