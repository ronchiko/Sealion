
#include "Sealion/vector2.h"
#include "Sealion/maths.h"

extern float Seal_FastInverseSqrt(float n);

void Seal_V2add(Seal_Vec2 *out, const Seal_Vec2 a, const Seal_Vec2 b){
	out->x = a.x + b.x;
	out->y = a.y + b.y;
}
void Seal_V2extend(Seal_Vec2 *out, const Seal_Vec2 b) {
	out->x += b.x;
	out->y += b.y;
}

void Seal_V2scale(Seal_Vec2 *out, const Seal_Vec2 a, float scale) {
	out->x = a.x * scale;
	out->y = a.y * scale;
}

void Seal_V2sub(Seal_Vec2 *out, const Seal_Vec2 a, const Seal_Vec2 b){
	out->x = a.x - b.x;
	out->y - a.y - b.y;
}

float Seal_V2sqaureMagnitude(const Seal_Vec2 a) {
	return a.x * a.x + a.y * a.y;
}

void Seal_V2normalize(Seal_Vec2 *a) {
	float oneOvermagnitude = Seal_FastInverseSqrt(Seal_V2sqaureMagnitude(*a));
	a->x *= oneOvermagnitude;
	a->y *= oneOvermagnitude; 
}

Seal_Vec2 Seal_V2normal(const Seal_Vec2 a) {
	float oneOvermagnitude = Seal_FastInverseSqrt(Seal_V2sqaureMagnitude(a));
	return (Seal_Vec2){a.x * oneOvermagnitude, a.y * oneOvermagnitude }; 
}

void Seal_V2invert(Seal_Vec2 *out) {
	out->x *= -1.f;
	out->y *= -1.f;
}
Seal_Vec2 Seal_V2inverse(const Seal_Vec2 in) {
	return (Seal_Vec2){ -in.x, -in.y };
}

Seal_Vec2 Seal_V2swap(const Seal_Vec2 out) {
	return (Seal_Vec2){out.y, out.x};
}