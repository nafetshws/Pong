CC = gcc
WINDOWS_CC = x86_64-w64-mingw32-gcc
LDFLAGS = -lGLU -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lz 
LDFLAGS_WINDOWS = lib/cglm/libcglm.a lib/glfw/libglfw3.a -L /usr/lib/x86_64-linux-gnu
CFLAGS = -I lib/cglm/include -I lib/glfw/include -I lib/freetype/include
LINKS = -L lib/cglm -lcglm -L lib/glfw/ -lglfw3 -L lib/freetype/lib -lfreetype -L /usr/lib/x86_64-linux-gnu 


FILES = src/main.c include/src/glad.c
OUT = run

all:
	$(CC) -o $(OUT) $(FILES) $(LDFLAGS) -lglfw -lm -lfreetype $(CFLAGS) 
windows:
	$(WINDOWS_CC) -o $(out).exe -fPIC $(FILES) $(CFLAGS) $(LINKS) $(LDFLAGS) -lgdi32 -lmvec 
.PHONY: clean
clean:
	rm -r -f $(OUT)
