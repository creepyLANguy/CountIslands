/* 
Based on : https://leetcode.com/problems/number-of-islands/description/ 

The Problem :
Given a 2d grid map of '1's (land) and '0's (water), count the number of islands. 
An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. 
You may assume all four edges of the grid are all surrounded by water. 

> Example A

Input: 
11110
11010
11000
00000

Output: 
1

> Example B

Input: 
11000
11000
00100
00011

Output: 
3 

*/

////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

enum TOPO_FEATURE
{
  WATER = 0,
  LAND  = 1,
};

//constStrings
namespace 
{
  const char* NL          = "\n";
  const char* print_Water = "_";
  const char* print_Land  = "x";
}

////////////////////////////////////////////////////////////////////////////////

void PrintGrid(vector<vector<TOPO_FEATURE>>& grid, const bool isPadded = false)
{
  const int padding = isPadded ? 1 : 0;

  for (int y = padding; y < grid.size() - padding; ++y)
  {
    for (int x = padding; x < grid[y].size() - padding; ++x)
    {
      if (grid[y][x] == WATER)
      {
        cout << print_Water;
      }
      else
      {
        cout << print_Land;
      }

    }
    cout << NL;
  }

  cout << NL;
}

////////////////////////////////////////////////////////////////////////////////

void RecursivelyConvertSelfAndNeighbours(
  vector<vector<TOPO_FEATURE>>& grid, 
  const int x, const int y, 
  const TOPO_FEATURE topo_feature, 
  const bool showRemovalSteps = true
)
{
  if (grid[y][x] == topo_feature)
  {
    return;
  }

  if (showRemovalSteps)
  {
    PrintGrid(grid, true);
  }

  grid[y][x] = topo_feature;

  RecursivelyConvertSelfAndNeighbours(grid, x-1, y, topo_feature, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x+1, y, topo_feature, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x, y-1, topo_feature, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x, y+1, topo_feature, showRemovalSteps);
}

int CountIslands(
  vector<vector<TOPO_FEATURE>>& grid, 
  const bool showIntermediateCount = true, 
  const bool showRemovalSteps = true
)
{
  int count = 0;

  for (int y = 1; y < grid.size() - 1; ++y)
  {
    for (int x = 1; x < grid[y].size() - 1; ++x)
    {
      if (grid[y][x] != LAND)
      {
        continue;
      }

      ++count;

      if (showIntermediateCount)
      {
        cout << NL << count << NL;
      }

      RecursivelyConvertSelfAndNeighbours(grid, x, y, WATER, showRemovalSteps);

      PrintGrid(grid, true);
    }
  }

  return count;
}

////////////////////////////////////////////////////////////////////////////////

vector<TOPO_FEATURE> GetHorizontalPad(vector<vector<TOPO_FEATURE>>& grid)
{
  vector<TOPO_FEATURE> horizontalPad;
  for (const int position : grid[0])
  {
    horizontalPad.push_back(WATER);
  }
  return horizontalPad;
}

////////////////////////////////////////////////////////////////////////////////

vector<vector<TOPO_FEATURE>> GetPaddedGrid(vector<vector<TOPO_FEATURE>>& grid)
{
  const vector<TOPO_FEATURE> horizontalPad = GetHorizontalPad(grid);

  vector<vector<TOPO_FEATURE>> paddedGrid;

  paddedGrid.push_back(horizontalPad);

  for (vector<TOPO_FEATURE> currentRow : grid)
  {
    vector<TOPO_FEATURE> paddedRow;

    paddedRow.push_back(WATER);

    for (TOPO_FEATURE topo_feature : currentRow)
    {
      paddedRow.push_back(topo_feature);
    }

    paddedRow.push_back(WATER);

    paddedGrid.push_back(paddedRow);

    paddedRow.push_back(WATER);
  }

  paddedGrid.push_back(horizontalPad);

  return paddedGrid;
}

////////////////////////////////////////////////////////////////////////////////

//test data
namespace
{
  const TOPO_FEATURE W = WATER;
  const TOPO_FEATURE L = LAND;

  //Hmm, I can see 14 islands here? 
  const vector<TOPO_FEATURE> r1 = { L,L,W,W,L,L,W,L,W,L,W,L,W };
  const vector<TOPO_FEATURE> r2 = { L,L,L,L,W,L,W,W,L,L,W,L,L };
  const vector<TOPO_FEATURE> r3 = { W,W,W,W,W,W,W,W,L,L,W,W,L };
  const vector<TOPO_FEATURE> r4 = { L,L,W,W,W,W,W,W,W,W,L,L,W };
  const vector<TOPO_FEATURE> r5 = { L,W,L,W,L,L,L,W,L,W,W,W,L };
  const vector<TOPO_FEATURE> r6 = { L,W,W,L,W,W,W,W,L,W,L,L,L };
  const vector<TOPO_FEATURE> r7 = { W,W,L,L,W,W,L,L,W,L,L,L,W };
  const vector<TOPO_FEATURE> r8 = { W,L,W,L,W,W,W,L,W,L,W,L,L };
  const vector<TOPO_FEATURE> r9 = { L,L,L,W,W,W,W,W,W,L,L,W,W };

  static vector<vector<TOPO_FEATURE>> testGrid = { r1, r2, r3, r4, r5, r6, r7, r8, r9 };

  const bool showRemovalSteps       = false;
  const bool showIntermediateCount  = true;
}

////////////////////////////////////////////////////////////////////////////////

void main()
{
  cout 
  << "The Problem : " << NL 
  << "Given a 2d grid map of '1's(land) and '0's(water), count the number of islands." << NL 
  << "An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically." << NL 
  << "You may assume all four edges of the grid are all surrounded by water." << NL << NL;

  PrintGrid(testGrid);

  vector<vector<TOPO_FEATURE>> paddedGrid = GetPaddedGrid(testGrid);

  const int islandsCount = CountIslands(paddedGrid, showIntermediateCount, showRemovalSteps);
  
  cout << NL << "Islands Counted : " << islandsCount << NL << endl;
}

////////////////////////////////////////////////////////////////////////////////