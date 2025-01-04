#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "lodepng.h"

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr


// Read the code for the shader programs from file
std::string vertex_shader_str = readFile("../lab3-3_vs.glsl");
std::string fragment_shader_str = readFile("../lab3-3_fs.glsl");
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

	float t = (1.0f + sqrtf(5.0f)) * 0.25f;
	int num_faces = 20;
	int num_verts = 60;

	float points[] = {
		// An icosahedron without shared vertices has 20*3 vertices
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,

		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,

		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,

		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,

		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,

		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,

		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,

		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,

		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,

		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,

		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,

		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5
	};

	float uv_coords[] =
	{
	  0.072405, 0.144686,
	  0.27133,  0.10033,
	  0.481007, 0.129901,
	  0.681948, 0.115116,
	  0.928587, 0.107723,
	  0.29888,  0.70651,
	  0.449421, 0.736084,
	  0.108695, 0.129901,
	  0.847270, 0.699121,
	  0.868776, 0.728691,
	  0.576437, 0.721299,
	  0.319044, 0.750869,
	  0.548883, 0.736084,
	  0.388265, 0.115116,
	  0.397674, 0.713906,
	  0.149690, 0.122508,
	  0.347942, 0.115116,
	  0.583157, 0.107723,
	  0.780738, 0.100331,
	  0.229663, 0.107723,
	  0.33436,  0.68882,
	  0.181276, 0.721299,
	  0.379528, 0.721299,
	  0.190684, 0.122508,
	  0.358023, 0.736084,
	  0.103319, 0.713906,
	  0.24512,  0.70651,
	  0.083157, 0.699121,
	  0.31006,  0.10251,
	  0.222942, 0.721299,
	  0.772001, 0.728691,
	  0.618776, 0.721299,
	  0.745120, 0.721299,
	  0.433292, 0.115116,
	  0.598614, 0.713906,
	  0.520657, 0.721299,
	  0.650362, 0.736084,
	  0.700765, 0.728691,
	  0.878184, 0.107723,
	  0.671867, 0.743476,
	  0.914475, 0.743476,
	  0.819717, 0.736084,
	  0.201437, 0.721299,
	  0.633561, 0.122508,
	  0.796195, 0.743476,
	  0.140953, 0.728691,
	  0.122136, 0.706514,
	  0.262593, 0.721299,
	  0.157754, 0.728691,
	  0.825765, 0.115116,
	  0.723614, 0.728691,
	  0.897001, 0.721299,
	  0.499152, 0.736084,
	  0.731007, 0.115116,
	  0.469582, 0.721299,
	  0.045523, 0.691729,
	  0.967566, 0.721299,
	  0.28341,  0.67157,
	  0.532754, 0.122508,
	  0.418507, 0.706514,
	};

	int num_indices = num_faces * 3;
	unsigned short faces[] = {
		// ... and 20 triangular faces, 
		// defined by the following vertex indices:
		0,  55, 25,
		1,  26,  5,
		2,  6, 35,
		3,  36, 50,
		4,  51, 56,
		7,  27, 45,
		28, 57, 20,
		58, 52, 10,
		53, 37, 30,
		38,  8, 40,
		15, 46, 21,
		16, 22, 11,
		17, 12, 31,
		18, 32, 41,
		19, 42, 47,
		23, 48, 29,
		13, 24, 59,
		33, 14, 54,
		43, 34, 39,
		49, 44, 9
	};



	float normals[sizeof(points) / sizeof(points[0])];
	//Normals array creation

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
		glm::vec3 p0(points[i0 * 3], points[i0 * 3 + 1], points[i0 * 3 + 2]);
		glm::vec3 p1(points[i1 * 3], points[i1 * 3 + 1], points[i1 * 3 + 2]);
		glm::vec3 p2(points[i2 * 3], points[i2 * 3 + 1], points[i2 * 3 + 2]);


		// Calculating the vectors
		glm::vec3 v1 = p1 - p0;
		glm::vec3 v2 = p2 - p0;

		// Calculating the normal of the triangle
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));  // Cross product v1.v2 or (p1-p0).(p2-p0)

		// Adding the calculated normal to each vertex of the triangle
		normals[i0 * 3] += normal.x;
		normals[i0 * 3 + 1] += normal.y;
		normals[i0 * 3 + 2] += normal.z;

		normals[i1 * 3] += normal.x;
		normals[i1 * 3 + 1] += normal.y;
		normals[i1 * 3 + 2] += normal.z;

		normals[i2 * 3] += normal.x;
		normals[i2 * 3 + 1] += normal.y;
		normals[i2 * 3 + 2] += normal.z;

	}

	// Normalizing the normals for each vertex
	for (size_t i = 0; i < sizeof(points) / sizeof(points[0]); i += 3) {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// Index 0: 3 components for the position (x, y, z)

	// Creating and filling the VBO (normals)
	GLuint vbo_normals;
	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1); // Utilisez 1 pour les normales (à adapter selon le shader)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);



	// Creating and filling the EBO (defines the vertex indices that form the triangles of the cube)
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//

	// Lab 3-3 (texture to normal_map_handle)
	GLuint normal_map_handle; // normal map handle
	glGenTextures(1, &normal_map_handle); // from 0 to 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_map_handle);

	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "normal_sampler"), 1);

	unsigned char* image_data;
	unsigned image_w, image_h;
	unsigned err_code = lodepng_decode32_file(&image_data, &image_w, &image_h, "../../common/data/numberline_hires.png");
	if (err_code) {
		std::cerr << "Error loading texture: " << lodepng_error_text(err_code) << std::endl;
		return -1;
	}
	std::cout << "Read " << image_h << " x " << image_w << " image\n";

	// Upload the texture to the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Create and upload UV buffer
	GLuint vbo_uv;
	glGenBuffers(1, &vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);

	// Enable and set the UV attribute pointer (location 2)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);


	/*
	//3-2 to 3-3 (replaced texture_handle with normal_map_handle)
	glGenTextures(1, &normal_map_handle);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, normal_map_handle);

	glUseProgram(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, "env_sampler"), 1);

	// Cube's faces
	const char* cube_faces[6] = {
		"../../common/data/cube_right.png",   // +X
		"../../common/data/cube_left.png",    // -X
		"../../common/data/cube_up.png",      // +Y
		"../../common/data/cube_down.png",    // -Y
		"../../common/data/cube_front.png",   // +Z
		"../../common/data/cube_back.png"     // -Z
	};

	// Faces textures loading
	for (unsigned int i = 0; i < 6; ++i) {
		unsigned char* image_data = nullptr;
		unsigned image_w = 0, image_h = 0;

		unsigned err_code = lodepng_decode32_file(&image_data, &image_w, &image_h, cube_faces[i]);
		if (err_code) {
			std::cerr << "Erreur de chargement de la texture : "
				<< lodepng_error_text(err_code)
				<< " (" << cube_faces[i] << ")" << std::endl;
			return -1;
		}

		if (image_w != image_h) {
			std::cerr << "Erreur : La texture " << cube_faces[i]
				<< " n'est pas carrée (" << image_w << "x" << image_h << ")" << std::endl;
			free(image_data);
			return -1;
		}

		std::cout << "Texture chargée : " << cube_faces[i]
			<< " (" << image_w << "x" << image_h << ")" << std::endl;


		// Texture transfer
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data
		);

		free(image_data);
	}

	// Define texture cube map parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	std::cout << "Cube map chargé avec succès." << std::endl;
	*/

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
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
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
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


		// Draw the cube using the indices in the EBO
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo_normals);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);

}









