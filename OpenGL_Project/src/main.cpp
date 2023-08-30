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
#include "Shader.h"
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


		Shader shader("res/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform("u_Color", 1.0f, 0.0f, 0.0f, 0.0f);
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();
		va.Unbind();



		float r = 0.0f;
		float increment = 0.05f;
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			shader.SetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);

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
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}