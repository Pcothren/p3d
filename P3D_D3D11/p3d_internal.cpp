#include "p3d_internal.h"
#include <assert.h>
#include <cmath>

namespace p3d_internal {

	#define TO_RADIANS(x) (x * M_PI / 180.0f)
	#define TO_DEGREES(x) (x * 180.0f / M_PI)

	void multiply16(float* matrix, float* multiplier) {

		static float ans[16]{ 0.0f };

		ans[4 * 0 + 0] = multiplier[4 * 0 + 0] * matrix[4 * 0 + 0] + multiplier[4 * 0 + 1] * matrix[4 * 1 + 0] + multiplier[4 * 0 + 2] * matrix[4 * 2 + 0] + multiplier[4 * 0 + 3] * matrix[4 * 3 + 0];
		ans[4 * 1 + 0] = multiplier[4 * 1 + 0] * matrix[4 * 0 + 0] + multiplier[4 * 1 + 1] * matrix[4 * 1 + 0] + multiplier[4 * 1 + 2] * matrix[4 * 2 + 0] + multiplier[4 * 1 + 3] * matrix[4 * 3 + 0];
		ans[4 * 2 + 0] = multiplier[4 * 2 + 0] * matrix[4 * 0 + 0] + multiplier[4 * 2 + 1] * matrix[4 * 1 + 0] + multiplier[4 * 2 + 2] * matrix[4 * 2 + 0] + multiplier[4 * 2 + 3] * matrix[4 * 3 + 0];
		ans[4 * 3 + 0] = multiplier[4 * 3 + 0] * matrix[4 * 0 + 0] + multiplier[4 * 3 + 1] * matrix[4 * 1 + 0] + multiplier[4 * 3 + 2] * matrix[4 * 2 + 0] + multiplier[4 * 3 + 3] * matrix[4 * 3 + 0];

		ans[4 * 0 + 1] = multiplier[4 * 0 + 0] * matrix[4 * 0 + 1] + multiplier[4 * 0 + 1] * matrix[4 * 1 + 1] + multiplier[4 * 0 + 2] * matrix[4 * 2 + 1] + multiplier[4 * 0 + 3] * matrix[4 * 3 + 1];
		ans[4 * 1 + 1] = multiplier[4 * 1 + 0] * matrix[4 * 0 + 1] + multiplier[4 * 1 + 1] * matrix[4 * 1 + 1] + multiplier[4 * 1 + 2] * matrix[4 * 2 + 1] + multiplier[4 * 1 + 3] * matrix[4 * 3 + 1];
		ans[4 * 2 + 1] = multiplier[4 * 2 + 0] * matrix[4 * 0 + 1] + multiplier[4 * 2 + 1] * matrix[4 * 1 + 1] + multiplier[4 * 2 + 2] * matrix[4 * 2 + 1] + multiplier[4 * 2 + 3] * matrix[4 * 3 + 1];
		ans[4 * 3 + 1] = multiplier[4 * 3 + 0] * matrix[4 * 0 + 1] + multiplier[4 * 3 + 1] * matrix[4 * 1 + 1] + multiplier[4 * 3 + 2] * matrix[4 * 2 + 1] + multiplier[4 * 3 + 3] * matrix[4 * 3 + 1];

		ans[4 * 0 + 2] = multiplier[4 * 0 + 0] * matrix[4 * 0 + 2] + multiplier[4 * 0 + 1] * matrix[4 * 1 + 2] + multiplier[4 * 0 + 2] * matrix[4 * 2 + 2] + multiplier[4 * 0 + 3] * matrix[4 * 3 + 2];
		ans[4 * 1 + 2] = multiplier[4 * 1 + 0] * matrix[4 * 0 + 2] + multiplier[4 * 1 + 1] * matrix[4 * 1 + 2] + multiplier[4 * 1 + 2] * matrix[4 * 2 + 2] + multiplier[4 * 1 + 3] * matrix[4 * 3 + 2];
		ans[4 * 2 + 2] = multiplier[4 * 2 + 0] * matrix[4 * 0 + 2] + multiplier[4 * 2 + 1] * matrix[4 * 1 + 2] + multiplier[4 * 2 + 2] * matrix[4 * 2 + 2] + multiplier[4 * 2 + 3] * matrix[4 * 3 + 2];
		ans[4 * 3 + 2] = multiplier[4 * 3 + 0] * matrix[4 * 0 + 2] + multiplier[4 * 3 + 1] * matrix[4 * 1 + 2] + multiplier[4 * 3 + 2] * matrix[4 * 2 + 2] + multiplier[4 * 3 + 3] * matrix[4 * 3 + 2];

		ans[4 * 0 + 3] = multiplier[4 * 0 + 0] * matrix[4 * 0 + 3] + multiplier[4 * 0 + 1] * matrix[4 * 1 + 3] + multiplier[4 * 0 + 2] * matrix[4 * 2 + 3] + multiplier[4 * 0 + 3] * matrix[4 * 3 + 3];
		ans[4 * 1 + 3] = multiplier[4 * 1 + 0] * matrix[4 * 0 + 3] + multiplier[4 * 1 + 1] * matrix[4 * 1 + 3] + multiplier[4 * 1 + 2] * matrix[4 * 2 + 3] + multiplier[4 * 1 + 3] * matrix[4 * 3 + 3];
		ans[4 * 2 + 3] = multiplier[4 * 2 + 0] * matrix[4 * 0 + 3] + multiplier[4 * 2 + 1] * matrix[4 * 1 + 3] + multiplier[4 * 2 + 2] * matrix[4 * 2 + 3] + multiplier[4 * 2 + 3] * matrix[4 * 3 + 3];
		ans[4 * 3 + 3] = multiplier[4 * 3 + 0] * matrix[4 * 0 + 3] + multiplier[4 * 3 + 1] * matrix[4 * 1 + 3] + multiplier[4 * 3 + 2] * matrix[4 * 2 + 3] + multiplier[4 * 3 + 3] * matrix[4 * 3 + 3];

		matrix[0] = ans[0];
		matrix[1] = ans[1];
		matrix[2] = ans[2];
		matrix[3] = ans[3];
		matrix[4] = ans[4];
		matrix[5] = ans[5];
		matrix[6] = ans[6];
		matrix[7] = ans[7];
		matrix[8] = ans[8];
		matrix[9] = ans[9];
		matrix[10] = ans[10];
		matrix[11] = ans[11];
		matrix[12] = ans[12];
		matrix[13] = ans[13];
		matrix[14] = ans[14];
		matrix[15] = ans[15];
	}

