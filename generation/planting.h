#pragma once

#include "../model/block.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

// depthCurve: controls how quickly ore probability ramps up with depth (higher
// -> steeper/log-like) rootDepthLayers: minimum number of deepest layers to
// treat as "root" veins that all ore must connect to
void plantOres(vector<vector<vector<block>>> &arr, int &x, int &y, int &z,
               int &baseCoef, int &neighborIntensity, int &vacancyImpact,
               float &depthCurve, int &rootDepthLayers);
