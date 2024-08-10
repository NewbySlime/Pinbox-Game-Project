#include "godot_cpp/core/class_db.hpp"

#include "game_context_listener_helper.h"

using namespace Game;
using namespace GameUtils;
using namespace GameUtils::Helper;
using namespace godot;



ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(game_context_listener_helper)
ATTR_NODE_FREE_FILTER_DEFINE(game_context_listener_helper)
ATTR_BASE_INTERFACE_HELPER_DEFINE(game_context_listener_helper)
ATTR_INTERFACE_HELPER_DEFINE(game_context_listener_helper)


void game_context_listener_helper::_on_game_context_changing(gdvar_custom_object _param){
  game_handler_on_context_changed* _param_obj = GDVAR_CUSTOM_TO(_param, game_handler_on_context_changed);
  String _scene_path; PARSE_GAME_CONTEXT_SCENE_PATH(_scene_path, _param_obj->get_context_ID(), _parent_class_name)

  Array _err_paramarr;{
    _err_paramarr.append(_param_obj->get_context_ID());
    _err_paramarr.append(_parent_class_name);
    _err_paramarr.append(structure_holder_node::get_class_static());
  }

  // remove and free all child first
  queue_free_filter();

  ResourceLoader* _res_loader = ResourceLoader::get_singleton();
  Ref<PackedScene> _context_scene = _res_loader->load(_scene_path);
  Node* _context_node = NULL;
  subautoload_container* _sa_container = NULL;
  if(_context_scene.is_null()){
    INIT_ASSERT()
    WARN_ASSERT(String("Cannot get context scene. Context ID: '{0}' Class: '{1}'").format(_err_paramarr))
    goto safe_return;
  }

  _context_node = _context_scene->instantiate();
  if(!_context_node->is_class(structure_holder_node::get_class_static())){
    INIT_ASSERT()
    WARN_ASSERT(String("Context scene is not a type of '{3}'. Context ID: '{0}' Class: '{1}'").format(_err_paramarr))
    goto safe_return;
  }
  
  // get subautoload_container
  add_child_sa(_context_node);
  return;

  safe_return:{
    if(_context_node)
      _context_node->queue_free();
  }
}


void game_context_listener_helper::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(game_context_listener_helper)
  ATTR_NODE_FREE_FILTER_BIND_GODOT(game_context_listener_helper)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(game_context_listener_helper)
  ATTR_INTERFACE_HELPER_BIND_GODOT(game_context_listener_helper)

  ClassDB::bind_method(
    D_METHOD("_on_game_context_changing"),
    &game_context_listener_helper::_on_game_context_changing
  );
}


void game_context_listener_helper::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    subautoload_container* _sc_object = get_subautoload_root();
    GET_SUBAUTOLOAD(Game::game_scene_handler, _sc_object, _game_scene_handler)
    _game_scene_handler->connect(SIGNAL_GAME_HANDLER_ON_CONTEXT_CHANGING, Callable(this, "_on_game_context_changing"));
  }
}


void game_context_listener_helper::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_NODE_FREE_FILTER_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    
    INIT_ASSERT()

    Array _paramarr;{
      _paramarr.append(code);
    }

    LOG_ASSERT(String("notification: {0}").format(_paramarr))
    switch(code){
      break; case NOTIFICATION_PARENTED:{
        _parent_class_name = get_parent()->get_class();
      }

      break; case NOTIFICATION_UNPARENTED:{
        _parent_class_name = "null";
      }
    }
  }
}