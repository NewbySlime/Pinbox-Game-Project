#include "algorithm_node.h"
#include "defines.h"
#include "item_frame_control.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace Game::Item;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_frame_control)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_frame_control)
ATTR_INTERFACE_HELPER_DEFINE(item_frame_control)


GD_PROPERTY_SETTER_FUNC(item_frame_control, double, margin_relative_size){
  if(var_margin_relative_size < 0)
    margin_relative_size = 0;
  else if(var_margin_relative_size > 0.5)
    margin_relative_size = 0.5;
  else
    margin_relative_size = var_margin_relative_size;

  _update_size();
}

GD_PROPERTY_GETTER_FUNC(item_frame_control, double, margin_relative_size){
  return margin_relative_size;
}


GD_PROPERTY_SETTER_FUNC(item_frame_control, int, min_size){
  min_size = var_min_size;
  _update_size();
}

GD_PROPERTY_GETTER_FUNC(item_frame_control, int, min_size){
  return min_size;
}


void item_frame_control::_update_size(){
  set_custom_minimum_size(Vector2(min_size, min_size));

  int _margin_size = min_size * margin_relative_size;
  add_theme_constant_override("margin_left", _margin_size);
  add_theme_constant_override("margin_right", _margin_size);
  add_theme_constant_override("margin_top", _margin_size);
  add_theme_constant_override("margin_bottom", _margin_size);
}


void item_frame_control::_on_mouse_entered(){
  emit_signal(SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_ENTERED, _bound_index);
}

void item_frame_control::_on_mouse_exited(){
  emit_signal(SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_EXITED);
}


void item_frame_control::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_frame_control)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_frame_control)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_frame_control)

  GD_PROPERTY_BIND(item_frame_control, Variant::FLOAT, margin_relative_size)
  GD_PROPERTY_BIND(item_frame_control, Variant::INT, min_size)

  GD_PROPERTY_BIND(item_frame_control, Variant::NODE_PATH, texture_rect_nodepath)
  GD_PROPERTY_BIND(item_frame_control, Variant::NODE_PATH, resource_instancer_nodepath)


  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_ENTERED,
    PropertyInfo(Variant::INT, "bound_index")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_FRAME_CONTROL_ON_MOUSE_EXITED
  ));


  ClassDB::bind_method(
    D_METHOD("_on_mouse_entered"),
    &item_frame_control::_on_mouse_entered
  );

  ClassDB::bind_method(
    D_METHOD("_on_mouse_exited"),
    &item_frame_control::_on_mouse_exited
  );
}


void item_frame_control::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  _update_size();

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    connect("mouse_entered", Callable(this, "_on_mouse_entered"));
    connect("mouse_exited", Callable(this, "_on_mouse_exited"));

    CREATE_GET_NODE_TESTER(_res_instancer, resource_instancer_nodepath, sprite_resource_instancer)
    CREATE_GET_NODE_TESTER(_image_renderer_node, texture_rect_nodepath, TextureRect)
  }
}


item_frame_control::item_frame_control(){
  margin_relative_size = 0;
  min_size = 0;
  _bound_index = 0;
}


void item_frame_control::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void item_frame_control::bind_frame_position(int index){
  _bound_index = index;
}

void item_frame_control::bind_item_object(const item_object* item){
  _bound_item = NULL;
  _bound_item = ITEM_ID_NULL;
  
  if(item){
    _bound_item = item;
    _bound_item_id = item->get_id();
  }

  _res_instancer->set_item_object(item);
  _image_renderer_node->set_texture(_res_instancer->as_viewport_texture());

  // the next line of code, for error checking
  if(!item)
    return;

  const item_resource_data* _res_data = item->get_resource_data();
  if(!_res_data){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(item->get_item_id());
    }

    ERR_ASSERT(String("Cannot get resource data for '{0}'").format(_paramarr))
    return;
  }
}


void item_frame_control::hide_item(){
  if(!_item_renderer)
    return;

  _item_renderer->set_modulate(Color(0,0,0,0));
}

void item_frame_control::show_item(){
  if(!_item_renderer)
    return;
    
  _item_renderer->set_modulate(Color(1,1,1,1));
}