	void multiply4(float* matrix, float* multiplier) {

		static float ans[4]{0.0f};

		ans[0] = multiplier[4 * 0 + 0] * matrix[0] + multiplier[4 * 0 + 1] * matrix[1] + multiplier[4 * 0 + 2] * matrix[2] + multiplier[4 * 0 + 3] * matrix[3];
		ans[1] = multiplier[4 * 1 + 0] * matrix[0] + multiplier[4 * 1 + 1] * matrix[1] + multiplier[4 * 1 + 2] * matrix[2] + multiplier[4 * 1 + 3] * matrix[3];
		ans[2] = multiplier[4 * 2 + 0] * matrix[0] + multiplier[4 * 2 + 1] * matrix[1] + multiplier[4 * 2 + 2] * matrix[2] + multiplier[4 * 2 + 3] * matrix[3];
		ans[3] = multiplier[4 * 3 + 0] * matrix[0] + multiplier[4 * 3 + 1] * matrix[1] + multiplier[4 * 3 + 2] * matrix[2] + multiplier[4 * 3 + 3] * matrix[3];

		matrix[0] = ans[0];
		matrix[1] = ans[1];
		matrix[2] = ans[2];
		matrix[3] = ans[3];
	}


	void scale(float* pt, float* modifier) {

		// prepare scaling modifier matrix data
		sMat[4 * 0 + 0] = modifier[0];
		sMat[4 * 1 + 1] = modifier[1];
		sMat[4 * 2 + 2] = modifier[2];
		sMat[4 * 3 + 3] = modifier[3];

		multiply4(pt, sMat);

	}

