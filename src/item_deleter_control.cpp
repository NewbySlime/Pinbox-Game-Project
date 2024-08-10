#include "defines.h"
#include "godot_macro.h"
#include "logger.h"
#include "id_types.h"

#include "item_deleter_control.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Item;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_deleter_control)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_deleter_control)
ATTR_INTERFACE_HELPER_DEFINE(item_deleter_control)


void item_deleter_control::_on_container_deleted(gdsignal_param_ref param){
  // TODO
}


void item_deleter_control::_on_button_released(){
  if(_deleter_container_id == ITEM_CONTAINER_ID_NULL)
    return;

  INIT_ASSERT()
  LOG_ASSERT("deleting items")
  _deleter_container->delete_items();
}


void item_deleter_control::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_deleter_control)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_deleter_control)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_deleter_control)

  GD_PROPERTY_BIND(item_deleter_control, Variant::INT, deleter_container_size)
  
  GD_PROPERTY_BIND(item_deleter_control, Variant::NODE_PATH, button_delete_node)
  GD_PROPERTY_BIND(item_deleter_control, Variant::NODE_PATH, item_container_control_node)

  ClassDB::bind_method(
    D_METHOD("_on_button_released"),
    &item_deleter_control::_on_button_released
  );
}


void item_deleter_control::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(item_server, sa_container, _item_server)
  }
}

void item_deleter_control::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_GET_NODE_TESTER(_item_container_control, item_container_control_node, item_container_control)
    CREATE_GET_NODE_TESTER(_delete_button, button_delete_node, Button)

    _delete_button->connect("button_up", Callable(this, "_on_button_released"));

    _deleter_container = (item_container_delete*)_item_server->create_item_container(deleter_container_size, ict_delete);
    if(_deleter_container){
      _deleter_container_id = _deleter_container->get_container_id();
      _item_container_control->set_reference(_deleter_container_id);
    }
  }
}


item_deleter_control::item_deleter_control(){
  _deleter_container = NULL;
  _deleter_container_id = ITEM_CONTAINER_ID_NULL;

  _item_server = NULL;
}


void item_deleter_control::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_item_server){
        if(_deleter_container_id != ITEM_CONTAINER_ID_NULL)
          _item_server->delete_item_container(_deleter_container_id);
      }
    }
  }
}