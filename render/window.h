#include "../model/orevoxel.h"
#include <vector>
#pragma once
#include "raylib.h"
#include "raymath.h"
#define CAMERA_IMPLEMENTATION
#include "../model/axis.h"
#include "../model/block.h"
#include "rcamera.h"
#include "rlgl.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

void castWindow(vector<OreVoxel> &oreBlocks, vector<OreVoxel> &surfaceBlocks,
                Axis &axis, int &x, int &y, int &z,
                vector<vector<vector<block>>> &litho, int &baseCoef,
                int &neighborIntensity, int &vacancyImpact, float &depthCurve,
                int &rootDepthLayers);
