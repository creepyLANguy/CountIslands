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
#include <algorithm>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

enum TOPO_FEATURE
{
  WATER = 0,
  LAND  = 1,
};

struct PrintPair
{
  TOPO_FEATURE feature;
  const char* print_symbol;
  
  PrintPair(const TOPO_FEATURE& feature, const char* print_symbol) 
  : 
  feature(feature), 
  print_symbol(print_symbol)
  {}
  
  bool operator < (const PrintPair& pair) const
  {
    return (feature < pair.feature);
  }
};

//NOTE
//Try and initialise with the same order as the TOPO_FEATURE enum, 
//so print function can easily find corresponding printable string.
//Edit: 
//We do a sort on this vector before using it, but still, don't rely on that.
vector<PrintPair> printMap =
{
  PrintPair(WATER, " "),
  PrintPair(LAND, "+")
};

const char* NL = "\n";

////////////////////////////////////////////////////////////////////////////////

void PrintGrid(const vector<vector<TOPO_FEATURE>>& grid, const bool& isPadded = true)
{
  const int padding = isPadded ? 1 : 0;

  for (unsigned int y = padding; y < grid.size() - padding; ++y)
  {
    for (unsigned int x = padding; x < grid[y].size() - padding; ++x)
    {
      cout << printMap[grid[y][x]].print_symbol;
    }
    cout << NL;
  }

  cout << NL;
}

////////////////////////////////////////////////////////////////////////////////

void RecursivelyConvertSelfAndNeighbours(
  vector<vector<TOPO_FEATURE>>& grid, 
  const int& x, 
  const int& y, 
  const TOPO_FEATURE& ignorableFeature, 
  const bool& showMaps = true,
  const bool& showRemovalSteps = true
)
{
  if (grid[y][x] == ignorableFeature)
  {
    return;
  }

  if (showMaps && showRemovalSteps)
  {
    PrintGrid(grid);
  }

  grid[y][x] = ignorableFeature;

  RecursivelyConvertSelfAndNeighbours(grid, x-1, y, ignorableFeature, showMaps, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x+1, y, ignorableFeature, showMaps, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x, y-1, ignorableFeature, showMaps, showRemovalSteps);
  RecursivelyConvertSelfAndNeighbours(grid, x, y+1, ignorableFeature, showMaps, showRemovalSteps);
}

////////////////////////////////////////////////////////////////////////////////

vector<TOPO_FEATURE> GetHorizontalPad(const vector<vector<TOPO_FEATURE>>& grid, const TOPO_FEATURE& ignorableFeature = WATER)
{
  vector<TOPO_FEATURE> horizontalPad;
  for (const int position : grid[0])
  {
    horizontalPad.push_back(ignorableFeature);
  }
  return horizontalPad;
}

////////////////////////////////////////////////////////////////////////////////

vector<vector<TOPO_FEATURE>> GetPaddedGrid(const vector<vector<TOPO_FEATURE>>& grid, const TOPO_FEATURE& ignorableFeature = WATER)
{
  const vector<TOPO_FEATURE> horizontalPad = GetHorizontalPad(grid, ignorableFeature);

  vector<vector<TOPO_FEATURE>> paddedGrid;

  paddedGrid.push_back(horizontalPad);

  for (vector<TOPO_FEATURE> currentRow : grid)
  {
    vector<TOPO_FEATURE> paddedRow;

    paddedRow.push_back(ignorableFeature);

    for (TOPO_FEATURE topo_feature : currentRow)
    {
      paddedRow.push_back(topo_feature);
    }

    paddedRow.push_back(ignorableFeature);

    paddedGrid.push_back(paddedRow);

    paddedRow.push_back(ignorableFeature);
  }

  paddedGrid.push_back(horizontalPad);

  return paddedGrid;
}

////////////////////////////////////////////////////////////////////////////////

int CountIslands(
  const vector<vector<TOPO_FEATURE>>& grid,
  const bool& showMaps                            = true,
  const bool& showIntermediateCount               = true, 
  const bool& showRemovalSteps                    = true,
  const vector<TOPO_FEATURE>& countableFeatures   = { LAND },
  const TOPO_FEATURE& ignorableFeature            = WATER
)
{
  //Just in case it was initialised in an order different to TOPO_FEATURE enum.
  sort(printMap.begin(), printMap.end());

  vector<vector<TOPO_FEATURE>> paddedGrid = GetPaddedGrid(grid, ignorableFeature);

  int count = 0;

  for (unsigned int y = 1; y < paddedGrid.size() - 1; ++y)
  {
    for (unsigned int x = 1; x < paddedGrid[y].size() - 1; ++x)
    {
      //This topo feature is not in the list of countable topo features.
      if (find(countableFeatures.begin(), countableFeatures.end(), paddedGrid[y][x]) == countableFeatures.end())
      {
        continue;
      }

      ++count;

      if (showIntermediateCount)
      {
        cout << NL << count << NL;
      }

      RecursivelyConvertSelfAndNeighbours(paddedGrid, x, y, ignorableFeature, showMaps, showRemovalSteps);

      if (showMaps)
      {
        PrintGrid(paddedGrid);
      }
      
    }
  }

  return count;
}

////////////////////////////////////////////////////////////////////////////////

//test data
namespace
{
  const TOPO_FEATURE _ = WATER;
  const TOPO_FEATURE X = LAND;

  //Hmm, I can see 14 islands here? 
  const vector<vector<TOPO_FEATURE>> testGrid =
  {
	  { X,X,_,_,X,X,_,X,_,X,_,X,_ },
	  { X,X,X,X,_,X,_,_,X,X,_,X,X },
	  { _,_,_,_,_,_,_,_,X,X,_,_,X },
	  { X,X,_,_,_,_,_,_,_,_,X,X,_ },
	  { X,_,X,_,X,X,X,_,X,_,_,_,X },
	  { X,_,_,X,_,_,_,_,X,_,X,X,X },
	  { _,_,X,X,_,_,X,X,_,X,X,X,_ },
	  { _,X,_,X,_,_,_,X,_,X,_,X,X },
	  { X,X,X,_,_,_,_,_,_,X,X,_,_ }
  };

  vector<TOPO_FEATURE> testCountableFeatures = { LAND };

  const bool showMaps               = true;
  const bool showRemovalSteps       = false;
  const bool showIntermediateCount  = true;
}

////////////////////////////////////////////////////////////////////////////////

void Run()
{
  cout 
  << "The Problem : " << NL 
  << "Given a 2d grid map of '1's(land) and '0's(water), count the number of islands." << NL 
  << "An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically." << NL 
  << "You may assume all four edges of the grid are all surrounded by water." << NL << NL;

  PrintGrid(testGrid, false);

  const int islandsCount = CountIslands(testGrid, showMaps, showIntermediateCount, showRemovalSteps, testCountableFeatures, WATER);
  
  cout << NL << "Islands Counted : " << islandsCount << NL << endl;
}

////////////////////////////////////////////////////////////////////////////////

void main()
{
  Run();
}
