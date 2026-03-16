#include "raylib.h"
#include "raymath.h"
#define CAMERA_IMPLEMENTATION
#include "rcamera.h"
#include "rlgl.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

int x = 40, y = 50, z = 60;
// int x = 40, y = 40, z = 40;
int baseCoef = 4, neighborIntensity = 10, vacancyImpact = 3;
int influence = 3;

struct block {
  double grade;
  bool isOre;
};

enum Axis { X = 1, Y = 2, Z = 3 };

enum Presentation { D3 = 3, D2 = 2 };

void sliceSoil(const vector<vector<vector<block>>> &arr, int axis, int layer) {
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

void plantOres(vector<vector<vector<block>>> &arr) {
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

void seedGrades(vector<vector<vector<block>>> &arr) {
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
        } else {
          arr[i][j][k].grade = 0.0;
        }
      }
    }
  }
}

void exportOre(const vector<vector<vector<block>>> &arr) {
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

int AxisSize(Axis axis) {
  if (axis == X)
    return x;
  if (axis == Y)
    return y;
  return z;
};

void DrawColoredGrid(int slices, float spacing, Color color) {
  float halfSlices = (float)slices / 2.0f;
  float step = spacing;
  float max = halfSlices * spacing;

  for (float i = -max; i <= max; i += step) {
    DrawLine3D((Vector3){i, 0.0f, -max}, (Vector3){i, 0.0f, max}, color);
    DrawLine3D((Vector3){-max, 0.0f, i}, (Vector3){max, 0.0f, i}, color);
  }
}

void castWindow(const vector<vector<vector<block>>> &arr, Axis &axis) {
  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1200, 900, "Viviore 0.0.1");
  SetTargetFPS(30);

  bool mode3D = false;

  Camera3D camera = {0};
  camera.position = {70.0f, 70.0f, 70.0f};
  camera.target = {0.0f, 0.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.fovy = mode3D ? 60.0f : 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Camera3D miniCam = {0};
  miniCam.position = {6.0f, 6.0f, 6.0f};
  miniCam.target = {0.0f, 0.0f, 0.0f};
  miniCam.up = {0.0f, 1.0f, 0.0f};
  miniCam.fovy = 45.0f;
  miniCam.projection = CAMERA_PERSPECTIVE;

  int slice = 0;

  int maxSlice;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(mode3D ? BLACK : RAYWHITE);

    if (IsKeyPressed(KEY_TAB))
      mode3D = !mode3D;

    if (mode3D) {
      // UpdateCamera(&camera, CAMERA_FREE);

      BeginMode3D(camera);
      BeginBlendMode(BLEND_ALPHA);

      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        DisableCursor();
      }

      if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        UpdateCamera(&camera, CAMERA_FREE);
        DisableCursor();

        Vector2 delta = GetMouseDelta();
        float sensitivity = 0.003f;

        Vector3 rotation = {-delta.y * sensitivity, -delta.x * sensitivity, 0};

        Matrix rot = MatrixRotateXYZ(rotation);

        Vector3 dir = Vector3Subtract(camera.target, camera.position);
        dir = Vector3Transform(dir, rot);

        camera.target = Vector3Add(camera.position, dir);
      }

      if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        EnableCursor();
        GetMouseDelta();
      }

      if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
        Vector2 delta = GetMouseDelta();

        camera.target.x -= delta.x * 0.05f;
        camera.target.y += delta.y * 0.05f;

        camera.position.x -= delta.x * 0.05f;
        camera.position.y += delta.y * 0.05f;
      }

      float speed = 0.5f;

      Vector3 forward =
          Vector3Normalize(Vector3Subtract(camera.target, camera.position));
      Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

      forward.y = 0;
      forward = Vector3Normalize(forward);

      if (IsKeyDown(KEY_W)) {
        camera.position =
            Vector3Add(camera.position, Vector3Scale(forward, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(forward, speed));
      }

      if (IsKeyDown(KEY_S)) {
        camera.position =
            Vector3Subtract(camera.position, Vector3Scale(forward, speed));
        camera.target =
            Vector3Subtract(camera.target, Vector3Scale(forward, speed));
      }

      if (IsKeyDown(KEY_A)) {
        camera.position =
            Vector3Subtract(camera.position, Vector3Scale(right, speed));
        camera.target =
            Vector3Subtract(camera.target, Vector3Scale(right, speed));
      }

      if (IsKeyDown(KEY_D)) {
        camera.position =
            Vector3Add(camera.position, Vector3Scale(right, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, speed));
      }

      float wheel = GetMouseWheelMove();
      camera.position.x +=
          wheel * (camera.target.x - camera.position.x) * 0.05f;
      camera.position.y +=
          wheel * (camera.target.y - camera.position.y) * 0.05f;
      camera.position.z +=
          wheel * (camera.target.z - camera.position.z) * 0.05f;

      DrawColoredGrid(100, 1.0f, PURPLE);

      for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
          for (int k = 0; k < z; k++) {
            if (arr[i][j][k].isOre) {
              float cx = x / 2.0f;
              float cy = y / 2.0f;
              float cz = z / 2.0f;
              Color oreColor;

              double g = arr[i][j][k].grade;

              if (g < 0.5)
                oreColor = Fade(YELLOW, 0.6f);
              else if (g < 0.8)
                oreColor = Fade(ORANGE, 0.6f);
              else
                oreColor = Fade(RED, 0.6f);
              // if (g < 0.5)
              //   oreColor = YELLOW;
              // else if (g < 0.8)
              //   oreColor = ORANGE;
              // else
              //   oreColor = RED;

              DrawCube({(float)i - cx, (float)j - cy, (float)k - cz}, 1.0f,
                       1.0f, 1.0f, oreColor);
              DrawCubeWires({(float)i - cx, (float)j - cy, (float)k - cz}, 1.0f,
                            1.0f, 1.0f, RAYWHITE);
            }
          }

      EndMode3D();
      EndDrawing();
    } else {
      // KEYBINDS
      if (IsKeyPressed(KEY_RIGHT))
        slice = min(slice + 1, maxSlice - 1);
      if (IsKeyPressed(KEY_LEFT))
        slice = max(slice - 1, 0);

      if (IsKeyPressed(KEY_UP))
        axis = (Axis)((axis % 3) + 1);
      if (IsKeyPressed(KEY_DOWN))
        axis = (Axis)((axis + 1) % 3 + 1);

      // PRIMITIVES
      int width, height;

      switch (axis) {
      case X:
        width = y;
        height = z;
        break;
      case Y:
        width = x;
        height = z;
        break;
      case Z:
        width = x;
        height = y;
        break;
      }

      float cellSize = min(900.0f / width, 900.0f / height);

      switch (axis) {
      case X:
        maxSlice = x;
        break;
      case Y:
        maxSlice = y;
        break;
      case Z:
        maxSlice = z;
        break;
      }

      slice = std::max(0, std::min(slice, maxSlice - 1));

      // 3D CUBE SLICE OVERVIEW
      Rectangle viewport = {920, 620, 240, 180};
      rlViewport(viewport.x, viewport.y, viewport.width, viewport.height);

      BeginMode3D(miniCam);
      DrawCubeWires({0, 0, 0}, 4, 4, 4, DARKGRAY);
      float p = (maxSlice > 1) ? (float)slice / (maxSlice - 1) * 4 - 2 : 0;

      if (axis == X)
        DrawCube({p, 0, 0}, 0.05f, 4, 4, Fade(RED, 0.5f));
      if (axis == Y)
        DrawCube({0, p, 0}, 4, 0.05f, 4, Fade(RED, 0.5f));
      if (axis == Z)
        DrawCube({0, 0, p}, 4, 4, 0.05f, Fade(RED, 0.5f));

      DrawLine3D({0, 0, 0}, {2.9, 0, 0}, RED);
      DrawLine3D({0, 0, 0}, {0, 2.9, 0}, GREEN);
      DrawLine3D({0, 0, 0}, {0, 0, 2.9}, BLUE);

      EndMode3D();
      rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());

      // OVERLAY
      string axisName = axis == X   ? "X (Left-Right)"
                        : axis == Y ? "Y (Depth)"
                                    : "Z (Front-Back)";

      DrawText(TextFormat("Axis: %s", axisName.c_str()), 920, 20, 20, BLACK);
      DrawText(TextFormat("Slice: %d / %d", slice, maxSlice - 1), 920, 50, 20,
               BLACK);

      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          block item;

          switch (axis) {
          case X:
            item = arr[slice][i][j];
            break;
          case Y:
            item = arr[i][slice][j];
            break;
          case Z:
            item = arr[i][j][slice];
            break;
          }

          Color c = LIGHTGRAY;

          if (item.isOre) {
            if (item.grade < 0.5)
              c = YELLOW;
            else if (item.grade < 0.8)
              c = ORANGE;
            else
              c = RED;
          }

          DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize,
                             DARKGRAY);
          DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, c);
        }
      }

      EndDrawing();
    }
  }

  CloseWindow();
};

int main() {
  srand(time(NULL));

  vector<vector<vector<block>>> litho(
      x, vector<vector<block>>(y, vector<block>(z, {0.0, false})));

  cout << endl;
  cout << ">> Viviore 0.0.1 by @qwyzex" << endl;

  cout << "Corner Grade: " << litho[0][0][0].grade << endl;
  cout << "Base Coeficient: " << baseCoef << "%" << endl;
  cout << "Neighbouring Intensity: " << neighborIntensity << "%" << endl;

  Axis currentAxis = Y;

  plantOres(litho);
  seedGrades(litho);
  //   sliceSoil(litho, Y, 10);

  exportOre(litho);
  // castWindow3D(litho);
  castWindow(litho, currentAxis);

  return 0;
}
