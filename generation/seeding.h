#pragma once
#include "../model/block.h"
#include "../model/orevoxel.h"
#include "seeding.h"
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

void seedGrades(vector<vector<vector<block>>> &arr, vector<OreVoxel> &oreBlocks,
                int &x, int &y, int &z);
