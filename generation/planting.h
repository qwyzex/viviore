#pragma once
#include "../model/block.h"
#include "planting.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

void plantOres(vector<vector<vector<block>>> &arr, int &x, int &y, int &z,
               int &baseCoef, int &neighborIntensity, int &vacancyImpact);
