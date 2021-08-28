
CC = gcc
SHADER_EMBEDDER = scripts/compile_shader.py

SRCS = $(wildcard src/*.c) $(wildcard src/math/*.c)
EMBBEDED_FILES = objs/resources/embedded.o
OBJS = $(patsubst src/%.c, objs/%.o, $(SRCS)) $(EMBBEDED_FILES)

SHADERS = $(wildcard shaders/*.glsl)
COMPILED_SHADERS = $(patsubst %.glsl, objs/precompiled/%.glsl.c, $(SHADERS))

GAME = dodger.exe

OPENGL_LIB = -lopengl32 -lglu32
GLEW_LIB = -L C:\Dev\GLEW\glew-2.1.0\lib\Release\x64 -lglew32 -lglew32s
SDL_LIB = -L C:\Dev\SDL2\lib\x64 -lSDL2main -lSDL2

FLAGS = -D_WIN32 -I include -I C:/Dev/include $(OPENGL_LIB) $(SDL_LIB) $(GLEW_LIB)

all: $(GAME)

$(GAME): build-shaders $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(GAME)

objs/%.o : src/%.c
	$(CC) -c $< $(FLAGS) -o $@ -ggdb

build-shaders: $(COMPILED_SHADERS) $(EMBBEDED_FILES)

$(EMBBEDED_FILES): resources/embedded.c
	$(CC) -c $< $(FLAGS) -I . -o $@ -ggdb

objs/precompiled/%.glsl.c: %.glsl
	rm -f $(EMBBEDED_FILES) $@
	python $(SHADER_EMBEDDER) $< $@

# Cleaning methods

clean-shaders:
	rm -f objs/precompiled/shaders/*.c objs/resources/*.o

clean-objs:
	rm -f objs/*.o $(GAME) 

clean: clean-shaders clean-objs
	