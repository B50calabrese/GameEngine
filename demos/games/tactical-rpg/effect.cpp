#include "effect.h"
#include <random>
#include <algorithm>

namespace tactical_rpg {

void DamageEffect::Apply(Character* actor, Character* target) {
    std::mt19937 gen(std::random_device{}());
    int damage = 0;
    for(int i=0; i < count_; ++i) {
        damage += std::uniform_int_distribution<int>(1, sides_)(gen);
    }
    damage += mod_;

    target->stats.current_hp -= damage;
    if (target->stats.current_hp <= 0) {
        target->is_downed = true;
        target->stats.current_hp = 0;
    }
    last_damage_ = damage;
}

std::string DamageEffect::GetLogMessage(Character* actor, Character* target) const {
    return actor->name + " hit " + target->name + " for " + std::to_string(last_damage_);
}

void HealEffect::Apply(Character* actor, Character* target) {
    std::mt19937 gen(std::random_device{}());
    int heal = 0;
    if (sides_ > 0) {
        for(int i=0; i < count_; ++i) {
            heal += std::uniform_int_distribution<int>(1, sides_)(gen);
        }
    }
    heal += mod_;

    target->stats.current_hp = std::min(target->stats.max_hp, target->stats.current_hp + heal);
    last_heal_ = heal;
}

std::string HealEffect::GetLogMessage(Character* actor, Character* target) const {
    return actor->name + " healed " + target->name + " for " + std::to_string(last_heal_);
}

} // namespace tactical_rpg
