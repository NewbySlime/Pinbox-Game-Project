#ifndef MODIFIER_COMBINATION_HELPER
#define MODIFIER_COMBINATION_HELPER

#include "modifier_container_item.h"
#include "hit_register_data_container.h"
#include "item_container.h"
#include "I_modifier.h"

#include "memory"
#include "vector"


namespace Game::Modifiers{
  class modifier_combination_helper: Game::Modifiers::I_modifier{
    private:
      std::vector<std::pair<
        const Game::Item::modifier_container_item*,
        std::shared_ptr<GameUtils::Memory::custom_memory>
      >> _combination_data;

    public:
      modifier_combination_helper(){}
      modifier_combination_helper(Game::Item::I_item_container* container);

      // if hitreg_state is NULL then the modifier will expect to apply passive modifiers
      void modify_weapon_stats(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state = NULL) const override;

  };
}

#endif