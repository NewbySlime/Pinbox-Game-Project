#include "algorithm_node.h"
#include "defines.h"
#include "game_handler.h"
#include "session_data_server.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/os.hpp"


using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace GameUtils;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(game_handler)
ATTR_BASE_INTERFACE_HELPER_DEFINE(game_handler)
ATTR_INTERFACE_HELPER_DEFINE(game_handler)


void game_handler::_game_scene_changing(){
  
}

void game_handler::_game_scene_done_changing(gdvar_custom_object object){
  game_handler_on_context_changed* _param = GDVAR_CUSTOM_TO(object, game_handler_on_context_changed);

  emit_signal(SIGNAL_GAME_HANDLER_INITIALIZE_GAME_OBJECT);

  emit_signal(SIGNAL_GAME_HANDLER_GAME_STARTING);
}


void game_handler::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(game_handler);
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(game_handler)
  ATTR_INTERFACE_HELPER_BIND_GODOT(game_handler)

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_GAME_STARTING
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_RESUMING
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_PAUSING
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_HANDLER_INITIALIZE_GAME_OBJECT
  ));

  ClassDB::bind_method(
    D_METHOD("_game_scene_changing"),
    &game_handler::_game_scene_changing
  );

  ClassDB::bind_method(
    D_METHOD("_game_scene_done_changing", "object"),
    &game_handler::_game_scene_done_changing
  );
}


void game_handler::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(game_scene_handler, container, _game_scene_handler)
    _game_scene_handler->connect(SIGNAL_GAME_HANDLER_ON_CONTEXT_CHANGING, Callable(this, "_game_scene_changing"));
    _game_scene_handler->connect(SIGNAL_GAME_HANDLER_ON_CONTEXT_DONE_CHANGING, Callable(this, "_game_scene_done_changing"));
  }
}

void game_handler::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    // get game_context_listener_helper
    _context_helper = find_child_node_type<game_context_listener_helper>(this);
    if(!_context_helper){
      INIT_ASSERT()
      Array _paramarr;{
        _paramarr.append(game_context_listener_helper::get_class_static());
      }

      ERR_ASSERT(String("Cannot find {0} on children.").format(_paramarr))
      throw ERR_UNCONFIGURED;
    }


    // get session_data_server
    session_data_server* _runtime_data = find_child_node_type<session_data_server>(this);
    if(!_runtime_data){
      INIT_ASSERT()
      Array _paramarr;{
        _paramarr.append(session_data_server::get_class_static());
      }

      ERR_ASSERT(String("Cannot find {0} on children.").format(_paramarr))
      throw ERR_UNCONFIGURED;
    }

    _context_helper->add_node_free_filter(_runtime_data);
  }
}


void game_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}