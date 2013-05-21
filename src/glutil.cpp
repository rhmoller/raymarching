#include "glutil.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

std::string loadFile(const char* fileName) {
  std::string content;
  std::ifstream in(fileName, std::ios::in);
 
  if (in.is_open()) {
    std::string line = "";
    while (getline(in, line)) {
      content += line + "\n";
    }
    in.close();
  }
  
  return content;  
}

GLuint compileShader(GLuint id, const std::string &src) {
  GLint res = GL_FALSE;
  int infoLogLength;

  char const* srcPtr = src.c_str();
  glShaderSource(id, 1, &srcPtr, NULL);
  glCompileShader(id);
  
  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<GLchar> vshErrMsg(infoLogLength + 1);
    glGetShaderInfoLog(id, infoLogLength, NULL, &vshErrMsg[0]);
    printf("%s\n", &vshErrMsg[0]);
  }
  
  return res;
}

GLuint loadShaders(const char* vshFileName, const char* fshFileName) {
  GLint ok = GL_TRUE;

  GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
  std::string vshSrc = loadFile(vshFileName);  
  printf("compiling: %s\n", vshFileName);
  ok &= compileShader(vsh, vshSrc);

  GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);  
  std::string fshSrc = loadFile(fshFileName);
  printf("compiling: %s\n", fshFileName);
  ok &= compileShader(fsh, fshSrc);
  
  GLuint p = 0;

  if (ok) {
    p = glCreateProgram();
    if (vsh != 0) glAttachShader(p, vsh);
    if (fsh != 0) glAttachShader(p, fsh);
    glLinkProgram(p);
  
    GLint res = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(p, GL_LINK_STATUS, &res);
    glGetShaderiv(p, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
      std::vector<char> errMsg(infoLogLength + 1);
      glGetProgramInfoLog(vsh, infoLogLength, NULL, &errMsg[0]);
      printf("%s\n", &errMsg[0]);
    }
  }

  if (vsh != 0) glDeleteShader(vsh);
  if (fsh != 0) glDeleteShader(fsh);
  
  return p;
}


