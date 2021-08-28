#pragma once

#include <stdio.h>

inline float Seal_FastInverseSqrt(float n) {
	
	const float x2 = n * 0.5f;
	const float threehalves = 1.5f;

	union {
		float f;
		unsigned int i;
	} conversion = { n };

	conversion.i = 0x5f3759df - ( conversion.i >> 1 );
	conversion.f *= (threehalves - x2 * conversion.f * conversion.f);

	return conversion.f;
}