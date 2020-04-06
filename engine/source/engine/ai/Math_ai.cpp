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

Grid_float gswy::GetLU(const Grid_float& matrix, int n)
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
	return lu;
}

Array_float gswy::SolveLU(const Grid_float& lu, const Array_float& b, int n)
{
	// lu = L+U-I
	// find solution of Ly = b
	float sum = 0;
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


/*
Get the coefficients of cubic spline interporlated curve
*/
std::vector<vec2> gswy::GetCubicSplineCoeff(const std::vector<vec2>& pts)
{
	std::vector<vec2> result;

	// n must be greater or equal to 2
	if (pts.size() < 2)
	{
		return result;
	}

	int n = pts.size();
	int degree = n - 1;
	int K = degree;
	int N = n + 2;

	Grid_float A(N, N);
	Array_float b_x(N);
	Array_float b_y(N);

	// Initialize the matrix for cubic spline
	for (int row = 0; row < n; ++row)
	{
		// Assign b values
		b_x[row] = pts[row].x;
		b_y[row] = pts[row].y;

		// Since knot sequences is t \in [0,K], so simply assign t = row in iteration here.
		float t = row;
		// Assign coefficients of the first four (standard polynomial degree 3) terms with f(t=row)
		A[row][0] = 1;
		for (int col = 1; col < 4; ++col)
		{
			A[row][col] = (float)glm::pow(t, col);
		}

		// Assign cubic spline terms start with 1,2...,K-1(=N-4)
		for (int col = 4; col < N; ++col)
		{
			float b = col - 3;
			float delta = t - b;
			if (delta >= 0)
			{
				A[row][col] = (float)glm::pow(delta, 3);
			}
		}
	}

	// Initialize the matrix for cubic spline differential terms
	{
		// 1st f''(0) = 0
		A[n][2] = 1;
	}
	{
		// 2nd f''(k=n-1) = 0
		A[n + 1][3] = 6 * K;
		for (int col = 4; col < N; ++col)
		{
			float b = col - 3;
			float delta = K - b;
			if (delta >= 0)
			{
				A[n + 1][col] = 6 * (delta);
			}
		}
	}

	// Linear solver : LU decom
	auto LU = GetLU(A, N);
	auto result_x = SolveLU(LU, b_x, N);
	auto result_y = SolveLU(LU, b_y, N);

	// Assign results
	for (int i = 0; i < N; ++i)
	{
		result.push_back(vec2(result_x[i], result_y[i]));
	}
	return result;
}

glm::vec2 gswy::CubicSplineInterpolate(float t, const std::vector<vec2>& coeff)
{
	int N = coeff.size();
	vec2 result;

	result.x += coeff[0].x;
	result.y += coeff[0].y;

	result += coeff[0];
	// Assign coefficients of the first four (standard polynomial degree 3) terms
	for (int i = 1; i < 4; ++i)
	{
		result += coeff[i] * (float)glm::pow(t, i);
	}
	// Assign cubic spline terms start with 1,2...,N-4
	for (int i = 4; i < N; ++i)
	{
		float b = i - 3;
		float delta = t - b;
		if (delta >= 0)
		{
			result += coeff[i] * (float)glm::pow(delta, 3);
		}
	}
	return result;
}

/*
Get the cubic spline interporlated curve that is represented by points.
Return an empty vector is the input vector has size smaller than 2.
(argument alpha represent the step size for the paramerteric value t which is used internally,
the smaller the smoother the curve, default is 0.05)
*/
std::vector<vec2> gswy::GetCubicSpline(const std::vector<vec2>& pts, float alpha)
{
	int K = pts.size() - 1;
	std::vector<vec2> result;
	std::vector<vec2> coeff = GetCubicSplineCoeff(pts);
	if (!coeff.empty())
	{
		for (float t = 0; t <= K; t += alpha)
		{
			result.push_back(CubicSplineInterpolate(t, coeff));
		}
	}
	return result;
}