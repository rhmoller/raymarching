#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "glutil.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

GLuint vao;
GLuint vbo;

GLuint p;

GLint uTime;
float t;

static const GLfloat vao_data[] = {
  -1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
  1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,

  1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
};

void setup(const char* shaderFileName) {
  p = loadShaders("raymarch_v.glsl", shaderFileName, NULL);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vao_data), vao_data, GL_STATIC_DRAW);

  glUseProgram(p);
  uTime = glGetUniformLocation(p, "time");
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(p);
    glUniform1f(uTime, t);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int main(int argc, char** argv) {
  int running = GL_TRUE;
  
  if (!glfwInit()) {
    fprintf(stderr, "failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);

  if (!glfwOpenWindow(WIN_WIDTH, WIN_HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW)) {
    fprintf(stderr, "Failed to open GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {    
    return -1;
  }
  
  glfwSetWindowTitle("Hello, OpenGL");

  setup(argv[1]);
  if (p == 0) running = GL_FALSE;

  while (running) {    
    t += 0.01f;
    render();
    
    glfwSwapBuffers();    
    running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
  }
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
