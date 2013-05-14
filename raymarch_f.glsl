#version 330 core

/*
Links
	http://www.pouet.net/topic.php?which=8177&page=1
*/


in VS_OUT {
  vec4 color;
  float time;
} fs_in;

out vec4 color;

float dfSphere(vec3 p, float s) {
	return length(p) - s;
}

float dfRoundBox(vec3 p, vec3 b, float r) {
	return length(max(abs(p) - b, 0.0)) - r;
}

float dfTorus(vec3 p, vec3 t) {
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;
}

float dist(vec3 rayPos, vec3 objPos) {
  vec3 rep = vec3(100, 100, 100);

//  float dSphere = dfSphere(mod(rayPos - objPos + vec3(0,0,2), rep) - 0.5 * rep, 8);
  float dBox1 = dfRoundBox(mod(rayPos - objPos, rep) - 0.5 * rep, vec3(8), 0.1);
  float dBox2 = dfRoundBox(mod(rayPos - objPos + vec3(10, 10, 10), rep) - 0.5 * rep, vec3(8), 0.1);

  return min(dBox1, dBox2) + 0.1;
}

void main() {
  float x = (gl_FragCoord.x) / 400.0 - 1.0;
  float y = (gl_FragCoord.y) / 300.0 - 1.0;

  vec3 v0 = vec3(x, y, 0); // ray intersection with view plane
  vec3 p = vec3(0, 0, -2); // camera position
  vec3 vd = normalize(v0 - p); // ray direction

  vec3 spherePos = vec3(
              120 * cos(fs_in.time * 0.3), 
  						120 * sin(fs_in.time * 0.5), 
  						fs_in.time * -100);

  vec3 lightPos = vec3(50 * cos(fs_in.time), 200, 5 * sin(fs_in.time));

  float sphereR = 5;

  bool hit = false;
  float maxZ = 200.0;

  float v;
  vec3 n;
  float steps;

  float brightness = 0.0;


  p += vd;

  while (!hit && p.z < maxZ) {
  	float tw = 4.0;
    float d = dist(p, spherePos);

  	if (d <= 0.2) {
  		hit = true;
  	} else {
	  	p += vd * max(0.1, 0.9 * (d - sphereR));
  	}
  	steps++;
  }

  if (hit) {
  	float r = brightness;
  	steps *= 0.01;

    float e = 0.001;
    vec3 dx = vec3(e, 0, 0);
    vec3 dy = vec3(0, e, 0);
    vec3 dz = vec3(0, 0, e);
    vec3 n = normalize(vec3(
        dist(p + dx, spherePos) - dist(p - dx, spherePos),
        dist(p + dy, spherePos) - dist(p - dy, spherePos),
        dist(p + dz, spherePos) - dist(p - dz, spherePos)
    ));

    vec3 ld = normalize(lightPos - p);      
    float specular = pow(clamp(dot(vd, reflect(ld, n)), 0, 1), 8) * 50 / length(lightPos - p);
    brightness = specular + clamp(dot(vd, reflect(ld, n)), 0, 1);

    float fog = pow(p.z / maxZ, 2);
    vec3 fogcol = vec3(0.2, 0.2, 0.2);

    float i = brightness;
  	color = vec4(mix(vec3(1, i, i), fogcol, fog), 1);

  } else {
  	color = vec4(0.2,0.2,0.2,1);
  }

}
