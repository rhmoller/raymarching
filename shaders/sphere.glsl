#version 330 core

in VS_OUT {
  float time;
} fs_in;

out vec4 color;

float dfSphere(vec3 p, float s) {
  return length(p) - s;
}

float dist(vec3 rayPos, vec3 objPos) {
  vec3 p = rayPos - objPos;
  return dfSphere(p, 16);
}

float doLight(vec3 vd, vec3 p, vec3 n, vec3 lightPos) {
  vec3 ld = normalize(lightPos - p);      
  float specular = pow(clamp(dot(vd, reflect(ld, n)), 0, 1), 16) * 30 / length(lightPos - p);
  return specular + 0.25 * clamp(dot(ld, n), 0, 1);
}

void main() {
  float x = (gl_FragCoord.x) / 400.0 - 1.0;
  float y = (gl_FragCoord.y) / 300.0 - 1.0;
  float t = fs_in.time;

  vec3 v0 = vec3(x, y, 0); // ray intersection with view plane
  vec3 p = vec3(0, 0, -2); // camera position
  vec3 vd = normalize(v0 - p); // ray direction

  vec3 spherePos = vec3(0, 0, 50);
  vec3 lightPos = vec3(70 * cos(fs_in.time * 1.7), 70 * sin(fs_in.time * 0.35), 70 * sin(fs_in.time));

  // epsilon vectors used for calculating surface normals
  float e = 0.001;    
  vec3 dx = vec3(e, 0, 0);
  vec3 dy = vec3(0, e, 0);
  vec3 dz = vec3(0, 0, e);


  // ray marching loop
  bool hit = false;
  float maxZ = 300.0;

  p += vd;
  while (!hit && p.z < maxZ) {
    float d = dist(p, spherePos);

    if (d <= 0.05) {
      hit = true;
    } else {
      p += vd * d;
    }
  }

  // shading 
  if (hit) {

    vec3 n = normalize(vec3(
			    dist(p + dx, spherePos) - dist(p - dx, spherePos),
			    dist(p + dy, spherePos) - dist(p - dy, spherePos),
			    dist(p + dz, spherePos) - dist(p - dz, spherePos)
			    ));

    float light = doLight(vd, p, n, lightPos);

    float fog = clamp(pow(1.1 * p.z / maxZ, 2), 0, 1);
    vec3 fogcol = vec3(0.2, 0.2, 0.2);

    color = vec4(mix(vec3(light), fogcol, fog), 1);

  } else {
    color = vec4(0.2, 0.2, 0.2, 1);
  }

}
