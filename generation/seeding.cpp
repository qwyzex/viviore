#include "seeding.h"
#include "../model/block.h"
#include "../model/orevoxel.h"
#include "raylib.h"
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
                int &x, int &y, int &z) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        double baseGrade = (double)rand() / RAND_MAX;
        double total = 0;

        for (int a = -1; a < 2; a += 2) {
          for (int b = -1; b < 2; b += 2) {
            for (int c = -1; c < 2; c += 2) {
              block ore = arr[i][j][k + c];
              if (k > 0 && k < z - 1 && ore.isOre == true) {
                total +=
                    ore.grade > arr[i][j][k].grade ? ore.grade / 2 : ore.grade;
              }
            }
            if (j > 0 && j < y - 1 && arr[i][j + b][k].isOre == true) {
              block ore = arr[i][j + b][k];
              total +=
                  ore.grade > arr[i][j][k].grade ? ore.grade / 2 : ore.grade;
            }
          }
          if (i > 0 && i < x - 1 && arr[i + a][j][k].isOre == true) {
            block ore = arr[i + a][j][k];
            total += ore.grade > arr[i][j][k].grade ? ore.grade / 2 : ore.grade;
          }
        }

        if (arr[i][j][k].isOre) {
          arr[i][j][k].grade = baseGrade + ((0.1) * (total / 6));
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
