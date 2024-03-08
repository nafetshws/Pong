CC = gcc
WINDOWS_CC = x86_64-w64-mingw32-gcc
LD_FLAGS_MAC = -L lib/glfw -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -L lib/freetype -lfreetype -lbz2 -ldl -L /opt/homebrew/lib -lpng -lharfbuzz -lbrotlidec -lz
LDFLAGS_WINDOWS = lib/cglm/libcglm.a lib/glfw/libglfw3.a lib/freetype/lib/libfreetype.a -L /usr/lib/x86_64-linux-gnu
CFLAGS = -I lib/cglm/include -I lib/glfw/include -I lib/freetype/include

FILES = src/main.c src/glad.c
OUT = build/pong 
BUILD_DIR = ./build

mac: build_directory
	$(CC) -o $(OUT) $(FILES) $(LD_FLAGS_MAC) $(CFLAGS) 
windows: build_directory
	$(WINDOWS_CC) -o $(OUT).exe $(FILES) $(LDFLAGS) -lglfw3 -lgdi32 -lmvec $(LDFLAGS_WINDOWS) $(CFLAGS)

.PHONY: build_directory
build_directory:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm $(OUT)
