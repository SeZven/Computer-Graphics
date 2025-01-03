// This code only shows how to load an object. Input this into your
// program to also draw it.

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <cmath>
#include <fstream>
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Include the GLM library for easier manipulation of transformations
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr

// Read the code for the shader programs from file
// Lab 2-4
//std::string vertex_shader_str = readFile("../lab2-2_vs.glsl");
//std::string fragment_shader_str = readFile("../lab2-4_fs.glsl");
// Lab 2-5
//std::string vertex_shader_str = readFile("../lab2-5_vs.glsl");
//std::string fragment_shader_str = readFile("../lab2-5_fs.glsl");
// Lab 2-6
std::string vertex_shader_str = readFile("../lab2-6_vs.glsl");
std::string fragment_shader_str = readFile("../lab2-6_fs.glsl");
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

// Callback for keyboard events
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

// Blinn-Phong BRDF implementation
double blinn_phong_brdf(glm::vec3 in_direction, glm::vec3 out_direction, glm::vec3 normal) {
	// Normalize vectors
	glm::vec3 L = glm::normalize(in_direction);  // Light direction
	glm::vec3 V = glm::normalize(out_direction); // Viewer's direction
	glm::vec3 N = glm::normalize(normal);        // Surface normal

	// BRDF parameters
	glm::vec3 rhoL = glm::vec3(0.8f, 0.8f, 0.8f); // Diffuse albedo (light gray)
	glm::vec3 rhoG = glm::vec3(1.0f, 1.0f, 1.0f); // Specular albedo (perfect white)
	float kL = 0.6f;    // Diffuse contribution
	float kG = 0.4f;    // Specular contribution
	float s = 200.0f;   // Shininess (sharpness of the specular peak)

	// Compute the half-angle vector
	glm::vec3 H = glm::normalize(L + V);

	// Diffuse contribution (Lambertian)
	float nDotL = glm::max(glm::dot(N, L), 0.0f);
	glm::vec3 diffuse = (kL * rhoL * nDotL) / glm::pi<float>();

	// Specular contribution (Blinn-Phong)
	float nDotH = glm::max(glm::dot(N, H), 0.0f);
	glm::vec3 specular = (kG * rhoG * glm::pow(nDotH, s) * (s + 8.0f)) / (8.0f * glm::pi<float>());

	// Combine results
	glm::vec3 brdf = diffuse + specular;

	// Return the average of RGB components as a scalar value
	return (brdf.r + brdf.g + brdf.b) / 3.0;
}

// Generate a BRDF plot and write to CSV
int plotting() {
	// Parameters
	glm::vec3 normal(0.0f, 0.0f, 1.0f);          // Normal facing the Z-axis
	glm::vec3 out_direction(0.0f, 0.0f, 1.0f);  // Viewer facing the -Z direction

	// Open a CSV file to save results
	std::ofstream outputFile("BP_brdf_plot_data.csv");
	if (!outputFile.is_open()) {
		std::cerr << "Error opening output file!" << std::endl;
		return -1;
	}

	outputFile << "angle,brdf_value\n"; // CSV header

	// Loop through angles
	for (int angle = -90; angle <= 90; ++angle) {
		// Convert angle to radians
		double radians = glm::radians(static_cast<double>(angle));

		// Compute the input direction (in the Z-plane)
		glm::vec3 in_direction(glm::sin(radians), 0.0f, glm::cos(radians));

		// Compute the BRDF value
		double brdf_value = blinn_phong_brdf(in_direction, out_direction, normal);

		// Write to CSV file
		outputFile << angle << "," << brdf_value << "\n";
	}

	outputFile.close();
	std::cout << "BRDF data written to BP_brdf_plot_data.csv" << std::endl;

	return 0;
}

int main(int argc, char** argv)
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

    glEnable(GL_DEPTH_TEST); // Enable depth-testing
    glDepthFunc(GL_LESS); // Depth-testing interprets a smaller value as "closer"
    plotting();

    std::string inputfile = "../../common/data/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    int flags = 0; // See load_flags_t enum for more information.

    // Load the object
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

    // Check for errors
    if (!err.empty()) { // `err` may contain a warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }
    else
    {
        std::cout << "Loaded " << inputfile
            << " ("
            << shapes.size() << " shapes"
            << ")"
            << "\n";
        if (shapes.size() > 0)
        {
            std::cout << shapes[0].mesh.positions.size() << " points "
                << "(" << shapes[0].mesh.positions.size() * sizeof(float) << " bytes), "
                << shapes[0].mesh.indices.size() << " indices "
                << "(" << shapes[0].mesh.indices.size() * sizeof(unsigned int) << " bytes), "
                << shapes[0].mesh.normals.size() << " normals.\n"
                ;
        }
    }

    // Populate your VBO with shapes[0].mesh.positions and shapes[0].mesh.normals
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Creation and filling of the VBO
    GLuint vbo_positions;
    glGenBuffers(1, &vbo_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER,
        shapes[0].mesh.positions.size() * sizeof(float), // Size in bytes
        shapes[0].mesh.positions.data(),                 // Raw data
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vbo_normals;
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER,
        shapes[0].mesh.normals.size() * sizeof(float), // Size in bytes
        shapes[0].mesh.normals.data(),                 // Raw data
        GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Populate your EBO with shapes[0].mesh.indices
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        shapes[0].mesh.indices.size() * sizeof(unsigned int), // Size in bytes
        shapes[0].mesh.indices.data(),                 // Raw data
        GL_STATIC_DRAW);

    // Set uniform variables
    GLint lightDirLoc = glGetUniformLocation(shader_program, "lightDir");
    GLint baseColorLoc = glGetUniformLocation(shader_program, "baseColor");
    GLint roughnessLoc = glGetUniformLocation(shader_program, "roughness");
    GLint metallicLoc = glGetUniformLocation(shader_program, "metallic");
    GLint shaderModelLoc = glGetUniformLocation(shader_program, "shaderModel");

    glm::vec3 lightDir = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 baseColor = glm::vec3(0.8f, 0.2f, 0.2f);
    float roughness = 0.5f;
    float metallic = 0.0f;
    int shaderModel = 2;

    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));
    glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor));
    glUniform1f(roughnessLoc, roughness);
    glUniform1f(metallicLoc, metallic);
    glUniform1i(shaderModelLoc, shaderModel);

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
        glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f));
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
        glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), x_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));

        //
        // Use glm::perspective to create a projection matrix
        const float fov = 0.7854f;           // 45 degrees in radians       fov and aspect are other options that are not in the course 
        const float aspect = 1.0f;
        const float near = 1.0f;             // Near and far are fundamental 
        const float far = 10.0f;
        glm::mat4 projectionMatrix = glm::perspective(0.7854, 1.0, 1.0, 10.0); //fov, aspect (width and height ratio of the window), near, far

        //
        // Use glm::translate and glm::rotate to create the
        // model and view matrices.
        modelMatrix = rotate_y * rotate_x;

        // Light parameters
        int light_count = 1;
        glm::vec3 lightPosition = glm::vec3(1.0f, 3.0f, 2.0f); // Light position 
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White color

        //
        // Multiply your matrices in the correct order to get a
        // modelViewProjection matrix and upload it to the appropriate
        // uniform variable in vertex shader.
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
        glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        //-----------------------------------------------------------------------//

        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo_positions);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    // Close GL context and any other GLFW resources
    glfwTerminate();
    exit(EXIT_SUCCESS);

    return 0;
}


