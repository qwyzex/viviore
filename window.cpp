#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#include <fstream>

using namespace std;
struct block {
  double grade;
  bool isOre;
};

int x = 40, y = 40, z = 40;
int main() {
  vector<vector<vector<block>>> litho(
      x, vector<vector<block>>(y, vector<block>(z, {0.0, false})));

  InitWindow(1000, 800, "Viviore");

  Camera camera = {0};
  camera.position = {40.0f, 40.0f, 40.0f};
  camera.target = {0.0f, 0.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    for (int i = 0; i < x; i++)
      for (int j = 0; j < y; j++)
        for (int k = 0; k < z; k++)
          if (litho[i][j][k].isOre)
            DrawCube({(float)i, (float)j, (float)k}, 1, 1, 1, RED);

    EndMode3D();
    EndDrawing();
  }
}
