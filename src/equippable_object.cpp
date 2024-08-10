#include "logger.h"

#include "equippable_object.h"
#include "equippable_factory.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"


using namespace Game;
using namespace Game::Modifiers;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace godot;



ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(equippable_object)
ATTR_BASE_INTERFACE_HELPER_DEFINE(equippable_object)
ATTR_INTERFACE_HELPER_DEFINE(equippable_object)


void equippable_object::_set_item(const item_object* item){
  _current_item = item;
  this->_on_item_set(item);
}

bool equippable_object::_set_item_id(item_inst_id item_id){
  const item_object* _item = _item_server->get_item_object(item_id);
  if(!_item)
    return false;

  _set_item(_item);
  return true;
}


void equippable_object::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(equippable_object)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(equippable_object)
  ATTR_INTERFACE_HELPER_BIND_GODOT(equippable_object)

  ADD_SIGNAL(MethodInfo(
    SIGNAL_EQUIPPABLE_OBJECT_ON_DELETED
  ));
}


void equippable_object::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){

  }
}


equippable_object::equippable_object(){
  _set_angle = 0;

  _current_factory = NULL;
}


void equippable_object::_notifaction(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)

    switch(code){
      break; case NOTIFICATION_PREDELETE:{
        emit_signal(SIGNAL_EQUIPPABLE_OBJECT_ON_DELETED);
      }
    }
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_current_factory)
        _current_factory->_delete_object(this, true);
    }
  }
}


void equippable_object::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    rotate(Math::deg_to_rad(_set_angle));
  }
}


void equippable_object::set_object_rotation_deg(double angle){
  _set_angle = angle;
}

double equippable_object::get_object_rotation(){
  return _set_angle;
}


const item_object* equippable_object::get_item_object(){
  return _current_item;
}