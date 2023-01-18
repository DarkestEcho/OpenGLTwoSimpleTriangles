#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int createShader(unsigned int shaderType, const char** shaderSource);
unsigned int createShaderProgram(unsigned int shaders[], int count = 0);
void deleteShaders(unsigned int shaders[], int count = 0);

// settings
constexpr unsigned int screenWidth = 800;
constexpr unsigned int screenHeight = 600;
const char* screenTitle = "OpenGL";

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, screenTitle, nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shader program
	// --------------------------------
	const unsigned int vertexShader = createShader(GL_VERTEX_SHADER, &vertexShaderSource);

	const unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);

	// link shaders
	unsigned int shaders[] = { vertexShader, fragmentShader };
	const unsigned int shaderProgram = createShaderProgram(shaders, 2);

	deleteShaders(shaders);

	// vertex data and buffers
	// -----------------------
	constexpr float vertices[] = {
		// first triangle
		-0.9f, -0.8f, 0.0f, // left
		 0.1f, -0.1f, 0.0f, // right
		-0.4f,  0.4f, 0.0f, // top
		// second triangle
		0.0f, 0.4f, 0.0f, // left
		0.8f, 0.8f, 0.0f, // right
		0.4f, -0.6f, 0.0f // top
	};

	// buffers
	unsigned VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw in wireframe polygons
	
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

unsigned createShader(unsigned int shaderType, const char** shaderSource)
{
	const unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, shaderSource, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned createShaderProgram(unsigned int shaders[], int count)
{
	const unsigned int shaderProgram = glCreateProgram();

	if (shaders!= nullptr && count > 0)
	{
		for(int i = 0; i < count; ++i)
		{
			glAttachShader(shaderProgram, shaders[i]);
		}
		glLinkProgram(shaderProgram);

		int success;

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	return shaderProgram;
}

void deleteShaders(unsigned int shaders[], int count)
{
	if (shaders != nullptr && count > 0)
	{
		for (int i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}
	}
}
