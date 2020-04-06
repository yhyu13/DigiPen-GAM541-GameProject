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

#pragma once
#include "Grid_float.h"

namespace gswy
{
    /*
    Get LU decomposition for a squared matrix of size NxN
    */
    Grid_float GetLU(const Grid_float& matrix, int n);
    /*
    Using NxN LU decomposed matrix to solve linear equation (matrix passed in is already a LU matrix)
    */
    Array_float SolveLU(const Grid_float& lu, const Array_float& b, int n);
    /*
    solve linear equation using LU decomposition for a squared matrix of size NxN
    */
    Array_float SolveUsingLU(const Grid_float& matrix, const Array_float& b, int n);



    /*
    Get the coefficients of a paramatric cubic spline interporlated curve
    */
    std::vector<vec2> GetCubicSplineCoeff(const std::vector<vec2>& pts);
    /*
    Get point on the paramatric cubic spline 
    */
    vec2 CubicSplineInterpolate(float t, const std::vector<vec2>& coeff);
    /*
    Get the cubic spline interporlated curve that is represented by points.
    Return an empty vector is the input vector has size smaller than 2.
     (argument alpha represent the step size for the paramerteric value t which is used internally, 
     the smaller the smoother the curve, default is 0.05)
    */
    std::vector<vec2> GetCubicSpline(const std::vector<vec2>& pts, float alpha = 0.05f);
}