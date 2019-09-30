/* GLEW*/
#include <GL/glew.h>

/* GLFW */
#include <GLFW/glfw3.h>

/* GLM*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Standard */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* Additional files */
#include "Shaders.cpp"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 550

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Exercise #3", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Couldn't load GLEW." << std::endl;
	}
		
	/* Prints current openGL version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[6] =
	{
		-0.2f, -0.2f,
		 0.0f,  0.2f,
		 0.2f, -0.2f
	};
	
	/* buffer ID*/
	unsigned int buffer; 

	/* Generate a buffer*/
	glGenBuffers(1, &buffer);

	/* select buffer*/
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	/* Specifies buffer data and size*/
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	
	Shaders::ShaderProgramSource source = Shaders::ParseShader("res/shaders/Basic_s.shader");
	unsigned int shader = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	std::cout << "VERTEX\n" << std::endl;
	std::cout << source.VertexSource << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		double value = sin(glfwGetTime()) / 2.0f + 0.5f;
		
		/* Render here */
		glClearColor(0.2f, 0.3f, (float) sin(glfwGetTime()) / 2.0f + 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(value - 0.6f, 0.0f, 0.6f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(sin(glfwGetTime()) / 2, 0.6, 0.8));

		/* update position (transform position) */
		unsigned int uniTrans = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(transform));

		/* update color */
		unsigned int uniColor = glGetUniformLocation(shader, "changeColor");
		glUniform3f(uniColor, 0.8, (float) value, 0.3);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */	
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}