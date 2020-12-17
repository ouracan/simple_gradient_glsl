#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION

#endif

#include <stdio.h>
#include <stdlib.h>
//GLFW and GLEW libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"



int main(int argc, char **argv)
{
    //Global variables
    GLFWwindow* window;
  //Initialize GLFW
  if(!glfwInit()){
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit(EXIT_FAILURE);
  }

    //enable anti-aliasing 4x with GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);
    /* specify the client API version that the created context must be compatible with. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //make the GLFW forward compatible
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //use the OpenGL Core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(640, 480, "Simple Gradient - GLSL", nullptr, nullptr);
    if(!window){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Final to Initialize GLEW\n");
      glfwTerminate();
      exit(EXIT_FAILURE);
    }


    GLuint program = LoadShaders("simple.vert", "simple.frag");
    glBindFragDataLocation(program, 0, "color_out");
    glUseProgram(program);

    // Create Vertex Array Object
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    // Create a Vertex Buffer Object and copy the vertex data to it

    GLuint vertex_buffer;
    GLuint color_buffer;
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &color_buffer);
    
    const GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f, 0.0f
    };
    const GLfloat colors[]={
      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


    GLint position_attrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLint color_attrib = glGetAttribLocation(program, "color_in");
    glEnableVertexAttribArray(color_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


    while(!glfwWindowShouldClose(window)){
      // Clear the screen to black
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      // Draw a rectangle from the 2 triangles using 6 vertices
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glfwSwapBuffers(window);
      glfwPollEvents();
    }


    //clean up the memories
    glDisableVertexAttribArray(position_attrib);
    glDisableVertexAttribArray(color_attrib);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(program);
    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  }
