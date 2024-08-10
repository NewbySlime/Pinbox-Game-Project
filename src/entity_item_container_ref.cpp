#include "entity_item_container_ref.h"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;



String entity_item_container_ref::get_subdata_name(){
  return "entity_item_container_ref";
}


void entity_item_container_ref::copy_subdata_to(custom_memory* mem){
  mem->change_data<entity_item_container_ref>();

  entity_item_container_ref* _new_item = mem->get_data<entity_item_container_ref>();{
    _new_item->_id_map = _id_map;
    _new_item->_usage_map = _usage_map;
  }
}


void entity_item_container_ref::add_item_container(String usage_id, item_container_inst_id inst_id){
  _usage_map[usage_id] = inst_id;
  _id_map[inst_id] = usage_id; 
}


item_container_inst_id entity_item_container_ref::get_item_container(String usage_id){
  auto _iter = _usage_map.find(usage_id);
  if(_iter == _usage_map.end())
    return ITEM_CONTAINER_ID_NULL;

  return _iter->second;
}


void entity_item_container_ref::remove_item_container_usage(String usage_id){
  auto _usage_iter = _usage_map.find(usage_id);
  if(_usage_iter == _usage_map.end())
    return;

  auto _inst_iter = _id_map.find(_usage_iter->second);
  if(_inst_iter != _id_map.end())
    _id_map.erase(_inst_iter);

  _usage_map.erase(_usage_iter);
}

void entity_item_container_ref::remove_item_container(item_container_inst_id inst_id){
  auto _inst_iter = _id_map.find(inst_id);
  if(_inst_iter == _id_map.end())
    return;

  auto _usage_iter = _usage_map.find(_inst_iter->second);
  if(_usage_iter != _usage_map.end())
    _usage_map.erase(_usage_iter);

  _id_map.erase(_inst_iter);
}