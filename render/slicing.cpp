#include "slicing.h"
#include "../model/block.h"
#include <iostream>
#include <vector>

using namespace std;

void sliceSoil(const vector<vector<vector<block>>> &arr, int &axis, int &layer,
               int &x, int &y, int &z) {
  int maxA, maxB;

  if (axis == 1) {
    maxA = y;
    maxB = z;
  }
  if (axis == 2) {
    maxA = x;
    maxB = z;
  }
  if (axis == 3) {
    maxA = x;
    maxB = y;
  }

  for (int a = 0; a < maxA; a++) {
    int oreCount = 0;
    for (int b = 0; b < maxB; b++) {
      block item;

      switch (axis) {
      case 1:
        item = arr[layer][a][b];
        break;
      case 2:
        item = arr[a][layer][b];
        break;
      case 3:
        item = arr[a][b][layer];
        break;
      }

      bool isOre = item.isOre;
      double grade = item.grade;

      string visual = !isOre        ? " ."
                      : grade < 0.5 ? " +"
                      : grade < 0.8 ? " #"
                                    : " @";

      cout << visual;

      if (isOre)
        oreCount++;
    }

    cout << "   *" << oreCount << endl;
  }
}
