#include "cursor_follower_control.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/rendering_server.hpp"


using namespace Game;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(cursor_follower_control)
ATTR_BASE_INTERFACE_HELPER_DEFINE(cursor_follower_control)
ATTR_INTERFACE_HELPER_DEFINE(cursor_follower_control)


void cursor_follower_control::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(cursor_follower_control)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(cursor_follower_control)
  ATTR_INTERFACE_HELPER_BIND_GODOT(cursor_follower_control)
}


void cursor_follower_control::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_z_index(RenderingServer::CANVAS_ITEM_Z_MAX);
    set_texture_filter(TEXTURE_FILTER_NEAREST);

    _follower = new Node2D();
    add_child_sa(_follower);
  }
}


cursor_follower_control::cursor_follower_control(){
  _follower = NULL;
}


void cursor_follower_control::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)

  }
  
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_follower)
        delete _follower;
    }
  }
}


void cursor_follower_control::_unhandled_input(const Ref<InputEvent>& _event){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_event->is_class("InputEventMouseMotion"))
      _follower->set_position(get_local_mouse_position());
  }
}


void cursor_follower_control::add_child_node(Node* _node){
  get_subautoload_root()->add_child_node(_follower, _node);
}

void cursor_follower_control::remove_child_node(Node* _node){
  _follower->remove_child(_node);
}