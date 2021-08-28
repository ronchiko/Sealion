
#include "Sealion/graphics.h"
#include "Sealion/matrix.h"

void Seal_M4identity(Seal_Matrix4x4 matrix) {
	int g = 0;
	for(int i = 0; i < 16; i++) {
		matrix[i] = (i + g) % 4;
		if(matrix[i]) g++;
	}
}

void Seal_M4ortho(Seal_Matrix4x4 matrix, const Seal_Rect *screen, const Seal_Vec2 *nearfar) {
	Seal_M4identity(matrix);

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