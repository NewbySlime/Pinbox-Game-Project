#include "defines.h"
#include "logger.h"
#include "ui_container.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::UI;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(ui_container)
ATTR_BASE_INTERFACE_HELPER_DEFINE(ui_container)
ATTR_INTERFACE_HELPER_DEFINE(ui_container)


void ui_container::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(ui_container)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(ui_container)
  ATTR_INTERFACE_HELPER_BIND_GODOT(ui_container)

  GD_PROPERTY_BIND(ui_container, Variant::NODE_PATH, left_controller_path);
  GD_PROPERTY_BIND(ui_container, Variant::NODE_PATH, right_controller_path);
}


void ui_container::_bind_subautoload(subautoload_container* container){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_GET_NODE_TESTER(_left_controller, left_controller_path, Control)
    CREATE_GET_NODE_TESTER(_right_controller, right_controller_path, Control)

    CREATE_GET_NODE_TESTER(_left_subviewport, left_subviewport_path, SubViewport)
    CREATE_GET_NODE_TESTER(_right_subviewport, right_subviewport_path, SubViewport)
  }
}


void ui_container::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


Ref<ViewportTexture> ui_container::get_left_viewport_tex(){
  return _left_subviewport->get_texture();
}

Ref<ViewportTexture> ui_container::get_right_viewport_tex(){
  return _right_subviewport->get_texture();
}