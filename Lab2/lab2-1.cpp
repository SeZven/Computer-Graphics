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
std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");
GLuint shader_program;

float x_rotation = 0;
float y_rotation = 0;


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
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT) y_rotation -= 0.05f;
		if (key == GLFW_KEY_RIGHT) y_rotation += 0.05f;
		if (key == GLFW_KEY_UP) x_rotation += 0.05f;
		if (key == GLFW_KEY_DOWN) x_rotation -= 0.05f;

	}
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
	 
	
// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.

float points[] = {
	// A cube has 8 vertices, but now we have three copies of each one:
  -0.5, -0.5, -0.5, 1, //0 0
	-0.5, -0.5, -0.5, 1, //0 1
	-0.5, -0.5, -0.5, 1, //0 2
	//
  -0.5, -0.5,  0.5, 1, //1 3
  -0.5, -0.5,  0.5, 1, //1 4
  -0.5, -0.5,  0.5, 1, //1 5
	//
  -0.5,  0.5, -0.5, 1, //2 6
  -0.5,  0.5, -0.5, 1, //2 7
  -0.5,  0.5, -0.5, 1, //2 8
	//
  -0.5,  0.5,  0.5, 1, //3 9
  -0.5,  0.5,  0.5, 1, //3 10
  -0.5,  0.5,  0.5, 1, //3 11
	//
  0.5, -0.5, -0.5, 1, //4 12
  0.5, -0.5, -0.5, 1, //4 13
  0.5, -0.5, -0.5, 1, //4 14
	//
  0.5, -0.5,  0.5, 1, //5 15
  0.5, -0.5,  0.5, 1, //5 16
  0.5, -0.5,  0.5, 1, //5 17
	//
  0.5,  0.5, -0.5, 1, //6 18
  0.5,  0.5, -0.5, 1, //6 19
  0.5,  0.5, -0.5, 1, //6 20
	//
  0.5,  0.5,  0.5, 1, //7 21
  0.5,  0.5,  0.5, 1, //7 22
  0.5,  0.5,  0.5, 1, //7 23
};



unsigned short faces[]= {
	// ... and 12 triangular faces,
	// defined by the following vertex indices:
	0, 9, 6, // 0 3 2
	0, 3, 9, // 0 1 3
	//
	1, 7, 18, // 0 2 6
	1, 18, 12, // 0 6 4
	//
	13, 19, 15, // 4 6 5
	15, 19, 21, // 5 6 7
	//
	16, 22, 10, // 5 7 3
	16, 10, 4, // 5 3 1
	//
	8, 11, 23, // 2 3 7
	8, 23, 20, // 2 7 6
	//
	2, 14, 5, // 0 4 1
	5, 14, 17 // 1 4 5
};

float normals[sizeof(points) / sizeof(points[0])];	//Normals array creation

// Initialisation
for (size_t i = 0; i < sizeof(normals) / sizeof(normals[0]); i++) {
	normals[i] = 0.0f;
}

// Normals calculus for each triangle
for (size_t i = 0; i < sizeof(faces) / sizeof(faces[0]); i += 3) {
	// Vertex indices
	unsigned short i0 = faces[i];
	unsigned short i1 = faces[i + 1];
	unsigned short i2 = faces[i + 2];

	// Extracting the coordinates of the vertices (x, y, z)
	glm::vec3 p0(points[i0 * 4], points[i0 * 4 + 1], points[i0 * 4 + 2]);
	glm::vec3 p1(points[i1 * 4], points[i1 * 4 + 1], points[i1 * 4 + 2]);
	glm::vec3 p2(points[i2 * 4], points[i2 * 4 + 1], points[i2 * 4 + 2]);

	// Calculating the vectors
	glm::vec3 v1 = p1 - p0;
	glm::vec3 v2 = p2 - p0;

	// Calculating the normal of the triangle
	glm::vec3 normal = glm::normalize(glm::cross(v1, v2));  // Cross product v1.v2 or (p1-p0).(p2-p0)

	// Adding the calculated normal to each vertex of the triangle
	normals[i0 * 4] += normal.x;
	normals[i0 * 4 + 1] += normal.y;
	normals[i0 * 4 + 2] += normal.z;

	normals[i1 * 4] += normal.x;
	normals[i1 * 4 + 1] += normal.y;
	normals[i1 * 4 + 2] += normal.z;

	normals[i2 * 4] += normal.x;
	normals[i2 * 4 + 1] += normal.y;
	normals[i2 * 4 + 2] += normal.z;
}

// Normalizing the normals for each vertex
for (size_t i = 0; i < sizeof(points) / sizeof(points[0]); i += 4) {
	glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
	normal = glm::normalize(normal);

	normals[i] = normal.x;
	normals[i + 1] = normal.y;
	normals[i + 2] = normal.z;
}


// Creating and binding the VAO (position, normals, texture coordinates)
GLuint vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);

// Creating and filling the VBO (vertex coordinates)
GLuint vbo;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

// Attribute for the position
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
// Index 0: 3 components for the position (x, y, z)
// A vertex = 4 coordinates, so 4 * sizeof...
// offset = 0 since positions start at the beginning of the data

// Creating and filling the VBO (normals)
GLuint vbo_normals;
glGenBuffers(1, &vbo_normals);
glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);


// Creating and filling the EBO (defines the vertex indices that form the triangles of the cube)
GLuint ebo;
glGenBuffers(1, &ebo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
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
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), x_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));

	//
	// Use glm::perspective to create a projection matrix
	const float fov = 0.7854f;           // 45 degrees in radians       fov and aspect are other options that are not in the course 
	const float aspect = 1.0f;
	const float near = 1.0f;             // near and far are fundamental 
	const float far = 10.0f;
	glm::mat4 projectionMatrix = glm::perspective(0.7854, 1.0, 1.0, 10.0); // fov, aspect (width and height ratio of the window), near, far

	//
	// Use glm::translate and glm::rotate to create the
	// model and view matrices.
	modelMatrix = rotate_y * rotate_x;

	//
	// Multiply your matrices in the correct order to get a
	// modelViewProjection matrix and upload it to the appropriate
	// uniform variable in the vertex shader.
	// 
	// I DO IT IN THE VERTEX SHADER 
	// -----------------------------------------------------------------------//

	//-----------------------------------------------------------------------//
	// Call glDrawElements as before
	// After loading and linking the shader program
	GLint modelLoc = glGetUniformLocation(shader_program, "modelMatrix");
	GLint viewLoc = glGetUniformLocation(shader_program, "viewMatrix");
	GLint projLoc = glGetUniformLocation(shader_program, "projectionMatrix");

	// Send the matrices with `GL_TRUE` to transpose if necessary
	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, glm::value_ptr(projectionMatrix));


	// Draw the cube using the indices in the EBO
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	//-----------------------------------------------------------------------//

	glfwSwapBuffers(window);
}

// close GL context and any other GLFW resources
glfwTerminate();
exit(EXIT_SUCCESS);
}






