#include "modifier_container_item.h"

#include "item_server.h"


using namespace Game;
using namespace Game::Item;
using namespace Game::Modifiers;
using namespace GameUtils::Memory;
using namespace godot;


void modifier_container_item::_update_description(){
  _description = "";
  for(auto _var: _container){
    _description += _var->get_modifier_description() + "\n";
  }
}


void modifier_container_item::modify_weapon_stats(weapon_stats_data* stats, const hit_register_state* hitreg_state) const{
  for(auto _var: _container)
    _var->modify_weapon_stats(stats, hitreg_state);
}


String modifier_container_item::get_modifier_title() const{
  return _title;
}

String modifier_container_item::get_modifier_description() const{
  return _description;
}


void modifier_container_item::add_modifier(std::shared_ptr<I_modifier> modifier){
  _container.insert(_container.end(), modifier);
  _update_description();
}

void modifier_container_item::clear_modifiers(){
  _container.clear();
}


uint64_t modifier_container_item::get_modifier_count(){
  return _container.size();
}

std::shared_ptr<const I_modifier> modifier_container_item::get_modifier_index(uint64_t index){
  if(index >= _container.size())
    return std::shared_ptr<const I_modifier>();

  return _container[index];
}

/*
modifier_rating_data modifier_container_item::get_rating_data(){
  return modifier_rating_data();
}
*/


String modifier_container_item::get_subdata_name(){
  return "modifier_container_item";
}

void modifier_container_item::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<modifier_container_item>();
  modifier_container_item* _mem_data = mem->get_data<modifier_container_item>();

  _mem_data->_container = _container;
  _mem_data->_title = _title;
  _mem_data->_description = _description;
}