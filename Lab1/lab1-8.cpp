
#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr


// Read the code for the shader programs from file
std::string vertex_shader_str = readFile("../lab1-6_vs.glsl");
std::string fragment_shader_str = readFile("../lab1-6_fs.glsl");
GLuint shader_program;

float g_rotation[2] = { 0,0 }; // g_rotation[0] = x_rotation, g_rotation[1] = y_rotation
glm::vec3 camera_pos(0, 0, 2); // LAB 1.7
float camera_rotation[2] = { 0,0 };
float g_rotation2[2] = { 0,0 }; // Lab 1.8


// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::string errorLog;
		errorLog.resize(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		std::cout << "shader compilation failed:" << std::endl;
		std::cout << errorLog << std::endl;
		return;
	}
	else
		std::cout << "shader compilation success." << std::endl;

	return;
}


// Load vertex and fragment shader on demand, and recompile the shader program
void loadShaders(std::string vertex_shader_str,
	std::string fragment_shader_str)
{
	const char* vertex_shader_src = vertex_shader_str.c_str();
	const char* fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
}

// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//-------------------------------------------------------------------------//
	// COPY YOUR CODE FROM BEFORE HERE
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] -= 0.1; //x
	}
	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] += 0.1; //x
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] += 0.1; //y
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] -= 0.1; //y
	}
	// LAB 1.8
	if ((key == GLFW_KEY_F) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation2[0] -= 0.1; //x
	}
	if ((key == GLFW_KEY_H) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation2[0] += 0.1; //x
	}
	if ((key == GLFW_KEY_T) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation2[1] += 0.1; //y
	}
	if ((key == GLFW_KEY_G) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation2[1] -= 0.1; //y
	}

	//-------------------------------------------------------------------------//
	// LAB 1.7
	// Camera translation with WASDQE keys
	if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.z -= 0.1;
	if ((key == GLFW_KEY_S) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.z += 0.1;
	if ((key == GLFW_KEY_A) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.x -= 0.1;
	if ((key == GLFW_KEY_D) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.x += 0.1;
	if ((key == GLFW_KEY_Q) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.y += 0.1;
	if ((key == GLFW_KEY_E) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_pos.y -= 0.1;

	// Camera rotation with IJKL keys
	if ((key == GLFW_KEY_I) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_rotation[1] += 0.1;
	if ((key == GLFW_KEY_K) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_rotation[1] -= 0.1;
	if ((key == GLFW_KEY_J) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_rotation[0] += 0.1;
	if ((key == GLFW_KEY_L) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) camera_rotation[0] -= 0.1;
	//-------------------------------------------------------------------------//
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




int main(int argc, char const* argv[])
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	loadShaders(vertex_shader_str, fragment_shader_str);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//-------------------------------------------------------------------------//
	// COPY FROM lab1-5
	// Geometry, VBO, EBO, VAO
	float points[] = { //1
		// A cube has 8 vertices...
		-0.5f, -0.5f, -0.5f, // vertex 0
		-0.5f, -0.5f,  0.5f, // vertex 1
		-0.5f,  0.5f, -0.5f, // vertex 2
		-0.5f,  0.5f,  0.5f, // vertex 3
		 0.5f, -0.5f, -0.5f, // vertex 4
		 0.5f, -0.5f,  0.5f, // vertex 5
		 0.5f,  0.5f, -0.5f, // vertex 6
		 0.5f,  0.5f,  0.5f  // vertex 7
	};

	float points2[] = { //Lab 1.8
		// A cube has 8 vertices...
		-0.0f, -0.5f, -0.5f, // vertex 0
		-0.0f, -0.5f,  0.5f, // vertex 1
		-0.0f,  0.5f, -0.5f, // vertex 2
		-0.0f,  0.5f,  0.5f, // vertex 3
		 1.0f, -0.5f, -0.5f, // vertex 4
		 1.0f, -0.5f,  0.5f, // vertex 5
		 1.0f,  0.5f, -0.5f, // vertex 6
		 1.0f,  0.5f,  0.5f  // vertex 7
	};

	unsigned short faces[] = {
		// ... and 12 triangular faces, defined by these vertex indices (counter-clockwise):
		0, 3, 2,  0, 1, 3,  // left face
		4, 6, 5,  5, 6, 7,  // right face
		0, 2, 4,  2, 6, 4,  // back face
		1, 5, 3,  3, 5, 7,  // front face
		2, 3, 7,  2, 7, 6,  // top face
		0, 4, 1,  1, 4, 5   // bottom face

	};

	GLuint vao; //2
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint vbo2;
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0); // need this twice?
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	GLuint ebo; //3
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------//


	while (!glfwWindowShouldClose(window))
	{
		// Update other events like keyboard/mouse input.
		glfwPollEvents();
		// Clear the drawing surface before drawing something new.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize(window, &w_width, &w_height);

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		//
		// Replace your hand-crafted matrices from lab1-5.cpp with ones
		// created by glm.
		//
		// Use glm::perspective to create a projection matrix
		//1
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w_width / (float)w_height, 1.0f, 100.0f); //all values have to be float
		// Use glm::translate and glm::rotate to create the
		// model and view matrices.
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1), -camera_pos); //LAB 1.7
		// Translation first, then rotation
		// Rotation first, then translation results in the camera orbiting around the origin
		viewMatrix = glm::rotate(viewMatrix, camera_rotation[1], glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, camera_rotation[0], glm::vec3(0, 1, 0));


		glm::vec3 rotate(g_rotation[0], g_rotation[1], 0);
		glm::mat4 modelMatrix = glm::rotate(glm::mat4(1), glm::radians(rotate.x), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate.y), glm::vec3(0, 1, 0));
		// Multiply your matrices in the correct order to get a
		// modelViewProjection matrix and upload it to the appropriate
		// uniform variable in vertex shader.
		glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

		GLuint mvp_location = glGetUniformLocation(shader_program, "mvp"); //2
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

		// -----------------------------------------------------------------------//

		//-----------------------------------------------------------------------//
		// Call glDrawElements as before
		glBindVertexArray(vao); //4
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		// LAB 1.8 2nd cube
		glm::vec3 rotate2(g_rotation2[0], g_rotation2[1], 0);
		glm::mat4 modelMatrix2 = glm::rotate(glm::mat4(1), glm::radians(rotate2.x), glm::vec3(1, 0, 0));
		modelMatrix2 = glm::rotate(modelMatrix2, glm::radians(rotate2.y), glm::vec3(0, 1, 0));

		glm::mat4 modelViewProjectionMatrix2 = projectionMatrix * viewMatrix * modelMatrix2;

		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix2));

		glBindVertexArray(vao2); //4
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

