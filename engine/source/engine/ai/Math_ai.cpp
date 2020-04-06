/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/05/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Math_ai.h"

/*
LU decomposition for squared matrix of size NxN
*/
Array_float gswy::SolveUsingLU(const Grid_float& matrix, const Array_float& b, int n)
{
	// decomposition of matrix
	Grid_float lu(n, n);
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			sum = 0;
			for (int k = 0; k < i; k++)
				sum += lu[i][k] * lu[k][j];
			lu[i][j] = matrix[i][j] - sum;
		}
		for (int j = i + 1; j < n; j++)
		{
			sum = 0;
			for (int k = 0; k < i; k++)
				sum += lu[j][k] * lu[k][i];
			lu[j][i] = (1.0f / lu[i][i]) * (matrix[j][i] - sum);
		}
	}

	// lu = L+U-I
	// find solution of Ly = b
	Array_float y(n);
	for (int i = 0; i < n; i++)
	{
		sum = 0;
		for (int k = 0; k < i; k++)
			sum += lu[i][k] * y[k];
		y[i] = b[i] - sum;
	}
	// find solution of Ux = y
	Array_float x(n);
	for (int i = n - 1; i >= 0; i--)
	{
		sum = 0;
		for (int k = i + 1; k < n; k++)
			sum += lu[i][k] * x[k];
		x[i] = (1.0f / lu[i][i]) * (y[i] - sum);
	}
	return x;
}
