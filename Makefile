
run:
	optirun ./shaderquad


compile:
	g++ -o shaderquad shaderquad.cpp glutil.cpp -lglfw -lGL -lGLEW -lpthread




