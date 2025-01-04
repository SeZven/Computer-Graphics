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
//GLuint shadow_shader_program;

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
/*
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
*/

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
	//loadShadowShaders(vertex_shader_str, "void main() {}");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Geometry setup code
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

	// Lab 3-3 (tangents and bitangents)
	std::vector<glm::vec3> tangents(num_verts);
	std::vector<glm::vec3> bitangents(num_verts);

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

		// Lab 3-3 uv coordinates
		glm::vec2 uv0(uv_coords[i0 * 2], uv_coords[i0 * 2 + 1]);
		glm::vec2 uv1(uv_coords[i1 * 2], uv_coords[i1 * 2 + 1]);
		glm::vec2 uv2(uv_coords[i2 * 2], uv_coords[i2 * 2 + 1]);


		// Calculating the vectors
		glm::vec3 v1 = p1 - p0;
		glm::vec3 v2 = p2 - p0;

		// Lab 3-3
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (v1 * deltaUV2.y - v2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (v2 * deltaUV1.x - v1 * deltaUV2.x) * r;


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

		// Lab 3-3
		tangents[i0] += tangent;
		tangents[i1] += tangent;
		tangents[i2] += tangent;

		bitangents[i0] += bitangent;
		bitangents[i1] += bitangent;
		bitangents[i2] += bitangent;

	}

	// Normalizing the normals for each vertex
	for (size_t i = 0; i < sizeof(points) / sizeof(points[0]); i += 3) {
		glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
		normal = glm::normalize(normal);

		normals[i] = normal.x;
		normals[i + 1] = normal.y;
		normals[i + 2] = normal.z;
	}

	// Lab 3-3
	for (size_t i = 0; i < num_verts; ++i) {
		tangents[i] = glm::normalize(tangents[i]);
		bitangents[i] = glm::normalize(bitangents[i]);
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

	// Lab 3-3
	// Create and upload Tangent buffer
	GLuint vbo_tangents;
	glGenBuffers(1, &vbo_tangents);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tangents);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	// Create and upload Bitangent buffer
	GLuint vbo_bitangents;
	glGenBuffers(1, &vbo_bitangents);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangents);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), bitangents.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//-------------------------------------------------------------------------//
	// Room geometry (cube with inward-facing normals)
	float room_vertices[] = {
		// positions          // normals
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
	};

	GLuint vao_room, vbo_room;
	glGenVertexArrays(1, &vao_room);
	glGenBuffers(1, &vbo_room);

	glBindVertexArray(vao_room);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_room);
	glBufferData(GL_ARRAY_BUFFER, sizeof(room_vertices), room_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);


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

		//glUseProgram(shadow_shader_program);
		glUseProgram(shader_program);
		//glUniformMatrix4fv(glGetUniformLocation(shadow_shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

        
		// Render scene from light's point of view
		// Geometry rendering code
		// Render the icosahedron
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);

		// Render the room
		glBindVertexArray(vao_room);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
		// Geometry rendering code
		// Render the icosahedron
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);

		// Render the room
		glBindVertexArray(vao_room);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
