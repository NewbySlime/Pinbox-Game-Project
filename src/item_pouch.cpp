#include "item_pouch.h"


using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


item_pouch::~item_pouch(){
  // TODO delete to factory
}


String item_pouch::get_subdata_name(){
  return "item_pouch";
}


void item_pouch::copy_subdata_to(custom_memory* mem) const{
  // TODO copy by factory
}


I_item_container* item_pouch::get_pouch() const{
  return _container;
}

Node* item_pouch::get_pouch_node() const{
  return _container_node;
}