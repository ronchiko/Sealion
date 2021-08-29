
## MAKEFILE FOR CYGWIN ##

CC = gcc
SHADER_EMBEDDER = scripts/compile_shader.py

GAME_SOURCE = $(wildcard game/*.c)
GAME_OBJECT = $(patsubst game/%.c, objs/game/%.o, $(GAME_SOURCE))

SRCS = $(wildcard src/*.c) $(wildcard src/math/*.c)
EMBBEDED_FILES = objs/resources/embedded.o
OBJS = $(patsubst src/%.c, objs/%.o, $(SRCS)) $(EMBBEDED_FILES)

SHADERS = $(wildcard shaders/*.glsl)
COMPILED_SHADERS = $(patsubst %.glsl, objs/precompiled/%.glsl.c, $(SHADERS))

LIB = Sealion
GAME = dodger.exe

# Change this to where those libararies are localed
OPENGL_LIB = -lopengl32 -lglu32
GLEW_LIB = -L C:\Dev\GLEW\glew-2.1.0\lib\Release\x64 -lglew32 -lglew32s
SDL_LIB = -L C:\Dev\SDL2\lib\x64 -lSDL2main -lSDL2

ALL_LIBS = $(OPENGL_LIB) $(SDL_LIB) $(GLEW_LIB)
FLAGS = -D_WIN32 -I include -I C:/Dev/include $(ALL_LIBS)

all: lib game

lib: $(LIB)
game: $(GAME)

# Build DLL
$(LIB): build-shaders $(OBJS)
	mkdir objs/.dllbuild

	$(CC) -shared -D_WIN32 -I include -I C:/Dev/include -o $(LIB).dll \
		-Wl,--out-implib,lib$(LIB).lib \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $(OBJS) \
		-Wl,--no-whole-archive $(ALL_LIBS)
	
	rm -r objs/.dllbuild

generate-implib:
	echo EXPORTS > objs/.dllbuild/$(LIB).def
	nm.exe $(LIB).dll | grep.exe ' T Seal_' | sed.exe 's/.* T //' >> objs/.dllbuild/$(LIB).def
	dlltool --def objs/.dllbuild/$(LIB).def --dllname $(LIB).dll --output-lib $(LIB).lib

# Build Demo
$(GAME): build-shaders $(GAME_OBJECT)
	$(CC) $(GAME_OBJECT) $(FLAGS) -L./ -l$(LIB) -o $(GAME)

# Generic build rules

objs/game/%.o: game/%.c
	$(CC) -c $< $(FLAGS) -o $@ -ggdb

objs/%.o : src/%.c
	$(CC) -c $< $(FLAGS) -o $@ -ggdb -fpic

build-shaders: $(COMPILED_SHADERS) $(EMBBEDED_FILES)

$(EMBBEDED_FILES): resources/embedded.c
	$(CC) -c $< $(FLAGS) -I . -o $@ -ggdb -fpic

objs/precompiled/%.glsl.c: %.glsl
	rm -f $(EMBBEDED_FILES) $@
	python $(SHADER_EMBEDDER) $< $@

# Cleaning rules

clean-shaders:
	rm -f objs/precompiled/shaders/*.c objs/resources/*.o

clean-objs:
	rm -f objs/*.o $(GAME) Sealion.* libSealion.dll.a

clean: clean-shaders clean-objs
	