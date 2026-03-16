#include "planting.h"
#include "../model/block.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

void plantOres(vector<vector<vector<block>>> &arr, int &x, int &y, int &z,
               int &baseCoef, int &neighborIntensity, int &vacancyImpact) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        int neighbours = 0;
        int vacancy = 0;

        for (int a = -1; a < 2; a += 2) {
          for (int b = -1; b < 2; b += 2) {
            for (int c = -1; c < 2; c += 2) {
              if (k > 0 && k < z - 1 && arr[i][j][k + c].isOre == true) {
                neighbours++;
              } else {
                vacancy++;
              }
            }
            if (j > 0 && j < y - 1 && arr[i][j + b][k].isOre == true) {
              neighbours++;
            } else {
              vacancy++;
            }
          }
          if (i > 0 && i < x - 1 && arr[i + a][j][k].isOre == true) {
            neighbours++;
          } else {
            vacancy++;
          }
        }
        int chance = baseCoef + (neighbours * neighborIntensity) -
                     (vacancy * vacancyImpact * baseCoef * 0.015);

        arr[i][j][k].isOre = rand() % 100 < chance;
      }
    }
  }
}
