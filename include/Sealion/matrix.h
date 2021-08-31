
#pragma once

#include "vector2.h" 

struct _Seal_Camera;

typedef float Seal_Matrix4x4[16];
typedef float Seal_Matrix3x3[9];
typedef float Seal_Matrix2x2[4];

void Seal_M2Identity	(Seal_Matrix2x2 matrix);
void Seal_M2Rotation	(Seal_Matrix2x2 matrix, float angle);
void Seal_M2Screen		(Seal_Matrix2x2 matrix, int ww, float ratio, float zoom);
void Seal_M2Transpose	(Seal_Matrix3x3 matrix);

void Seal_M3Identity	(Seal_Matrix3x3 matrix);
void Seal_M3Rotation	(Seal_Matrix3x3 matrix, float angle);
void Seal_M3Translate	(Seal_Matrix3x3 matrix, float x, float y);
void Seal_M3Scale		(Seal_Matrix3x3 matrix, float x, float y);
void Seal_M3Camera		(Seal_Matrix3x3 matrix, struct _Seal_Camera *camera, float w2hr);
void Seal_M3Transpose	(Seal_Matrix3x3 matrix);

Seal_Vec2 Seal_M3V2Mult	(Seal_Matrix3x3 matrix, Seal_Vec2 v2);
void Seal_M3Multiply	(Seal_Matrix3x3 out, Seal_Matrix3x3 a, Seal_Matrix3x3 b);
void Seal_M3Transform	(Seal_Matrix3x3 out, Seal_Vec2 position, Seal_Vec2 scale, float angle);

void Seal_M4Identity	(Seal_Matrix4x4 matrix);
void Seal_M4Ortho		(Seal_Matrix4x4 matrix, const Seal_Rect *projection_rect, const Seal_Vec2 *farnear);
void Seal_M4Transpose	(Seal_Matrix3x3 matrix);
