
#pragma once

#include "vector2.h" 

typedef float Seal_Matrix4x4[16];
typedef float Seal_Matrix2x2[4];

void Seal_M4identity(Seal_Matrix4x4 matrix);
void Seal_M4ortho(Seal_Matrix4x4 matrix, const Seal_Rect *projection_rect, const Seal_Vec2 *farnear);