#include "turn_manager.h"

namespace tactical_rpg {

void TurnManager::RollInitiative(std::vector<Character>& party,
                                 std::vector<Character>& enemies) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> d20(1, 20);

  turn_order_.clear();
  for (auto& p : party) {
    if (!p.is_downed) {
      p.initiative_roll = d20(gen) + p.stats.initiative_mod;
      turn_order_.push_back(&p);
    }
  }
  for (auto& e : enemies) {
    e.initiative_roll = d20(gen) + e.stats.initiative_mod;
    turn_order_.push_back(&e);
  }

  std::sort(turn_order_.begin(), turn_order_.end(),
            [](Character* a, Character* b) {
              return a->initiative_roll > b->initiative_roll;
            });

  current_index_ = -1;
}

void TurnManager::NextTurn() {
  if (turn_order_.empty()) return;
  current_index_ = (current_index_ + 1) % turn_order_.size();
  auto* active = turn_order_[current_index_];

  if (active->is_downed) {
    NextTurn();
    return;
  }

  // Reset turn state
  active->movement_remaining = active->stats.speed;
  active->action_used = false;
  active->bonus_action_used = false;
}

Character* TurnManager::GetActiveCharacter() const {
  if (current_index_ < 0 || current_index_ >= (int)turn_order_.size())
    return nullptr;
  return turn_order_[current_index_];
}

bool TurnManager::IsBattleOver(const std::vector<Character>& party,
                               const std::vector<Character>& enemies) const {
  bool all_enemies_down =
      std::all_of(enemies.begin(), enemies.end(),
                  [](const Character& c) { return c.is_downed; });
  bool all_party_down =
      std::all_of(party.begin(), party.end(),
                  [](const Character& c) { return c.is_downed; });
  return all_enemies_down || all_party_down;
}

}  // namespace tactical_rpg
