#include <GL/glew.h>
#include <cmath>
#include <fstream>
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
std::string vertex_shader_str = readFile("../lab2_vs.glsl");
std::string fragment_shader_str = readFile("../lab2-2_fs.glsl");
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
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
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

double lambertian_brdf(glm::vec3 in_direction, glm::vec3 normal) {
	// Normalize the vectors
	in_direction = glm::normalize(in_direction);
	normal = glm::normalize(normal);

	// Compute the cosine of the angle between the normal and the incoming direction
	double cos_theta = glm::dot(normal, in_direction);
	//std::cout << "cos_theta: " << cos_theta << std::endl;

	// Return the BRDF value: rho_d / pi (rho_d assumed to be 1 for simplicity)
	return std::max(cos_theta, 0.0) / 3.14159; // Ensure non-negative with max()
}

int plotting() {
	// Set up parameters
	glm::vec3 normal(0, 0, 1); // Normal pointing in the Z direction
	glm::vec3 in_direction(0, 0, 1);

	// Open a CSV file to save results
	std::ofstream outputFile("brdf_plot_data.csv");
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file for output!" << std::endl;
		return -1;
	}

	outputFile << "angle,brdf_value\n"; // CSV header

	// Loop over input angles
	for (int angle = -90; angle <= 90; ++angle) {
		// Convert angle to radians
		double radians = glm::radians(static_cast<double>(angle));

		// Compute the incoming direction in the XZ plane
		glm::vec3 in_direction(0, 0, cos(radians));

		// Compute the Lambertian BRDF value
		double brdf_value = lambertian_brdf(in_direction, normal);

		// Write to file
		outputFile << angle << "," << brdf_value << "\n";
	}

	outputFile.close();
	std::cout << "BRDF data written to brdf_plot_data.csv" << std::endl;

	return 0;
}



int main(int argc, char const* argv[])
{
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
	plotting();
	//-------------------------------------------------------------------------//
	// COPY FROM lab1-5
	// Geometry, VBO, EBO, VAO
	   // Cube vertex definition

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


	unsigned short faces[] = {
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

	float normals[sizeof(points) / sizeof(points[0])];    // Create the normals array

	// Initialize the normals array to zero
	for (size_t i = 0; i < sizeof(normals) / sizeof(normals[0]); i++) {
		normals[i] = 0.0f;
	}

	// Calculate the normals for each triangle
	for (size_t i = 0; i < sizeof(faces) / sizeof(faces[0]); i += 3) {
		// Indices of the vertices
		unsigned short i0 = faces[i];
		unsigned short i1 = faces[i + 1];
		unsigned short i2 = faces[i + 2];

		// Extract the coordinates of the vertices (x, y, z)
		glm::vec3 p0(points[i0 * 4], points[i0 * 4 + 1], points[i0 * 4 + 2]);
		glm::vec3 p1(points[i1 * 4], points[i1 * 4 + 1], points[i1 * 4 + 2]);
		glm::vec3 p2(points[i2 * 4], points[i2 * 4 + 1], points[i2 * 4 + 2]);

		// Calculate the vectors
		glm::vec3 v1 = p1 - p0;
		glm::vec3 v2 = p2 - p0;

		// Calculate the normal of the triangle
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));    // Cross product v1.v2 or (p1-p0).(p2-p0)

		// Add the calculated normal to each vertex of the triangle
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

	// Normalize the normals for each vertex
	for (size_t i = 0; i < sizeof(points) / sizeof(points[0]); i += 4) {
		glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
		normal = glm::normalize(normal);

		normals[i] = normal.x;
		normals[i + 1] = normal.y;
		normals[i + 2] = normal.z;
	}

	// Create and bind the VAO (positions, normals, texture coordinates)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and fill the VBO (vertex coordinates)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Attribute for position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	// Index 0: 3 components for position (x, y, z)
	// A vertex = 4 coordinates so 4 * sizeof...
	// offset = 0 because positions start at the beginning of the data

	// Attribute for w (index 1)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	// Index 1: 1 component for w
	// same
	// Here the offset = 3 because we start after x, y, z for the normal


	// Create and fill the VBO (normals)
	GLuint vbo_normals;
	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);


	// Create and fill the EBO (defines the indices of the vertices forming the cube's triangles)
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);




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

		// Light parameters
		int light_count = 5;
		glm::vec3 lightPosition = glm::vec3(1.0f, 1.0f, 1.0f); // Light position
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White color

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

		// Light parameters 
		GLuint lightcountLocation = glGetUniformLocation(shader_program, "light_count");
		GLuint lightPosLocation = glGetUniformLocation(shader_program, "lightPosition");
		GLuint lightColorLocation = glGetUniformLocation(shader_program, "lightColor");

		// Send the matrices with `GL_TRUE` to transpose if necessary
		glUniformMatrix4fv(modelLoc, 1, GL_TRUE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projLoc, 1, GL_TRUE, glm::value_ptr(projectionMatrix));

		// Light parameters 
		glUniform1i(lightcountLocation, light_count);
		glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition)); // Send the position
		glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));  // Send the color

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






