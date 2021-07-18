CC = gcc
#WINDOWS_CC = i686-w64-mingw32-gcc
WINDOWS_CC = x86_64-w64-mingw32-gcc
LDFLAGS = -lGLU -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lz -lfreetype
LDFLAGS_WINDOWS = lib/cglm/libcglm.a lib/glfw/libglfw3.a lib/freetype/lib/libfreetype.a -L /usr/lib/x86_64-linux-gnu
CFLAGS = -I lib/cglm/include -I lib/glfw/include -I lib/freetype/include

FILES = src/main.c include/src/glad.c
OUT = run

all:
	$(CC) -o $(OUT) $(FILES) $(LDFLAGS) lib/freetype/lib/libfreetype.a -lglfw -lm $(CFLAGS) 
windows:
	$(WINDOWS_CC) -o $(OUT).exe $(FILES) $(LDFLAGS) -lglfw3 -lgdi32 -lmvec $(LDFLAGS_WINDOWS) $(CFLAGS) 
.PHONY: clean
clean:
	rm -r -f $(OUT)
