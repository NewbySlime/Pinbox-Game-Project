#ifndef ITEM_POUCH_DATA_HEADER
#define ITEM_POUCH_DATA_HEADER

#include "custom_memory.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class item_pouch_data: public GameUtils::Memory::custom_data{
    private:
      uint64_t _pouch_size;
      uint32_t _container_type;
      uint32_t _item_filter;

      void _parse_item_pouch_data(godot::Dictionary* data);

    public:
      static godot::String get_subdata_name();

      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

      uint64_t get_pouch_size() const;
      uint32_t get_container_type() const;
      uint32_t get_item_filter() const;
  };
}

#endif