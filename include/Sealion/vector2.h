#pragma once

#include <stdbool.h>

typedef struct  {
	int x;
	int y;
} Seal_Int2;

typedef struct {
	float x;
	float y;
} Seal_Vec2;

void Seal_V2add(Seal_Vec2 *out, const Seal_Vec2 a, const Seal_Vec2 b);
void Seal_V2extend(Seal_Vec2 *a, const Seal_Vec2 b);

void Seal_V2sub(Seal_Vec2 *out, const Seal_Vec2 a, const Seal_Vec2 b);

float Seal_V2sqaureMagnitude(const Seal_Vec2 a);

void Seal_V2scale(Seal_Vec2 *out, const Seal_Vec2 in, float scale);
#define Seal_V2div(out, in, scale) Seal_V2scale((out), (in), 1.f / (scale))

Seal_Vec2 Seal_V2inverse(const Seal_Vec2 in);
void v2invert(Seal_Vec2 *v2);

void Seal_V2normalize(Seal_Vec2 *out);
Seal_Vec2 Seal_V2normal(const Seal_Vec2 in);

Seal_Vec2 Seal_V2swap(const Seal_Vec2 out);
void Seal_V2prependicular(Seal_Vec2 *out, const Seal_Vec2 to);

typedef struct {
	Seal_Vec2 position, size;
} Seal_Rect;

bool Seal_RectContains(const Seal_Rect *rect, Seal_Vec2 point);
bool Seal_RectCollides(const Seal_Rect *a, const Seal_Rect *b);