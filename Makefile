all:
	gcc -o run src/main.c include/src/glad.c -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lm
clean:
	rm run