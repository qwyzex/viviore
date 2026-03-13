#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

int x = 30, y = 30, z = 30;
int baseCoef = 7, neighborIntensity = 12;

struct block {
  double grade;
  bool isOre;
};

void sliceSoil(const vector<vector<vector<block>>> &arr) {
  for (int i = 0; i < y; i++) {
    int oreCount = 0;
    for (int j = 0; j < z; j++) {
      bool isOre = arr[0][i][j].isOre;
      double grade = arr[0][i][j].grade;
      string visual = !isOre            ? " ."
                      : grade < 1.0 / 2 ? " +"
                      : grade < 4.0 / 5 ? " #"
                                        : " @";

      cout << visual;
      if (isOre == true) {
        oreCount++;
      }
    }
    cout << "   *" << oreCount << endl;
  }
}

void plantOres(vector<vector<vector<block>>> &arr) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        int neighbours = 0;

        for (int a = -1; a < 2; a += 2) {
          for (int b = -1; b < 2; b += 2) {
            for (int c = -1; c < 2; c += 2) {
              if (k > 0 && k < z - 1 && arr[i][j][k + c].isOre == true) {
                neighbours++;
              }
            }
            if (j > 0 && j < y - 1 && arr[i][j + b][k].isOre == true) {
              neighbours++;
            }
          }
          if (i > 0 && i < x - 1 && arr[i + a][j][k].isOre == true) {
            neighbours++;
          }
        }
        int chance = baseCoef + neighbours * neighborIntensity;

        arr[i][j][k].isOre = rand() % 100 < chance;
      }
    }
  }
}

void seedGrades(vector<vector<vector<block>>> &arr) {
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        arr[i][j][k].grade = (double)rand() / RAND_MAX;
      }
    }
  }
}

int main() {
  srand(time(NULL));

  vector<vector<vector<block>>> litho(
      x, vector<vector<block>>(y, vector<block>(z, {0.0, false})));

  cout << endl;
  cout << ">> Viviore 0.0.1 by @qwyzex" << endl;

  cout << "Corner Grade: " << litho[0][0][0].grade << endl;
  cout << "Base Coeficient: " << baseCoef << "%" << endl;
  cout << "Neighbouring Intensity: " << neighborIntensity << "%" << endl;

  plantOres(litho);
  seedGrades(litho);
  sliceSoil(litho);

  return 0;
}
