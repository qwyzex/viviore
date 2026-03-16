#include "../model/orevoxel.h"
#include "raylib.h"
#include "raymath.h"
#define CAMERA_IMPLEMENTATION
#include "rcamera.h"
#include "rlgl.h"
#include "window.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

void DrawColoredGrid(int slices, float spacing, Color color) {
  float halfSlices = (float)slices / 2.0f;
  float step = spacing;
  float max = halfSlices * spacing;

  for (float i = -max; i <= max; i += step) {
    DrawLine3D((Vector3){i, 0.0f, -max}, (Vector3){i, 0.0f, max}, color);
    DrawLine3D((Vector3){-max, 0.0f, i}, (Vector3){max, 0.0f, i}, color);
  }
}

void castWindow(const vector<OreVoxel> &oreBlocks,
                const vector<OreVoxel> &surfaceBlocks, Axis &axis, int &x,
                int &y, int &z) {
  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1200, 900, "Viviore 0.0.1");
  SetTargetFPS(30);

  bool mode3D = false;
  bool simplified = true;

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
    ClearBackground(mode3D ? GRAY : RAYWHITE);

    if (IsKeyPressed(KEY_TAB))
      mode3D = !mode3D;

    if (IsKeyPressed(KEY_SPACE))
      simplified = !simplified;

    if (mode3D) {
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

      DrawColoredGrid(100, 1.0f, BLUE);

      float cx = x / 2.0f;
      float cy = y / 2.0f;
      float cz = z / 2.0f;

      // NEW
      Vector3 camForward =
          Vector3Normalize(Vector3Subtract(camera.target, camera.position));

      //   for (const auto &v : (simplified ? surfaceBlocks : oreBlocks)) {
      //     DrawCube({(float)v.x - cx, (float)v.y - cy, (float)v.z - cz}, 1.0f,
      //              1.0f, 1.0f, Fade(v.displayColor, 0.6f));
      //   };

      for (const auto &v : (simplified ? surfaceBlocks : oreBlocks)) {
        Vector3 pos = {(float)v.x - cx, (float)v.y - cy, (float)v.z - cz};

        Vector3 camToVoxel = Vector3Subtract(pos, camera.position);

        float dot = Vector3DotProduct(camToVoxel, camForward);
        float dist = Vector3Length(camToVoxel);

        if (dot < 0)
          continue; // voxel is behind camera
        if (dist > 200.0f)
          continue;

        DrawCube(pos, 1.0f, 1.0f, 1.0f, Fade(v.displayColor, 0.6f));
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

      slice = max(0, min(slice, maxSlice - 1));

      // 3D CUBE SLICE OVERVIEW
      Rectangle viewport = {920, 620, 240, 180};
      rlViewport(viewport.x, viewport.y, viewport.width, viewport.height);
      BeginMode3D(miniCam);

      float maxDim = max({(float)x, (float)y, (float)z});
      float sx = 4 * x / maxDim;
      float sy = 4 * y / maxDim;
      float sz = 4 * z / maxDim;
      DrawCubeWires({0, 0, 0}, sx, sy, sz, BLACK);

      float p;
      if (axis == X)
        p = (float)slice / (x - 1) * sx - sx / 2;
      if (axis == Y)
        p = (float)slice / (y - 1) * sy - sy / 2;
      if (axis == Z)
        p = (float)slice / (z - 1) * sz - sz / 2;

      if (axis == X)
        DrawCube({p, 0, 0}, 0.05f, sy, sz, Fade(RED, 0.5f));
      if (axis == Y)
        DrawCube({0, p, 0}, sx, 0.05f, sz, Fade(RED, 0.5f));
      if (axis == Z)
        DrawCube({0, 0, p}, sx, sy, 0.05f, Fade(RED, 0.5f));

      // 3-axes indicator
      Vector3 origin = {-sx / 2, -sy / 2, -sz / 2};

      DrawLine3D(origin, {origin.x + sx, origin.y, origin.z}, RED);
      DrawLine3D(origin, {origin.x, origin.y + sy, origin.z}, GREEN);
      DrawLine3D(origin, {origin.x, origin.y, origin.z + sz}, BLUE);

      EndMode3D();
      rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());

      // OVERLAY
      string axisName = axis == X   ? "X (Left-Right)"
                        : axis == Y ? "Y (Depth)"
                                    : "Z (Front-Back)";

      DrawText(TextFormat("Axis: %s", axisName.c_str()), 920, 20, 20, BLACK);
      DrawText(TextFormat("Slice: %d / %d", slice, maxSlice - 1), 920, 50, 20,
               BLACK);

      DrawRectangle(0, 0, width * cellSize, height * cellSize, LIGHTGRAY);

      for (const auto &v : oreBlocks) {
        if ((axis == X && v.x != slice) || (axis == Y && v.y != slice) ||
            (axis == Z && v.z != slice))
          continue;

        int a, b;

        if (axis == X) {
          a = v.y;
          b = v.z;
        }
        if (axis == Y) {
          a = v.x;
          b = v.z;
        }
        if (axis == Z) {
          a = v.x;
          b = v.y;
        }

        DrawRectangle(a * cellSize, b * cellSize, cellSize, cellSize,
                      v.displayColor);
      }

      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize,
                             DARKGRAY);
        }
      }

      EndDrawing();
    }
  }

  CloseWindow();
};
