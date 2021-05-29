all:
	gcc -o run src/main.c -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
clean:
	rm run

