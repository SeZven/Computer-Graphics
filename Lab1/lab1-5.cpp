#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"


// Read the code for the shader programs from file
std::string vertex_shader_str =	readFile("../lab1-5_vs.glsl");
std::string fragment_shader_str =	readFile("../lab1-5_fs.glsl");
GLuint shader_program;

// You can store the rotation angles here, for example
float g_rotation[2] = {0,0}; // g_rotation[0] = x_rotation, g_rotation[1] = y_rotation 


// Multiply two 4x4 matrices A * B = C
void MUL_4x4 (GLfloat (*C)[4], const GLfloat (*A)[4], const GLfloat (*B)[4])
{
	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Compute C = A * B
	//4
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			C[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	//-------------------------------------------------------------------------//
}



// Helper function to forward shader compilation errors to terminal.
void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
	if(isCompiled == GL_FALSE)
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
void loadShaders( std::string vertex_shader_str,
                  std::string fragment_shader_str )
{
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader_src, NULL);
	glCompileShader (vs);
	checkShaderCompileError(vs);
  
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader_src, NULL);
	glCompileShader (fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram ();
	glAttachShader (shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram (shader_program);
}

// Write out error in terminal in case init failed.
static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

// GLFW callback function to handle mouse and keyboard input.
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		loadShaders( vertex_shader_str, fragment_shader_str );
	}
	
	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update rotation angle here, for example
	//1
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
	//-------------------------------------------------------------------------//

	//  if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
	//  if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )     
	//  if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
	//  if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ) 
	
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
}

// GLFW callback function to handle window resizing.
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



// Main comes here!
int main(int argc, char const *argv[])
{
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);

	// NEW: variables for window width and height
	int w_width = 800;
	int w_height = 600;
	GLFWwindow* window = glfwCreateWindow (w_width, w_height,
	                                       "Hello Cube", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);

	glewExperimental = GL_TRUE;
	glewInit ();

	// Load shader program from file (see function above).
	loadShaders( vertex_shader_str, fragment_shader_str );
	
	glClearColor(0.1, 0.2, 0.3, 0.0);

	// NEW: tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"


	//-------------------------------------------------------------------------//
	// COPY FROM lab1-4
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

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	GLuint ebo; //3
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------//
   

	while (!glfwWindowShouldClose (window)) 
	{
		// Update other events like keyboard/mouse input.
		glfwPollEvents ();
		// Clear the drawing surface before drawing something new.
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// NEW: You might need this for correcting the aspect ratio
		glfwGetFramebufferSize(window, &w_width , &w_height ); 

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Define the projection matrix, rotation matrices, model matrix, etc.

		GLfloat rotate_y[4][4] = {
		{cos(g_rotation[1]),	0, sin(g_rotation[1]),	0},
		{0,						1, 0,					0},
		{-sin(g_rotation[1]),	0, cos(g_rotation[1]),	0},
		{0,						0, 0,					1}
		};; //2
		GLfloat rotate_x[4][4] = {
		{1,	0,					0,						0},
		{0, cos(g_rotation[0]),	-sin(g_rotation[0]),	0},
		{0, sin(g_rotation[0]),	cos(g_rotation[0]),		0},
		{0, 0,					0,						1}
		};;
    
		GLfloat modelMatrix[4][4]; //3
		MUL_4x4(modelMatrix, rotate_x, rotate_y);//5
    
		GLfloat viewMatrix[4][4] = { //6
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 2},
		{0, 0, 0, 1}
		};

		GLfloat inverseViewMatrix[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, -2},
		{0, 0, 0, 1}
		};
    
		GLfloat modelViewMatrix[4][4];
		MUL_4x4(modelViewMatrix, inverseViewMatrix, modelMatrix);


		GLfloat projectionMatrix[4][4] = {
		{1, 0, 0,			0},
		{0, 1, 0,			0},
		{0, 0, -101 / 99,	-200 / 99}, // 100+1/100-1, 2*100*1/100-1
		{0, 0, -1,			0}
		};//7
		
		GLfloat modelViewProjectionMatrix[4][4];
		MUL_4x4(modelViewProjectionMatrix, projectionMatrix, modelViewMatrix);
		//-----------------------------------------------------------------------//
    
		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Use glUniformMatrix4fv to send your matrix or matrices to the shader
		//8
		GLint mvp_location = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
		glUniformMatrix4fv(mvp_location, 1, GL_TRUE, &modelViewProjectionMatrix[0][0]);
		//-----------------------------------------------------------------------//
    
		//-----------------------------------------------------------------------//
		// Call glDrawElements as before
		glBindVertexArray(vao); //4
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//
		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
		
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

