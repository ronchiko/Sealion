
#include <string.h>

#include "Sealion/matrix.h"
#include "Sealion/scene.h"

extern void _Seal_MatrixBlit(float *matrix, float *in, int ow, int oh, int rw, int rh);
extern void _Seal_MatrixIdentity(float *matrix, int nelem);
extern void _Seal_MatrixTranspose(float *matrix, float *in, int n);
extern void _Seal_DrawMatrix(float *matrix, int w, int h);

void Seal_M3Identity(Seal_Matrix3x3 matrix){
	_Seal_MatrixIdentity(matrix, 3);
}

void Seal_M3Rotation(Seal_Matrix3x3 matrix, float angle) {
	Seal_M3Identity(matrix);

	Seal_Matrix2x2 _2d_rotation;
	Seal_M2Rotation(_2d_rotation, angle);
	_Seal_MatrixBlit(matrix, _2d_rotation, 2, 2, 3, 3);
}

void Seal_M3Translate(Seal_Matrix3x3 matrix, float x, float y) {
	Seal_M3Identity(matrix);

	matrix[2] = x;
	matrix[5] = y;
}

void Seal_M3Scale(Seal_Matrix3x3 matrix, float x, float y) {
	Seal_M3Identity(matrix);

	matrix[0] = x;
	matrix[4] = y;
}

void Seal_M3Multiply(Seal_Matrix3x3 out, Seal_Matrix3x3 a, Seal_Matrix3x3 b) {
	float buffer[9];
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			buffer[i + 3 * j] = a[i] * b[3 * j] + a[i + 3 * 1] * b[1 + 3 * j] + a[i + 3 * 2] * b[3 * j + 2];

	memcpy(out, buffer, 9 * sizeof(float));
}

void Seal_M3Transform(Seal_Matrix3x3 matrix, Seal_Vec2 p, Seal_Vec2 s, float a) {
	Seal_M3Identity(matrix);

	Seal_Matrix3x3 scale;		Seal_M3Scale(scale, s.x, s.y);
	Seal_Matrix3x3 rotation;	Seal_M3Rotation(rotation, a);
	Seal_Matrix3x3 transform;	Seal_M3Translate(transform, p.x, p.y);

	Seal_M3Multiply(matrix, matrix, scale);
	Seal_M3Multiply(matrix, matrix, rotation);
	Seal_M3Multiply(matrix, matrix, transform);
}

Seal_Vec2 Seal_M3V2Mult(Seal_Matrix3x3 matrix, Seal_Vec2 v){

	float v3[3] = { 0 };
	for(int i = 0; i < 9; i += 3)
		v3[i / 3] = matrix[i] * v.x + matrix[i + 1] * v.y + matrix[i + 2];

	return (Seal_Vec2){ v3[0], v3[1] };
}

void Seal_M3Camera(Seal_Matrix3x3 matrix, Seal_Camera *camera, float whr) {

	Seal_Matrix2x2 screen; 		Seal_M2Screen(screen, camera->units, whr, camera->zoom);
	Seal_Matrix3x3 rotation; 	Seal_M3Rotation(rotation, camera->rotation);
	Seal_Matrix3x3 position; 	Seal_M3Translate(position, 
													-camera->position.x / camera->units, 
													-camera->position.y / camera->units);
	Seal_Matrix3x3 screen3x3;	_Seal_MatrixBlit(position, screen, 2, 2, 3, 3);

	Seal_M3Identity(matrix);
	Seal_M3Multiply(matrix, matrix, rotation);
	Seal_M3Multiply(matrix, matrix, position);
}

void Seal_M3Transpose(Seal_Matrix3x3 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 3);
}