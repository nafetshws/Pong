CC = gcc
WINDOWS_CC = x86_64-w64-mingw32-gcc
LD_FLAGS_MAC = -L lib/glfw/mac -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -L lib/freetype/mac -lfreetype -lbz2 -ldl -L /opt/homebrew/lib -lpng -lharfbuzz -lbrotlidec -lz
LDFLAGS_WINDOWS = -L lib/glfw/windows -lglfw3 -L lib/cglm -lcglm -L lib/freetype/windows -lfreetype -lgdi32
CFLAGS = -I lib/cglm/include -I lib/glfw/include -I lib/freetype/include

FILES = src/main.c src/glad.c
OUT = build/pong
BUILD_DIR = build

mac: build_directory
	$(CC) -o $(OUT) $(FILES) $(LD_FLAGS_MAC) $(CFLAGS)
windows: build_directory_win
	$(CC) -o $(OUT).exe $(FILES) $(LDFLAGS_WINDOWS) $(CFLAGS)
	@copy /Y "lib\freetype\windows\freetype.dll" "build\freetype.dll"

.PHONY: build_directory
build_directory:
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

.PHONY: build_directory_win
build_directory_win:
	@if not exist "build" mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	rm $(OUT)