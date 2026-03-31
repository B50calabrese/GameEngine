#ifndef RPG_MAP_GENERATOR_H_
#define RPG_MAP_GENERATOR_H_

#include <algorithm>
#include <random>
#include <vector>

namespace rpg {

enum class TileType { kFloor, kWall, kStairs, kChest };

struct Room {
  int x, y, w, h;
  bool Intersects(const Room& other) const {
    return (x < other.x + other.w && x + w > other.x && y < other.y + other.h &&
            y + h > other.y);
  }
};

class MapGenerator {
 public:
  struct MapData {
    int width;
    int height;
    std::vector<TileType> tiles;
    int start_x;
    int start_y;
  };

  static MapData Generate(int width, int height, int seed) {
    MapData data;
    data.width = width;
    data.height = height;
    data.tiles.resize(width * height, TileType::kWall);

    std::mt19937 gen(seed);
    std::vector<Room> rooms;

    const int max_rooms = 10;
    const int min_size = 3;
    const int max_size = 7;

    for (int i = 0; i < max_rooms; ++i) {
      int w = std::uniform_int_distribution<>(min_size, max_size)(gen);
      int h = std::uniform_int_distribution<>(min_size, max_size)(gen);
      int x = std::uniform_int_distribution<>(1, width - w - 1)(gen);
      int y = std::uniform_int_distribution<>(1, height - h - 1)(gen);

      Room new_room{x, y, w, h};
      bool intersect = false;
      for (const auto& r : rooms) {
        if (new_room.Intersects(r)) {
          intersect = true;
          break;
        }
      }

      if (!intersect) {
        for (int ry = y; ry < y + h; ++ry) {
          for (int rx = x; rx < x + w; ++rx) {
            data.tiles[ry * width + rx] = TileType::kFloor;
          }
        }

        if (rooms.empty()) {
          data.start_x = x + w / 2;
          data.start_y = y + h / 2;
        } else {
          int prev_x = rooms.back().x + rooms.back().w / 2;
          int prev_y = rooms.back().y + rooms.back().h / 2;
          int curr_x = x + w / 2;
          int curr_y = y + h / 2;

          for (int cx = std::min(prev_x, curr_x);
               cx <= std::max(prev_x, curr_x); ++cx) {
            data.tiles[prev_y * width + cx] = TileType::kFloor;
          }
          for (int cy = std::min(prev_y, curr_y);
               cy <= std::max(prev_y, curr_y); ++cy) {
            data.tiles[cy * width + curr_x] = TileType::kFloor;
          }
        }
        rooms.push_back(new_room);
      }
    }

    if (!rooms.empty()) {
      int last_x = rooms.back().x + rooms.back().w / 2;
      int last_y = rooms.back().y + rooms.back().h / 2;
      data.tiles[last_y * width + last_x] = TileType::kStairs;

      for (size_t i = 1; i < rooms.size() - 1; ++i) {
        if (std::uniform_real_distribution<>(0.0, 1.0)(gen) < 0.5) {
          int cx = rooms[i].x +
                   std::uniform_int_distribution<>(0, rooms[i].w - 1)(gen);
          int cy = rooms[i].y +
                   std::uniform_int_distribution<>(0, rooms[i].h - 1)(gen);
          if (data.tiles[cy * width + cx] == TileType::kFloor) {
            data.tiles[cy * width + cx] = TileType::kChest;
          }
        }
      }
    }

    return data;
  }
};

}  // namespace rpg

#endif  // RPG_MAP_GENERATOR_H_
