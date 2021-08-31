
#include <math.h>

#include "Sealion/maths.h"
#include "Sealion/graphics.h"
#include "Sealion/matrix.h"

extern int Seal_Min(int a, int b);
extern int Seal_Max(int a, int b);

void _Seal_MatrixIdentity(float *matrix, int rows) {
	int g = 0;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < rows; j++)
			matrix[i * rows + j] = i == j;
	}
}

void _Seal_DrawMatrix(float *matrix, int w, int h) {
	for(int i = 0; i < w; i++) {
		printf("[");
		for(int j = 0; j < h; j++)
			printf("%f ", matrix[j + i * w]);
		printf("]\n");
	}
	printf("\n");
}

void _Seal_MatrixBlit(float *matrix, float *in_matrix, int ow, int oh, int w, int h) {
	for(int i = 0; i < Seal_Min(ow, w); i++)
		for(int j = 0; j < Seal_Min(oh, h); j++)
			matrix[i + j * h] = in_matrix[i + j * oh];
}

void _Seal_MatrixTranspose(float *matrix, float *in, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < i; j++) {
			float temp = in[j + i * n];
			matrix[j + i * n] = in[i + j * n];
			matrix[i + j * n] = temp;
		}
	}
}

void Seal_M4Identity(Seal_Matrix4x4 matrix) {
	_Seal_MatrixIdentity(matrix, 4);
}

void Seal_M4Ortho(Seal_Matrix4x4 matrix, const Seal_Rect *screen, const Seal_Vec2 *nearfar) {
	Seal_M4Identity(matrix);

	float rl = screen->size.x - screen->position.x;
	float tb = screen->size.y - screen->position.y;
	float fn = nearfar->x - nearfar->y;

	matrix[0]  = 2 / rl;
	matrix[5]  = 2 / tb;
	matrix[10] = 2 / fn;

	matrix[12] = -(screen->size.x + screen->position.x) / rl;   
	matrix[13] = -(screen->size.y + screen->position.y) / tb;   
	matrix[14] = -(nearfar->y + nearfar->x) / fn;

	matrix[15] = 1.f;
}

void Seal_M4Transpose(Seal_Matrix4x4 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 4);
}