#ifndef ENTITY_DATA_HEADER
#define ENTITY_DATA_HEADER

#include "custom_memory.h"
#include "defines.h"
#include "inventory_config.h"
#include "subdata_container.h"

#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Entity{
  class entity_database;
  class entity_data: public GameUtils::ParsingHelper::subdata_container, public GameUtils::Memory::custom_data{
    public:
      SUBDATA_OVERRIDE_NAME(Game::Item::inventory_config, inv_config_data, VARNAME_ENTITY_DATA_INVENTORY_CONFIG)
      SUBDATA_OVERRIDE_NAME(Game::Item::inventory_config, loadout_inv_config_data, VARNAME_ENTITY_DATA_LOADOUT_INVENTORY_CONFIG)

    private:
      friend entity_database;

      godot::String _entity_id;
      godot::String _entity_name;
      godot::String _entity_desc;
      godot::String _entity_type_class;

      void _parse_data_from(const godot::Dictionary& _data_part);

    
    public:
      ~entity_data(){}

      godot::String get_entity_id() const;
      godot::String get_entity_name() const;
      godot::String get_entity_description() const;
      godot::String get_entity_type_class() const;

      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);


      template<typename subdata_class> subdata_class* get_subdata_name(){
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<GameUtils::Memory::custom_memory> get_subdata_shared_mutable(){
        return __get_subdata_shared<subdata_class>();
      }

      template<typename subdata_class> const std::shared_ptr<GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }
  };
}

#endif