#include "hit_register_handler.h"
#include "hit_registers.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/variant/variant.hpp"


using namespace Game;
using namespace Game::HitRegisters;
using namespace GameUtils::Memory;
using namespace godot;
using namespace std;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(hit_register_handler)
ATTR_BASE_INTERFACE_HELPER_DEFINE(hit_register_handler)
ATTR_INTERFACE_HELPER_DEFINE(hit_register_handler)


void hit_register_handler::_load_hr_nodes(){
  INIT_ASSERT()
  Array _paramarr;

  Array _keys = hit_reg_node_packed_scene.keys();
  for(int i = 0; i < _keys.size(); i++){
    String _key;
    uint32_t _key_int;
    Variant _res_var;
    Object* _res_obj;
    PackedScene* _resource;
    Node* _test_node = NULL;

    _key = _keys[i];
    if(!_key.is_valid_int()){
      _paramarr.clear();{
        _paramarr.append(_key);
      }

      ERR_ASSERT(String("Key {0} is not an integer.").format(_paramarr))
      goto skip_label;
    }
    _key_int = _key.to_int();

    _res_var = hit_reg_node_packed_scene[_key];
    if(_res_var.get_type() != Variant::OBJECT){
      _paramarr.clear();{
        _paramarr.append(_key);
      }

      ERR_ASSERT(String("Key {0} is not an Object").format(_paramarr))
      goto skip_label;
    }

    _res_obj = _res_var;
    if(!_res_obj->is_class("PackedScene")){
      _paramarr.clear();{
        _paramarr.append(_key);
      }

      ERR_ASSERT(String("Key {0} is not a PackedScene.").format(_paramarr))
      goto skip_label;
    }

    _resource = (PackedScene*)_res_obj;


#define __HRH_CHECK_NODE(node, target_node)\
  if(!node->is_class(#target_node)){\
    ERR_ASSERT("Resource loaded is not a " #target_node)\
    goto skip_label;\
  }\
  target_node* _test_casted_node = (target_node*)node;
// end of #define __HRH_CHECK_NODE

    _test_node = _resource->instantiate();
    switch(_key_int){
      break; case hrte_raycast:{
        __HRH_CHECK_NODE(_test_node, hr_raycast)
      }

      break; default:{
        _paramarr.clear();{
          _paramarr.append(_key);
        }

        ERR_ASSERT(String("Key {0} is not a valid hit_register_type_enum.").format(_paramarr))
        goto skip_label;
      }
    }

    _hr_packed_scenes[_key_int] = _resource;

    skip_label:{
      if(_test_node)
        _test_node->queue_free();
    }
  }
}

PackedScene* hit_register_handler::_get_hr_node(hit_register_type_enum hrte_type){
  auto _iter = _hr_packed_scenes.find((uint32_t)hrte_type);
  if(_iter == _hr_packed_scenes.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append((uint32_t)hrte_type);
    }

    ERR_ASSERT(String("Cannot find hitreg for key {0}.").format(_paramarr))
    return NULL;
  }

  return _iter->second;
}


void hit_register_handler::_cast_hit_reg(
  std::shared_ptr<Game::hit_register_data_container>& reg_data, 
  Game::hit_register_data* _hitreg_data,
  std::shared_ptr<Game::I_damager>& damager,
  hit_register_start_data* start_data
){
  hit_register* _hitreg;
  Node* _hr_node = NULL;
  PackedScene* _hr_node_pck = _get_hr_node(_hitreg_data->hitreg_type);
  if(!_hr_node_pck)
    goto on_error;

  _hr_node = _hr_node_pck->instantiate();
  switch(_hitreg_data->hitreg_type){
    break; case hrte_raycast:{
      hr_raycast *_raycast = (hr_raycast*)_hr_node;
      _hitreg = (hit_register*)_raycast;
    }

    break; default:
      goto on_error;
  }

  _hitreg->_this_node = _hr_node;

  _hitreg->_metadata.damager = damager;
  _hitreg->_metadata.data = reg_data;
  _hitreg->_metadata.current_hitreg_data = _hitreg_data;
  _hitreg->_metadata.handler = this;
  _hitreg->_start_data = *start_data;

  if(_root_node){
    get_subautoload_root()->add_child_node(_root_node, _hr_node);
  }
  else{
    add_child_sa(_hr_node);
  }

  _hitreg->start_casting();

  goto skip_error;
  on_error:{
    if(_hr_node)
      _hr_node->queue_free();
  }

  skip_error:{}
}


void hit_register_handler::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _load_hr_nodes();
  }
}


void hit_register_handler::_bind_methods(){  
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(hit_register_handler)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(hit_register_handler)
  ATTR_INTERFACE_HELPER_BIND_GODOT(hit_register_handler)
  
  GD_PROPERTY_BIND(hit_register_handler, Variant::DICTIONARY, hit_reg_node_packed_scene)
}


hit_register_handler::hit_register_handler(){
  _root_node = NULL;
}


void hit_register_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void hit_register_handler::_physics_process(double delta){
  
}


void hit_register_handler::cast_hit_reg(std::shared_ptr<hit_register_data_container>& reg_data, std::shared_ptr<Game::I_damager>& damager, hit_register_start_data* start_data){
  hit_register_data* _hitreg_data = reg_data->get_head();
  this->get_class();
  _cast_hit_reg(
    reg_data,
    _hitreg_data,
    damager,
    start_data
  );
}

void hit_register_handler::next_hitreg_part(hit_register* hitreg, hit_register_start_data* start_data){
  hitreg_metadata* metadata = &hitreg->_metadata;
  hit_register_data* _last_hitreg = metadata->current_hitreg_data;

  for(int i = 0; i < _last_hitreg->_next.size(); i++){
    hit_register_data* _new_hitreg = _last_hitreg->_next[i];
    std::shared_ptr<I_damager> _new_damager = metadata->damager->clone_damager();

    hit_register_state _new_state;{
      _new_state.hitreg_type = _new_hitreg->hitreg_type;
    }
    
    _new_damager->on_hitreg_changed(&_new_state);
    _cast_hit_reg(
      metadata->data,
      _new_hitreg,
      metadata->damager,
      start_data
    );
  }

  stop_hitreg(hitreg);
}

void hit_register_handler::stop_hitreg(hit_register* hitreg){
  hitreg->_this_node->queue_free();
}


void hit_register_handler::set_root(Node2D* node){
  _root_node = node;
}



void hit_register::on_hitreg_finished(){
  _metadata.handler->next_hitreg_part(this, &_start_data);
}

void hit_register::on_hitreg_interrupted(){
  _metadata.handler->stop_hitreg(this);
}