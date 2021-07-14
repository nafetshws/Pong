CC = gcc
WINDOWS_CC = i686-w64-mingw32-gcc
LDFLAGS = -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lm

FILES = src/main.c include/src/glad.c
OUT = run

all:
	$(CC) -o $(OUT) $(FILES) $(LDFLAGS)
windows:
	$(WINDOWS_CC) -o $(OUT).exe $(FILES) $(LDFLAGS) -L /usr/local/lib -llibcglm
.PHONY: clean
clean:
	rm -r -f $(OUT)
