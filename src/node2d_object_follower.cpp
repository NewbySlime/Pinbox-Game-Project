#include "godot_cpp/classes/engine.hpp"

#include "godot_var_extended.h"
#include "node2d_object_follower.h"


using namespace Game;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(node2d_object_follower)
ATTR_BASE_INTERFACE_HELPER_DEFINE(node2d_object_follower)
ATTR_INTERFACE_HELPER_DEFINE(node2d_object_follower)


GD_PROPERTY_GETTER_FUNC(node2d_object_follower, bool, disabled){
  return disabled;
}

GD_PROPERTY_SETTER_FUNC(node2d_object_follower, bool, disabled){
  disabled = var_disabled;

  if(disabled){
    set_process(false);
    set_physics_process(false);
  }
  else
    set_use_physics_process(use_physics_process);
}


GD_PROPERTY_GETTER_FUNC(node2d_object_follower, bool, use_physics_process){
  return use_physics_process;
}

GD_PROPERTY_SETTER_FUNC(node2d_object_follower, bool, use_physics_process){
  use_physics_process = var_use_physics_process;

  set_process(!use_physics_process);
  set_physics_process(use_physics_process);
}



void node2d_object_follower::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(node2d_object_follower)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(node2d_object_follower)
  ATTR_INTERFACE_HELPER_BIND_GODOT(node2d_object_follower)

  GD_PROPERTY_BIND(node2d_object_follower, Variant::BOOL, use_physics_process)
  
  ADD_SIGNAL(MethodInfo(
    SIGNAL_NODE2D_OBJECT_FOLLOWER_POSITION_UPDATE,
    PropertyInfo(GDVAR_CUSTOM_VARIANT_TYPE, "update_data")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_NODE2D_OBJECT_FOLLOWER_DELETED,
    PropertyInfo(Variant::INT, "node_inst_id")
  ));
}


void node2d_object_follower::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_disabled(disabled);
  }
}

void node2d_object_follower::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)

    switch(code){
      break; case NOTIFICATION_PREDELETE:{
        emit_signal(SIGNAL_NODE2D_OBJECT_FOLLOWER_DELETED, get_instance_id());
      }
    }
  }
}


void node2d_object_follower::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    signal_position_update _signal_data;{
      _signal_data._position = get_global_position();
    }

    emit_signal(SIGNAL_NODE2D_OBJECT_FOLLOWER_POSITION_UPDATE, GDVAR_CUSTOM_FROM(_signal_data));
  }
}

void node2d_object_follower::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    signal_position_update _signal_data;{
      _signal_data._position = get_global_position();
    }

    emit_signal(SIGNAL_NODE2D_OBJECT_FOLLOWER_POSITION_UPDATE, GDVAR_CUSTOM_FROM(_signal_data));
  }
}