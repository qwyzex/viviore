#pragma once
#include "../model/block.h"
#include "slicing.h"
#include <iostream>
#include <vector>

using namespace std;

void sliceSoil(const vector<vector<vector<block>>> &arr, int &axis, int &layer,
               int &x, int &y, int &z);
