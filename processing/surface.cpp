#include "surface.h"
#include <unordered_set>

using namespace std;

struct VoxelKey {
  int x, y, z;

  bool operator==(const VoxelKey &o) const {
    return x == o.x && y == o.y && z == o.z;
  }
};

struct VoxelKeyHash {
  size_t operator()(const VoxelKey &v) const {
    return v.x * 73856093 ^ v.y * 19349663 ^ v.z * 83492791;
  }
};

vector<OreVoxel> extractSurface(const vector<OreVoxel> &oreBlocks) {
  unordered_set<VoxelKey, VoxelKeyHash> lookup;

  for (const auto &v : oreBlocks)
    lookup.insert({v.x, v.y, v.z});

  vector<OreVoxel> surface;

  for (const auto &v : oreBlocks) {
    bool exposed = false;

    if (!lookup.count({v.x + 1, v.y, v.z}))
      exposed = true;
    if (!lookup.count({v.x - 1, v.y, v.z}))
      exposed = true;
    if (!lookup.count({v.x, v.y + 1, v.z}))
      exposed = true;
    if (!lookup.count({v.x, v.y - 1, v.z}))
      exposed = true;
    if (!lookup.count({v.x, v.y, v.z + 1}))
      exposed = true;
    if (!lookup.count({v.x, v.y, v.z - 1}))
      exposed = true;

    if (exposed)
      surface.push_back(v);
  }

  return surface;
}
