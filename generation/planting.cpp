#include "planting.h"
#include "../model/block.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <time.h>
#include <tuple>
#include <vector>

using namespace std;

static inline float clampFloat(float v, float lo, float hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

void plantOres(vector<vector<vector<block>>> &arr, int &x, int &y, int &z,
               int &baseCoef, int &neighborIntensity, int &vacancyImpact,
               float depthCurve, int rootDepthLayers) {
  // Ensure parameters are sane
  float curve = depthCurve > 0.0f ? depthCurve : 1.0f;
  int roots = max(1, min(y, rootDepthLayers));

  // Phase 1: Base ore placement influenced by depth and neighbor/ vacancy bias
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      // Depth ratio (0 = surface, 1 = deepest layer)
      float depthRatio = y > 1 ? (float)j / (float)(y - 1) : 0.0f;
      // Log-like curve to make ore rare near surface and rapidly increase with
      // depth
      float depthFactor = logf(1.0f + curve * depthRatio) / logf(1.0f + curve);
      // Keep a small probability near surface so some veins can still form
      float depthScale = clampFloat(0.05f + 0.95f * depthFactor, 0.0f, 1.0f);

      for (int k = 0; k < z; k++) {
        int neighbours = 0;
        int vacancy = 0;

        for (int a = -1; a < 2; a += 2) {
          for (int b = -1; b < 2; b += 2) {
            for (int c = -1; c < 2; c += 2) {
              if (k > 0 && k < z - 1 && arr[i][j][k + c].isOre) {
                neighbours++;
              } else {
                vacancy++;
              }
            }
            if (j > 0 && j < y - 1 && arr[i][j + b][k].isOre) {
              neighbours++;
            } else {
              vacancy++;
            }
          }
          if (i > 0 && i < x - 1 && arr[i + a][j][k].isOre) {
            neighbours++;
          } else {
            vacancy++;
          }
        }

        float chance = (float)baseCoef + (float)neighbours * neighborIntensity -
                       (float)vacancy * vacancyImpact * baseCoef * 0.015f;

        // Depth bias makes ore less likely near surface and more likely deeper
        // down
        chance *= depthScale;
        chance = clampFloat(chance, 0.0f, 100.0f);

        arr[i][j][k].isOre = rand() % 100 < (int)chance;
      }
    }
  }

  // Phase 2: prune isolated vein pockets not connected to deep "root" veins
  vector<vector<vector<bool>>> visited(
      x, vector<vector<bool>>(y, vector<bool>(z, false)));
  queue<tuple<int, int, int>> q;

  int rootStart = max(0, y - roots);
  for (int i = 0; i < x; i++) {
    for (int j = rootStart; j < y; j++) {
      for (int k = 0; k < z; k++) {
        if (arr[i][j][k].isOre) {
          visited[i][j][k] = true;
          q.push({i, j, k});
        }
      }
    }
  }

  const int dirs[6][3] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                          {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

  while (!q.empty()) {
    auto [ci, cj, ck] = q.front();
    q.pop();

    for (auto &d : dirs) {
      int ni = ci + d[0];
      int nj = cj + d[1];
      int nk = ck + d[2];

      if (ni < 0 || ni >= x || nj < 0 || nj >= y || nk < 0 || nk >= z)
        continue;

      if (!visited[ni][nj][nk] && arr[ni][nj][nk].isOre) {
        visited[ni][nj][nk] = true;
        q.push({ni, nj, nk});
      }
    }
  }

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        if (arr[i][j][k].isOre && !visited[i][j][k]) {
          arr[i][j][k].isOre = false;
        }
      }
    }
  }
}
