#version 330 core

in VS_OUT {
  vec4 color;
} fs_in;

out vec4 color;

void main() {
  float x = (gl_FragCoord.x) / 800.0;
  float y = (gl_FragCoord.y) / 600.0;
  color = vec4(x, y, 0, 1.0);  
}

