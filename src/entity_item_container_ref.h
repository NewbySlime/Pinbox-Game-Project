#ifndef ENTITY_ITEM_CONTAINER_REF_HEADER
#define ENTITY_ITEM_CONTAINER_REF_HEADER

#include "custom_memory.h"
#include "id_types.h"

#include "godot_cpp/variant/string.hpp"

#include "map"


namespace Game::Entity{
  class entity_item_container_ref: public GameUtils::Memory::custom_data{

    private:
      std::map<godot::String, Game::Item::item_container_inst_id> _usage_map;
      std::map<Game::Item::item_container_inst_id, godot::String> _id_map;

    public:
      static godot::String get_subdata_name();

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);

      void add_item_container(godot::String usage_id, Game::Item::item_container_inst_id inst_id);
      
      Game::Item::item_container_inst_id get_item_container(godot::String usage_id);

      void remove_item_container_usage(godot::String usage_id);
      void remove_item_container(Game::Item::item_container_inst_id inst_id);

  };
}

#endif