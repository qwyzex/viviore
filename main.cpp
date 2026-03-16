#include "raylib.h"
#include "raymath.h"
// #define CAMERA_IMPLEMENTATION
#include "generation/planting.h"
#include "generation/seeding.h"
#include "model/axis.h"
#include "model/block.h"
#include "model/presentation.h"
#include "processing/surface.h"
#include "rcamera.h"
#include "render/export.h"
#include "render/slicing.h"
#include "render/window.h"
#include "rlgl.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

int x = 40, y = 40, z = 40;
int baseCoef = 4, neighborIntensity = 10, vacancyImpact = 3;
int influence = 3;

int main() {
  srand(time(NULL));
  vector<vector<vector<block>>> litho(
      x, vector<vector<block>>(y, vector<block>(z, {0.0, false})));
  vector<OreVoxel> oreBlocks;

  cout << endl;
  cout << ">> Viviore 0.0.1 by @qwyzex" << endl;

  cout << "Corner Grade: " << litho[0][0][0].grade << endl;
  cout << "Base Coeficient: " << baseCoef << "%" << endl;
  cout << "Neighbouring Intensity: " << neighborIntensity << "%" << endl;

  Axis currentAxis = Y;

  plantOres(litho, x, y, z, baseCoef, neighborIntensity, vacancyImpact);
  seedGrades(litho, oreBlocks, x, y, z);
  //   sliceSoil(litho, Y, 10, x, y, z);

  vector<OreVoxel> surfaceBlocks = extractSurface(oreBlocks);

  exportOre(litho, x, y, z);
  castWindow(oreBlocks, surfaceBlocks, currentAxis, x, y, z);

  return 0;
}
