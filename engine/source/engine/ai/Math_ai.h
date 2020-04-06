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
    LU decomposition for squared matrix of size NxN
    */
    Array_float SolveUsingLU(const Grid_float& matrix, const Array_float& b, int n);
}