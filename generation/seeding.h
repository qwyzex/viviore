#pragma once

#include "../model/block.h"
#include "../model/orevoxel.h"
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

// depthCurve: controls how strongly grade is biased by depth (higher -> deeper
// ore tends to be higher grade)
void seedGrades(vector<vector<vector<block>>> &arr, vector<OreVoxel> &oreBlocks,
                int &x, int &y, int &z, float depthCurve = 10.0f);
