#include <GL/glew.h>
#include <cmath>
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "lodepng.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string vertex_shader_str = readFile("../lab3-4_vs.glsl");
std::string fragment_shader_str = readFile("../lab3-4_fs.glsl");
GLuint shader_program;
GLuint shadow_shader_program;

float x_rotation = 0;
float y_rotation = 0;

void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

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

void loadShaders(std::string vertex_shader_str, std::string fragment_shader_str)
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

void loadShadowShaders(std::string vertex_shader_str, std::string fragment_shader_str)
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

	shadow_shader_program = glCreateProgram();
	glAttachShader(shadow_shader_program, fs);
	glAttachShader(shadow_shader_program, vs);
	glLinkProgram(shadow_shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shadow_shader_program);
}

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT) y_rotation -= 0.05f;
		if (key == GLFW_KEY_RIGHT) y_rotation += 0.05f;
		if (key == GLFW_KEY_UP) x_rotation += 0.05f;
		if (key == GLFW_KEY_DOWN) x_rotation -= 0.05f;
	}
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

	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Shadow Mapping", NULL, NULL);
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
	loadShadowShaders(vertex_shader_str, "void main() {}");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Geometry setup code here (skipped)

	// Shadow map setup
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		// 1. Render depth of scene to texture (from light's perspective)
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

		glUseProgram(shadow_shader_program);
		glUniformMatrix4fv(glGetUniformLocation(shadow_shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		// Render scene from light's point of view
		// Geometry rendering code here (skipped)

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Render scene as normal using the generated depth/shadow map
		glViewport(0, 0, w_width, w_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)w_width / (float)w_height, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(x_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(y_rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		glUseProgram(shader_program);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniform3fv(glGetUniformLocation(shader_program, "lightPos"), 1, glm::value_ptr(glm::vec3(-2.0f, 4.0f, -1.0f)));

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shader_program, "shadowMap"), 1);

		// Render scene
		// Geometry rendering code here (skipped)

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo_normals);
	glDeleteBuffers(1, &vbo_tangents);
	glDeleteBuffers(1, &vbo_bitangents);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
