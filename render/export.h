#pragma once
#include "../model/block.h"
#include "export.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void exportOre(const vector<vector<vector<block>>> &arr, int &x, int &y,
               int &z);
