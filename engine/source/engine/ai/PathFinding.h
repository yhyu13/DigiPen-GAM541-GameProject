/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/04/2020
- End Header ----------------------------*/

#pragma once
#include <vector>
#include <stack>
#include <set>
#include "Grid_float.h"
#include "Math_ai.h"

using namespace std;

namespace gswy
{

	// Reference https://www.geeksforgeeks.org/a-search-algorithm/

	// Creating a shortcut for int, int pair type 
	typedef pair<int, int> Pair;

	// Creating a shortcut for pair<int, pair<int, int>> type 
	typedef pair<double, pair<int, int>> pPair;

	// A structure to hold the neccesary parameters 
	struct cell
	{
		// Row and Column index of its parent 
		// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
		int parent_i, parent_j;
		// f = g + h 
		double f, g, h;
	};

	class PathFinding
	{
	public:
		PathFinding() = delete;
		PathFinding(const PathFinding&) = delete;
		PathFinding& operator=(const PathFinding&) = delete;
		explicit PathFinding(int x, int y);
		~PathFinding();

		bool Search(const Grid_float& grid, const ivec2& src, const ivec2& dest);
		const std::vector<ivec2>& GetResult();

	private:
		std::vector<ivec2> m_reuslt;
		int ROW;
		int COL;

		bool** closedList;
		cell** cellDetails;
	
	private:
		// A Utility Function to check whether given cell (row, col) 
		// is a valid cell or not. 
		bool isValid(int row, int col);

		// A Utility Function to check whether the given cell is 
		// blocked or not 
		bool isUnBlocked(const Grid_float& grid, int row, int col);

		// A Utility Function to check whether destination cell has 
		// been reached or not 
		bool isDestination(int row, int col, Pair dest);

		// A Utility Function to calculate the 'h' heuristics. 
		double calculateHValue(int row, int col, Pair dest);

		// A Utility Function to trace the path from the source 
		// to destination 
		void tracePath(cell** cellDetails, Pair dest);

		// A Function to find the shortest path between 
		// a given source cell to a destination cell according 
		// to A* Search Algorithm 
		bool aStarSearch(const Grid_float& grid, Pair src, Pair dest);
	};
}

