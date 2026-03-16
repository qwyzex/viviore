#include "seeding.h"
#include "../model/block.h"
#include "../model/orevoxel.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

Color getColor(double grade) {
  if (grade < 0.5)
    return YELLOW;
  else if (grade < 0.8)
    return ORANGE;
  else
    return RED;
}

void seedGrades(vector<vector<vector<block>>> &arr, vector<OreVoxel> &oreBlocks,
                int &x, int &y, int &z, float &depthCurve) {
  float curve = depthCurve > 0.0f ? depthCurve : 1.0f;

  const int dirs[6][3] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                          {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      // Depth ratio (0 = surface, 1 = deepest) according to rendering coords
      // (y=0 is bottom, y=y-1 is surface in the current renderer).
      float depthRatio = y > 1 ? (float)(y - 1 - j) / (float)(y - 1) : 0.0f;
      float depthBias = logf(1.0f + curve * depthRatio) / logf(1.0f + curve);
      float gradeDepthScale = 0.25f + 0.75f * depthBias;

      for (int k = 0; k < z; k++) {
        double baseGrade = (double)rand() / RAND_MAX;
        double total = 0;
        double currentGrade = arr[i][j][k].grade;

        for (auto &d : dirs) {
          int ni = i + d[0];
          int nj = j + d[1];
          int nk = k + d[2];

          if (ni < 0 || ni >= x || nj < 0 || nj >= y || nk < 0 || nk >= z)
            continue;

          if (!arr[ni][nj][nk].isOre)
            continue;

          double oreGrade = arr[ni][nj][nk].grade;
          total += oreGrade > currentGrade ? oreGrade / 2.0 : oreGrade;
        }

        if (arr[i][j][k].isOre) {
          arr[i][j][k].grade =
              (baseGrade * gradeDepthScale) + (0.1 * (total / 6.0));
          oreBlocks.push_back({
              i,
              j,
              k,
              arr[i][j][k].grade,
              getColor(arr[i][j][k].grade),
          });
        } else {
          arr[i][j][k].grade = 0.0;
        }
      }
    }
  }
}
