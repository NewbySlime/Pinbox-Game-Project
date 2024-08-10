#ifndef INVENTORY_CONFIG_HEADER
#define INVENTORY_CONFIG_HEADER

#include "custom_memory.h"

#include "godot_cpp/variant/dictionary.hpp" 
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class inventory_config: public GameUtils::Memory::custom_data{
    public:
      enum inv_type{
        _wrong_type = -1,
      inv_type_normal,
      inv_type_loadout
      };
    
    private:
      inv_type _inventory_type;
      uint32_t _inventory_size;


      inv_type _parse_inventory_type(const godot::String& value);
      void _parse_data(const godot::Dictionary& _data_part);
    

    public:
      ~inventory_config(){}

      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);


      inv_type get_inventory_type() const;
      uint32_t get_inventory_size() const;
  };
}

#endif