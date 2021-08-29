
#define DLL_EXPORT __declspec(dllexport)

DLL_EXPORT const char DEFAULT_VERTEX_SHADER_SOURCE[] = {
	#include "objs/precompiled/shaders/vertex.glsl.c"
};

DLL_EXPORT const char DEFAULT_FRAGMENT_SHADER_SOURCE[] = {
	#include "objs/precompiled/shaders/fragment.glsl.c"
};