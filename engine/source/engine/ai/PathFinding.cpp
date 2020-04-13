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

#include "engine-precompiled-header.h"
#include "PathFinding.h"

// A Utility Function to check whether given cell (row, col) 
// is a valid cell or not. 

gswy::PathFinding::PathFinding(int x, int y)
	:
	ROW(x),
	COL(y)
{
	// Create a closed list and initialise it to false which means 
	// that no cell has been included yet 
	// This closed list is implemented as a boolean 2D array 
	closedList = (bool**)MemoryManager::Allocate(ROW * sizeof(bool*));
	for (int i = 0; i < ROW; ++i)
	{
		closedList[i] = (bool*)MemoryManager::Allocate(COL * sizeof(bool));
	}

	// Declare a 2D array of structure to hold the details 
	//of that cell 
	cellDetails = (cell**)MemoryManager::Allocate(ROW * sizeof(cell*));
	for (int i = 0; i < ROW; ++i)
	{
		cellDetails[i] = (cell*)MemoryManager::Allocate(COL * sizeof(cell));
	}
}

gswy::PathFinding::~PathFinding()
{
	for (int i = 0; i < ROW; ++i)
	{
		MemoryManager::Free(closedList[i], sizeof(bool) * COL);
		MemoryManager::Free(cellDetails[i], sizeof(cell) * COL);
	}
	MemoryManager::Free(closedList, sizeof(bool*) * ROW);
	MemoryManager::Free(cellDetails, sizeof(cell*) * ROW);
}

bool gswy::PathFinding::Search(const Grid_float & grid, const ivec2 & src, const ivec2 & dest)
{
	Pair _src = Pair(src.x, src.y);
	Pair _dest = Pair(dest.x, dest.y);
	return aStarSearch(grid, _src, _dest);
}

const std::vector<ivec2>& gswy::PathFinding::GetResult()
{
	return m_reuslt;
}

bool gswy::PathFinding::isValid(int row, int col)
{
	// Returns true if row number and column number 
	// is in range 
	return (row >= 0) && (row < ROW) &&
		(col >= 0) && (col < COL);
}

// A Utility Function to check whether the given cell is 
// blocked or not 

bool gswy::PathFinding::isUnBlocked(const Grid_float& grid, int row, int col)
{
	// Returns true if the cell is not blocked else false 
	if (grid[row][col] != 1.0f)
		return (true);
	else
		return (false);
}

// A Utility Function to check whether destination cell has 
// been reached or not 

bool gswy::PathFinding::isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics. 

double gswy::PathFinding::calculateHValue(int row, int col, Pair dest)
{
	
	double dx = row - dest.first;
	double dy = col - dest.second;
	switch (H_option)
	{
	case 0:
		// L1
		return D * (dx + dy);
	case 1:
		// L2
		return D * sqrt(dx * dx + dy * dy);
	case 2:
		// L-infinity
		return D * (dx + dy) + (D2 - 2 * D) * MIN(dx, dy);
	}
}

// A Utility Function to trace the path from the source 
// to destination 

