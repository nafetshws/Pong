all:
	gcc -o run src/exercise.c  src/glad.c -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
clean:
	rm run