	void translate(float* pt, float* modifier) {

		// prepare traslate modifier matrix data
		tMat[4 * 0 + 3] = modifier[0];
		tMat[4 * 1 + 3] = modifier[1];
		tMat[4 * 2 + 3] = modifier[2];
		tMat[4 * 3 + 3] = modifier[3];

		multiply4(pt, tMat);
	}

	void rotateX(float* pt, float angle) {

		// prepare rotate x modifier matrix data
		rxMat[4 * 1 + 2] = cosf(TO_RADIANS(angle));
		rxMat[4 * 1 + 3] = -sinf(TO_RADIANS(angle));
		rxMat[4 * 2 + 2] = sinf(TO_RADIANS(angle));
		rxMat[4 * 2 + 3] = cosf(TO_RADIANS(angle));

		multiply4(pt, rxMat);
	}

	void rotateY(float* pt, float angle) {

		// prepare rotate y modifier matrix data
		ryMat[4 * 0 + 0] = cosf(TO_RADIANS(angle));
		ryMat[4 * 0 + 2] = sinf(TO_RADIANS(angle));
		ryMat[4 * 2 + 0] = -sinf(TO_RADIANS(angle));
		ryMat[4 * 2 + 2] = cosf(TO_RADIANS(angle));

		multiply4(pt, ryMat);
	}

	void rotateZ(float* pt, float angle) {

		// prepare rotate z modifier matrix data
		rzMat[4 * 0 + 0] = cosf(TO_RADIANS(angle));
		rzMat[4 * 0 + 1] = -sinf(TO_RADIANS(angle));
		rzMat[4 * 1 + 0] = sinf(TO_RADIANS(angle));
		rzMat[4 * 1 + 1] = cosf(TO_RADIANS(angle));

		multiply4(pt, rzMat);
	}

	void modifyCompositionMatrix(float* matrix, float* scaleModifier, float* angle, float* translateModifier )
	{
		if (scaleModifier != nullptr)
		{
			// prepare scaling modifier matrix data
			sMat[4 * 0 + 0] = scaleModifier[0];
			sMat[4 * 1 + 1] = scaleModifier[1];
			sMat[4 * 2 + 2] = scaleModifier[2];
			sMat[4 * 3 + 3] = scaleModifier[3];
			multiply16(matrix, sMat);
		}

		if (angle != nullptr)
		{
			// prepare rotate x modifier matrix data
			rxMat[4 * 1 + 1] = cosf(TO_RADIANS(angle[0]));
			rxMat[4 * 1 + 2] = -sinf(TO_RADIANS(angle[0]));
			rxMat[4 * 2 + 1] = sinf(TO_RADIANS(angle[0]));
			rxMat[4 * 2 + 2] = cosf(TO_RADIANS(angle[0]));
			// prepare rotate y modifier matrix data
			ryMat[4 * 0 + 0] = cosf(TO_RADIANS(angle[1]));
			ryMat[4 * 0 + 2] = sinf(TO_RADIANS(angle[1]));
			ryMat[4 * 2 + 0] = -sinf(TO_RADIANS(angle[1]));
			ryMat[4 * 2 + 2] = cosf(TO_RADIANS(angle[1]));
			// prepare rotate z modifier matrix data
			rzMat[4 * 0 + 0] = cosf(TO_RADIANS(angle[2]));
			rzMat[4 * 0 + 1] = -sinf(TO_RADIANS(angle[2]));
			rzMat[4 * 1 + 0] = sinf(TO_RADIANS(angle[2]));
			rzMat[4 * 1 + 1] = cosf(TO_RADIANS(angle[2]));

			multiply16(matrix, rxMat);
			multiply16(matrix, ryMat);
			multiply16(matrix, rzMat);
		}

		if (translateModifier != nullptr)
		{
			// prepare traslate modifier matrix data
			tMat[4 * 0 + 3] = translateModifier[0];
			tMat[4 * 1 + 3] = translateModifier[1];
			tMat[4 * 2 + 3] = translateModifier[2];
			tMat[4 * 3 + 3] = translateModifier[3];
			multiply16(matrix, tMat);
		}
	}
}