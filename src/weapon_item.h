#ifndef WEAPON_ITEM_HEADER
#define WEAPON_ITEM_HEADER

#include "godot_var_extended.h"
#include "item_object.h"

#include "custom_memory.h"
#include "hitreg_data.h"
#include "hit_register_data_container.h"
#include "item_container.h"
#include "weapon_stats_data.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"

#include "memory"


namespace Game::Item{
  namespace Factory{
    class weapon_factory;
  }

  class weapon_item: public GameUtils::Memory::custom_data{
    public:
      enum stats_type{
        stats_type_primary,
        stats_type_secondary
      };

    private:
      friend Game::Item::Factory::weapon_factory;

      // editable
      std::shared_ptr<GameUtils::Memory::custom_memory> _cd_primary_stats;
      Game::Item::weapon_stats_data* _primary_stats = NULL;
      
      // editable
      std::shared_ptr<GameUtils::Memory::custom_memory> _cd_secondary_stats;
      Game::Item::weapon_stats_data* _secondary_stats = NULL;

      static void copy_stats_to(const Game::Item::weapon_stats_data* _original_stats, std::shared_ptr<GameUtils::Memory::custom_memory>* _target_cd_stats, Game::Item::weapon_stats_data** _target_stats);

    public:
      ~weapon_item(){}

      static godot::String get_subdata_name();
      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

      Game::Item::weapon_stats_data* get_stats_mutable(stats_type type);
      const Game::Item::weapon_stats_data* get_stats(stats_type type) const;

      std::shared_ptr<GameUtils::Memory::custom_memory> get_stats_shared_mutable(stats_type type);
      std::shared_ptr<const GameUtils::Memory::custom_memory> get_stats_shared(stats_type type) const;
  };
}

#endif