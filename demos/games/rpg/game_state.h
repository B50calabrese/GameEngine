#ifndef RPG_GAME_STATE_H_
#define RPG_GAME_STATE_H_

#include <algorithm>
#include <random>
#include <string>
#include <vector>

namespace rpg {

struct Attack {
  std::string name;
  float damage_multiplier;
  float hit_chance;
  int max_uses;
  int current_uses;

  Attack(std::string n, float dm, float hc, int mu)
      : name(n),
        damage_multiplier(dm),
        hit_chance(hc),
        max_uses(mu),
        current_uses(mu) {}
};

struct Stats {
  int hp;
  int max_hp;
  int atk;
  int def;
};

class GameState {
 public:
  Stats player_stats = {10, 10, 1, 1};
  std::vector<Attack> player_attacks;
  int current_floor = 1;
  bool is_run_active = false;

  static GameState& Get() {
    static GameState instance;
    return instance;
  }

  void ResetRun() {
    player_stats = {10, 10, 1, 1};
    player_attacks.clear();
    player_attacks.emplace_back("Quick Strike", 0.8f, 0.95f, 10);
    player_attacks.emplace_back("Heavy Blow", 1.5f, 0.70f, 5);
    current_floor = 1;
    is_run_active = true;
  }

  void RefreshAttackUses() {
    for (auto& attack : player_attacks) {
      attack.current_uses = attack.max_uses;
    }
  }

  std::vector<Attack> GetRandomAttacks(int count) {
    std::vector<Attack> pool = {
        {"Sunder", 1.2f, 0.85f, 8},        {"Drain", 0.7f, 0.90f, 6},
        {"Execute", 2.0f, 0.50f, 3},       {"Flurry", 0.3f, 1.0f, 15},
        {"Piercing Shot", 1.1f, 0.90f, 7}, {"Crushing Blow", 1.8f, 0.60f, 4}};

    std::vector<Attack> results;
    std::shuffle(pool.begin(), pool.end(),
                 std::mt19937(std::random_device()()));
    for (int i = 0; i < std::min(count, (int)pool.size()); ++i) {
      results.push_back(pool[i]);
    }
    return results;
  }

 private:
  GameState() = default;
};

}  // namespace rpg

#endif  // RPG_GAME_STATE_H_
