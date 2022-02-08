#pragma once

#include <string>
#include <vector>

namespace p3d_internal {



	struct p3d_point {

		// raw point data
		float data[4]{ 0.0f };

		// coordinate axis views into point's data for easier front facing identificaiton
		float* px = &data[0];
		float* py = &data[1];
		float* pz = &data[2];
		float* pw = &data[3];

		p3d_point(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;
		};
	};

	struct p3d_series {

		std::vector<float> xData{};
		std::vector<float> yData{};

		// setting up series transform data
		float scale[4]		{ 1, 1, 1, 1 };
		float angle			{ 0 };
		float translation[4]{ 0, 0, 0, 1 };

		// series matrix
		float mat[16]{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	};

	// prepare identity matrix used for scale
	static float sMat[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// prepare identity matrix used for translation
	static float tMat[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// prepare identity matrix used for x axis rotation
	static float rxMat[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// prepare identity matrix used for y axis rotation
	static float ryMat[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// prepare identity matrix used for z axis rotation
	static float rzMat[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	void multiply16(float* m1, float* m2);
	void multiply4(float* m1, float* m2);
	void scale(float* pt, float* modifier);
	void translate(float* pt, float* modifier);
	void rotateX(float* pt, float angle);
	void rotateY(float* pt, float angle);
	void rotateZ(float* pt, float angle);
	void createCompositionMatrix(float* matrix, float* scaleModifier, float* angle, float* translateModifier);
}