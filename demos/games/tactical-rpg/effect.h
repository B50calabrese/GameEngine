#ifndef DEMOS_GAMES_TACTICAL_RPG_EFFECT_H_
#define DEMOS_GAMES_TACTICAL_RPG_EFFECT_H_

#include <memory>
#include <string>

#include "game_types.h"

namespace tactical_rpg {

class Effect {
 public:
  virtual ~Effect() = default;
  virtual void Apply(Character* actor, Character* target) = 0;
  virtual std::string GetLogMessage(Character* actor,
                                    Character* target) const = 0;
};

class DamageEffect : public Effect {
 public:
  DamageEffect(int sides, int count, int mod)
      : sides_(sides), count_(count), mod_(mod) {}

  void Apply(Character* actor, Character* target) override;
  std::string GetLogMessage(Character* actor, Character* target) const override;

 private:
  int sides_, count_, mod_;
  int last_damage_ = 0;
};

class HealEffect : public Effect {
 public:
  HealEffect(int sides, int count, int mod)
      : sides_(sides), count_(count), mod_(mod) {}

  void Apply(Character* actor, Character* target) override;
  std::string GetLogMessage(Character* actor, Character* target) const override;

 private:
  int sides_, count_, mod_;
  int last_heal_ = 0;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_EFFECT_H_
