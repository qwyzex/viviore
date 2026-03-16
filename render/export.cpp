#include "export.h"
#include "../model/block.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void exportOre(const vector<vector<vector<block>>> &arr, int &x, int &y,
               int &z) {
  ofstream file("orebody.csv");

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {

        if (arr[i][j][k].isOre) {
          file << i << "," << j << "," << k << "," << arr[i][j][k].grade
               << "\n";
        }
      }
    }
  }
}
