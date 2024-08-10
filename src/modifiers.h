#ifndef MODIFIERS_HEADER
#define MODIFIERS_HEADER

#include "I_modifier.h"
#include "I_modifier_database_helper.h"

#include "godot_cpp/classes/random_number_generator.hpp"


namespace Game::Item::Factory{
  class modifier_factory;
}

namespace Game::Modifiers{
  class damage_normal_additive_modifier: Game::Modifiers::I_modifier, Game::Item::I_modifier_database_helper{
    private:
      double _addition = 0;

    public:
      ~damage_normal_additive_modifier(){}

      void modify_weapon_stats(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state = NULL) const override;
       
      void randomize_modifier(godot::RandomNumberGenerator* rng, std::shared_ptr<GameUtils::Memory::custom_memory> modifier_data) override;

      godot::String get_modifier_title() const override;
      godot::String get_modifier_description() const override;

      MODDB_HELPER_ADD_STATIC_ID_GETTER("damage_normal_additive")

      MODDB_HELPER_ADD_STATIC_JSON_PARSER_DECLARE()
  };

  class damage_percentage_additive_modifier: Game::Modifiers::I_modifier, Game::Item::I_modifier_database_helper{
    private:
      double _percentage = 0;

    public:
      ~damage_percentage_additive_modifier(){}

      void modify_weapon_stats(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state = NULL) const override;
       
      void randomize_modifier(godot::RandomNumberGenerator* rng, std::shared_ptr<GameUtils::Memory::custom_memory> modifier_data) override;

      godot::String get_modifier_title() const override;
      godot::String get_modifier_description() const override;

      MODDB_HELPER_ADD_STATIC_ID_GETTER("damage_percentage_additive")

      MODDB_HELPER_ADD_STATIC_JSON_PARSER_DECLARE()
  };

  class weapon_inventory_modifier: Game::Modifiers::I_modifier, Game::Item::I_modifier_database_helper{

    public:
      uint32_t get_container_size();
  };

  class item_accelerator_modifier: public weapon_inventory_modifier{

  };
}

#endif