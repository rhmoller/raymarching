#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out VS_OUT {
  vec4 color;
} vs_out;

void main() {
  gl_Position.xyz = pos;
  gl_Position.w = 1.0;
  
  vs_out.color = color;
}

