#ifndef ITEM_POUCH_HEADER
#define ITEM_POUCH_HEADER

#include "custom_memory.h"
#include "I_item_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  namespace Factory{
    class item_pouch_factory;
  }

  // this is different than the item_pouch_data, this is a game object representation of the object.
  class item_pouch: public GameUtils::Memory::custom_data{
    private:
      friend Game::Item::Factory::item_pouch_factory;
      Game::Item::I_item_container* _container = NULL;
      godot::Node* _container_node = NULL;
 
    public:
      ~item_pouch();

      static godot::String get_subdata_name();

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

      Game::Item::I_item_container* get_pouch() const;
      godot::Node* get_pouch_node() const;
  };
}

#endif