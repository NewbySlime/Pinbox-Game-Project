#include "damage_indicator.h"
#include "damage_indicator_handler.h"
#include "defines.h"
#include "logger.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/physics_direct_body_state2d.hpp"
#include "godot_cpp/classes/physics_server2d.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Effect;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(damage_indicator_handler)
ATTR_BASE_INTERFACE_HELPER_DEFINE(damage_indicator_handler)
ATTR_INTERFACE_HELPER_DEFINE(damage_indicator_handler)


void damage_indicator_handler::_on_damageable_damaged(Vector2 origin, double damage){
  if(_damage_indicator_object.ptr()){
    damage_indicator* _obj = (damage_indicator*)_damage_indicator_object->instantiate();

    _obj->set_global_position(origin);
    if(_root_node){
      get_subautoload_root()->add_child_node(_root_node, _obj);
    }
    else{
      add_child_sa(_obj);
    }

    _obj->start_indication(damage);
  }
}


void damage_indicator_handler::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(damage_indicator_handler)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(damage_indicator_handler)
  ATTR_INTERFACE_HELPER_BIND_GODOT(damage_indicator_handler)

  GD_PROPERTY_BIND_HINT(damage_indicator_handler, Variant::STRING, damage_indicator_object, PropertyHint::PROPERTY_HINT_FILE)

  ClassDB::bind_method(D_METHOD("_on_damageable_damaged"), &damage_indicator_handler::_on_damageable_damaged);
}


void damage_indicator_handler::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(bus_event, sa_container, _bus_event)
  }
}

void damage_indicator_handler::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ResourceLoader* _res_loader = ResourceLoader::get_singleton();
    CREATE_LOAD_PACKED_SCENE_CHECKER(_damage_indicator_object, damage_indicator_object, damage_indicator)

    _bus_event->connect(SIGNAL_BUS_EVENT_DAMAGEABLE_DAMAGED, Callable(this, "_on_damageable_damaged"));
  }
}


void damage_indicator_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void damage_indicator_handler::set_parent_target(Node* _node){
  _root_node = _node;
}