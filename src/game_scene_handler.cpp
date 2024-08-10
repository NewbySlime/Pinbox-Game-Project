#include "game_scene_handler.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/os.hpp"


using namespace Game;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(game_scene_handler)
ATTR_BASE_INTERFACE_HELPER_DEFINE(game_scene_handler)
ATTR_INTERFACE_HELPER_DEFINE(game_scene_handler)


void game_scene_handler::_change_to_context(String contextID){
  game_handler_on_context_changed _signal_param;{
    _signal_param._context_ID = contextID;
  }

  emit_signal(SIGNAL_GAME_HANDLER_ON_CONTEXT_CHANGING, GDVAR_CUSTOM_FROM(_signal_param));

  emit_signal(SIGNAL_GAME_HANDLER_ON_CONTEXT_DONE_CHANGING);
}



void game_scene_handler::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(game_scene_handler)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(game_scene_handler)
  ATTR_INTERFACE_HELPER_BIND_GODOT(game_scene_handler)

  GD_PROPERTY_BIND(game_scene_handler, Variant::STRING, INIT_game_contextID)
  GD_PROPERTY_BIND(game_scene_handler, Variant::STRING, DEBUG_game_contextID)

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_ON_CONTEXT_CHANGING,
    PropertyInfo(Variant::INT, "parameter")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_ON_CONTEXT_DONE_CHANGING
  ));

  ClassDB::bind_method(
    D_METHOD("_on_subautoload_ready"),
    &game_scene_handler::_on_subautoload_ready
  );
}


void game_scene_handler::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    container->connect(SIGNAL_SUBAUTOLOAD_ON_READY, Callable(this, "_on_subautoload_ready"));
  }
}

void game_scene_handler::_on_subautoload_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    OS* _this_os = OS::get_singleton();
    if(_this_os->is_debug_build())
      _change_to_context(DEBUG_game_contextID);
    else
      _change_to_context(INIT_game_contextID);
  }
}


void game_scene_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}