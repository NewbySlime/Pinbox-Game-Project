#ifndef MODIFIER_CONTAINER_ITEM_HEADER
#define MODIFIER_CONTAINER_ITEM_HEADER

#include "custom_memory.h"
#include "I_modifier.h"
#include "item_object.h"
#include "weapon_stats_data.h"

#include "modifier_rating_data.h"

#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  namespace Factory{
    class modifier_factory;
  }
  class modifier_container_item: public Game::Modifiers::I_modifier, public GameUtils::Memory::custom_data{
    private:
      friend Factory::modifier_factory;

      std::vector<std::shared_ptr<Game::Modifiers::I_modifier>> _container;

      godot::String _description;
      godot::String _title; 

      void _update_description();

    public:
      ~modifier_container_item(){}

      void modify_weapon_stats(Game::Item::weapon_stats_data* stats, const Game::hit_register_state* hitreg_state) const override;

      godot::String get_modifier_title() const override;
      godot::String get_modifier_description() const override;

      void add_modifier(std::shared_ptr<Game::Modifiers::I_modifier> modifier);
      void clear_modifiers();

      uint64_t get_modifier_count();
      std::shared_ptr<const Game::Modifiers::I_modifier> get_modifier_index(uint64_t index);

      //Game::modifier_rating_data get_rating_data();

      // subdata functions
      static godot::String get_subdata_name();
      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;
  };
}

#endif