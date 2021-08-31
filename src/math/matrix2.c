
#include <math.h>

#include "Sealion/matrix.h"

extern void _Seal_MatrixBlit(float *matrix, float *in, int ow, int oh, int rw, int rh);
extern void _Seal_MatrixIdentity(float *matrix, int nelem);
extern void _Seal_MatrixTranspose(float *matrix, float *in, int n);


void Seal_M2Identity(Seal_Matrix2x2 matrix) {
	_Seal_MatrixIdentity(matrix, 2);
}

void Seal_M2Screen(Seal_Matrix2x2 matrix, int ww, float ratio, float zoom) {
	matrix[1] = matrix[2] = 0;
	matrix[3] = ratio * (matrix[0] = zoom / ww);
}

void Seal_M2Rotation(Seal_Matrix2x2 matrix, float angle) {
	matrix[2] = -(matrix[1] = sinf(angle));
	matrix[0] = matrix[3] = cosf(angle);
}

void Seal_M2Transpose(Seal_Matrix2x2 matrix) {
	_Seal_MatrixTranspose(matrix, matrix, 2);
}