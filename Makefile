CC = gcc
#WINDOWS_CC = i686-w64-mingw32-gcc
WINDOWS_CC = x86_64-w64-mingw32-gcc
LDFLAGS = -lGLU -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl 
LDFLAGS_WINDOWS = lib/cglm/libcglm.a lib/glfw/libglfw3.a -L /usr/lib/x86_64-linux-gnu
CFLAGS = -I lib/cglm/include -I lib/glfw/include

FILES = src/main.c include/src/glad.c
OUT = run

all:
	$(CC) -o $(OUT) $(FILES) $(LDFLAGS) -lglfw -lm
windows:
	$(WINDOWS_CC) -o $(OUT).exe $(FILES) $(LDFLAGS) -lglfw3 -lgdi32 -lmvec $(LDFLAGS_WINDOWS) $(CFLAGS) 
.PHONY: clean
clean:
	rm -r -f $(OUT)