void gswy::PathFinding::tracePath(cell** cellDetails, Pair dest)
{
	// DEBUG_PRINT("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col))
	{
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	m_reuslt.clear();
	// Assign path to result
	while (!Path.empty())
	{
		pair<int, int> p = Path.top();
		m_reuslt.push_back(ivec2(p.first, p.second));
		Path.pop();
		// DEBUG_PRINT("-> (%d,%d) ", p.first, p.second);
	}

	return;
}

// A Function to find the shortest path between 
// a given source cell to a destination cell according 
// to A* Search Algorithm 

bool gswy::PathFinding::aStarSearch(const Grid_float& grid, Pair src, Pair dest)
{
	// If the source is out of range 
	if (isValid(src.first, src.second) == false)
	{
		DEBUG_PRINT("Source is invalid\n");
		return false;
	}

	// If the destination is out of range 
	if (isValid(dest.first, dest.second) == false)
	{
		DEBUG_PRINT("Destination is invalid\n");
		return false;
	}

	// Either the source or the destination is blocked 
	if (isUnBlocked(grid, src.first, src.second) == false ||
		isUnBlocked(grid, dest.first, dest.second) == false)
	{
		DEBUG_PRINT("Source or the destination is blocked\n");
		return false;
	}

	// If the destination cell is the same as source cell 
	if (isDestination(src.first, src.second, dest) == true)
	{
		DEBUG_PRINT("We are already at the destination\n");
		return false;
	}

	int i, j;

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			closedList[i][j] = false;
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node 
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implenented as a set of pair of pair.*/
	set<pPair> openList;

	// Put the starting cell on the open list and set its 
	// 'f' as 0 
	openList.insert(make_pair(0.0, make_pair(i, j)));

	// We set this boolean value as false as initially 
	// the destination is not reached. 
	bool foundDest = false;

	while (!openList.empty())
	{
		pPair p = *openList.begin();

		// Remove this vertex from the open list 
		openList.erase(openList.begin());

		// Add this vertex to the closed list 
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		/*
		Generating all the 8 successor of this cell

		N.W   N   N.E
		\   |   /
		\  |  /
		W----Cell----E
		/ | \
		/   |  \
		S.W    S   S.E

		Cell-->Popped Cell (i, j)
		N -->  North       (i-1, j)
		S -->  South       (i+1, j)
		E -->  East        (i, j+1)
		W -->  West           (i, j-1)
		N.E--> North-East  (i-1, j+1)
		N.W--> North-West  (i-1, j-1)
		S.E--> South-East  (i+1, j+1)
		S.W--> South-West  (i+1, j-1)*/

		// To store the 'g', 'h' and 'f' of the 8 successors 
		double gNew, hNew, fNew;

		//----------- 1st Successor (North) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j] == false &&
				isUnBlocked(grid, i - 1, j) == true)
			{
				gNew = cellDetails[i][j].g + D;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i - 1][j].f == FLT_MAX ||
					cellDetails[i - 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i - 1, j)));

					// Update the details of this cell 
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		//----------- 2nd Successor (South) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j] == false &&
				isUnBlocked(grid, i + 1, j) == true)
			{
				gNew = cellDetails[i][j].g + D;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i + 1][j].f == FLT_MAX ||
					cellDetails[i + 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j)));
					// Update the details of this cell 
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- 3rd Successor (East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j + 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j + 1] == false &&
				isUnBlocked(grid, i, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + D;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j + 1].f == FLT_MAX ||
					cellDetails[i][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i, j + 1)));

					// Update the details of this cell 
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//----------- 4th Successor (West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j - 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j - 1] == false &&
				isUnBlocked(grid, i, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + D;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j - 1].f == FLT_MAX ||
					cellDetails[i][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i, j - 1)));

					// Update the details of this cell 
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

		//----------- 5th Successor (North-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j + 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j + 1] == false &&
				isUnBlocked(grid, i - 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j + 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i - 1][j + 1].f == FLT_MAX ||
					cellDetails[i - 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i - 1, j + 1)));

					// Update the details of this cell 
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 6th Successor (North-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j - 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j - 1] == false &&
				isUnBlocked(grid, i - 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j - 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i - 1][j - 1].f == FLT_MAX ||
					cellDetails[i - 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
					// Update the details of this cell 
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//----------- 7th Successor (South-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j + 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j + 1] == false &&
				isUnBlocked(grid, i + 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j + 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i + 1][j + 1].f == FLT_MAX ||
					cellDetails[i + 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i + 1, j + 1)));

					// Update the details of this cell 
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 8th Successor (South-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j - 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				DEBUG_PRINT("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				break;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j - 1] == false &&
				isUnBlocked(grid, i + 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j - 1, dest);
				fNew = gNew + hNew;

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//                OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i + 1][j - 1].f == FLT_MAX ||
					cellDetails[i + 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i + 1, j - 1)));

					// Update the details of this cell 
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	// When the destination cell is not found and the open 
	// list is empty, then we conclude that we failed to 
	// reach the destiantion cell. This may happen when the 
	// there is no way to destination cell (due to blockages) 
	if (foundDest == false)
		DEBUG_PRINT("Failed to find the Destination Cell\n");

	return foundDest;
}
