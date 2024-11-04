
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"


// Read the code for the shader programs from file
std::string vertex_shader_str =	readFile("../lab1-2_vs.glsl");
std::string fragment_shader_str =	readFile("../lab1-2_fs.glsl");
GLuint shader_program;


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
}

// GLFW callback function to handle window resizing.
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}



// Main comes here!
int main(int argc, char const *argv[])
{
	// First, start GL context and a window using the GLFW helper library.
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	GLFWwindow* window = glfwCreateWindow (640, 480,
	                                       "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);

	// OK, now we have a window we can draw in.  
	// Start GLEW, so that we can access all OpenGL functions.
	glewExperimental = GL_TRUE;
	glewInit ();

	// Load shader program from file (see function above).
	loadShaders( vertex_shader_str, fragment_shader_str );
	
  glClearColor(0.1, 0.2, 0.3, 0.0);
  
  //-------------------------------------------------------------------------//
  // COPY YOUR SOLUTION FROM lab1-1.cpp HERE.
  // Create geometry and VBO and VAO.
  //-------------------------------------------------------------------------//
  // YOUR CODE GOES HERE.
  // 1. Create geometry
  float points[3][3] = { {-0.5,-0.5,0.0},{0.5,-0.5,0.0},{0.5,0.5,0.0} }; //QUESTION: How many elements? 9
  // 2-3. Create VAO and VBO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo); //GL_ARRAY_BUFFER, because vertex attributes (position in this case)
  // 4-6. Copy vertex data to VBO, configure the VAO
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);//4

  glEnableVertexAttribArray(0);//5

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//6
  // attribute nr 0
  // 3 elements per attribute (x, y, z)
  // the data type for each element
  // normalise integer values to be <1 or not?
  // stride = nr of bytes between start of each attribute
  // array offset


  //-------------------------------------------------------------------------//
  //-------------------------------------------------------------------------//

  // Loop until the window is closed.
  while (!glfwWindowShouldClose (window)) 
  {
	  // Update other events like keyboard/mouse input.
	  glfwPollEvents ();
	  // Clear the drawing surface before drawing something new.
	  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //-----------------------------------------------------------------------//
    // COPY YOUR SOLUTION FROM lab1-1.cpp HERE.
    // Bind the VAO and issue an appropriate glDraw*() command.
	  //-----------------------------------------------------------------------//
	  // YOUR CODE GOES HERE.
	  // 7. Bind the VAO and issue an appropriate glDraw*() command.
	  glBindVertexArray(vao);
	  glDrawArrays(GL_TRIANGLES, 0, 3); //set of triangles, starting at index 0, using 3 vertices
	  //-----------------------------------------------------------------------//
    //-----------------------------------------------------------------------//

	  //Refresh the displayed image.
    glfwSwapBuffers (window);
  }

  // Close GL context and any other GLFW resources.
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
