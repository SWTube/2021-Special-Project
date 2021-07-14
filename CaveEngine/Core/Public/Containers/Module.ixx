/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Containers/Array.h"
export module Matrix;

namespace cave
{
	export class Matrix4x4Float
	{
	public:
		Matrix4x4Float()
			: mMatrix{ 0.f }
		{

		}

		Matrix4x4Float(float x, float y, float z, float w)
			: mMatrix{ 0.f }
		{
			mMatrix[0][3] = x;
			mMatrix[1][3] = y;
			mMatrix[2][3] = z;
			mMatrix[3][3] = w;
		}

		static Matrix4x4Float GetIdentity()
		{
			Matrix4x4Float matrix;

			for (int i = 0; i < 4; ++i)
			{
				matrix.mMatrix[i][i] = 1;
			}

			return matrix;
		}

		void TranslationX(float distance)
		{
			Translation(distance, 0, 0);
		}

		void TranslationY(float distance)
		{
			Translation(0, distance, 0);
		}

		void TranslationZ(float distance)
		{
			Translation(0, 0, distance);
		}

		void Translation(float xDistance, float yDistance, float zDistance)
		{

		}

		void ScaleX(float multiple)
		{
			Scale(multiple, 1, 1);
		}

		void ScaleY(float multiple)
		{
			Scale(1, multiple, 1);
		}

		void ScaleZ(float multiple)
		{
			Scale(1, 1, multiple);
		}

		void Scale(float xMultiple, float yMultiple, float zMultiple)
		{

		}

		void RotationX(float degree)
		{
			Rotation(degree, 0, 0);
		}

		void RotationY(float degree)
		{
			Rotation(0, degree, 0);
		}

		void RotationZ(float degree)
		{
			Rotation(0, 0, degree);
		}

		void Rotation(float xDegree, float yDegree, float zDegree)
		{

		}

	private:
		Matrix4x4Float GetTranslationMatrix(float xDistance, float yDistance, float zDistance)
		{
			Matrix4x4Float translationMatrix(xDistance, yDistance, zDistance, 1);

			return translationMatrix;
		}

		Matrix4x4Float GetScaleMatrix(float xMultiple, float yMultiple, float zMultiple)
		{
			Matrix4x4Float scaleMatrix;

			scaleMatrix.mMatrix[0][0] = xMultiple;
			scaleMatrix.mMatrix[1][1] = yMultiple;
			scaleMatrix.mMatrix[2][2] = zMultiple;
			scaleMatrix.mMatrix[3][3] = 1;

			return scaleMatrix;
		}

		Matrix4x4Float GetRotaitonMatrix(float xDegree, float yDegree, float zDegree)
		{
			Matrix4x4Float rotationMatrix;



			return rotationMatrix;
		}

		friend Matrix4x4Float operator*(const Matrix4x4Float& leftMatrix, const Matrix4x4Float& rightMatrix);

	private:
		float mMatrix[4][4];
	};

	friend Matrix4x4Float operator*(const Matrix4x4Float& leftMatrix, const Matrix4x4Float& rightMatrix)
	{
		Matrix4x4Float productMatrix;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					productMatrix[i][j] += leftMatrix[j][k] * rightMatrix[k][j];
				}
			}
		}

		return productMatrix;
	}
}