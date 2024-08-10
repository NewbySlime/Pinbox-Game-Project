#include "static_level.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/utility_functions.hpp"


using namespace Game;
using namespace Game::Level;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(static_level)
ATTR_BASE_INTERFACE_HELPER_DEFINE(static_level)
ATTR_INTERFACE_HELPER_DEFINE(static_level)
ATTR_PARENT_CHILD_WATCHER_HELPER_DEFINE(static_level)


void static_level::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(static_level)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(static_level)
  ATTR_INTERFACE_HELPER_BIND_GODOT(static_level)
  ATTR_PARENT_CHILD_WATCHER_HELPER_BIND_GODOT(static_level)

  GD_PROPERTY_BIND(static_level, Variant::NODE_PATH, custom_player_spawner_nodepath)
}


void static_level::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(game_handler, container, _game_handler)
  }
}

void static_level::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(custom_player_spawner_nodepath.is_empty()){
      _player_spawner = NULL;
      bool _spawner_dupe_found = false;

      LOG_ASSERT("Finding player spawner...")
      for(int i = 0; i < get_child_count(); i++){
        Node* _child = get_child(i);
        if(_child->is_class(player_spawner::get_class_static())){
          if(!_player_spawner){
            _spawner_dupe_found = true;
          }
          else{
            LOG_ASSERT("Player spawner found!")

            _player_spawner = (player_spawner*)_child;
          }
        }
      }

      if(!_player_spawner){
        ERR_ASSERT("Player's spawner cannot be found, player cannot be spawned.")
      }
      else if(_spawner_dupe_found){
        WARN_ASSERT("Found spawner more than one, the ideal should be only one.")
        WARN_ASSERT("Spawner dupes will not be used.")
      }
    }
    else{
      CREATE_GET_NODE_TESTER(_player_spawner, custom_player_spawner_nodepath, player_spawner)
      if(!_player_spawner){
        ERR_ASSERT("Failed to fetch player spawner with fixed NodePath.")
        ERR_ASSERT("Player cannot be spawned.")
      }
    }

    __child_watcher_change_parent(this);
  }
}


void static_level::_on_child_entered(Node* child){
  // skip if child is a player spawner
  if(child->is_class(player_spawner::get_class_static()))
    return;

  I_spawner* _spawner = NULL; INTERFACE_GET(child, I_spawner, _spawner)
  if(!_spawner)
    return;

  _spawner_metadata _metadata;{
    _metadata._spawner_node = child;
    _metadata._spawner_interface = _spawner;
  }

  _world_entity_spawner_map[child->get_instance_id()] = _metadata;
}

void static_level::_on_child_exited(Node* child){
  // skip if child is a player spawner
  if(child->is_class(player_spawner::get_class_static()))
    return;

  
}


void static_level::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_PARENT_CHILD_WATCHER_HELPER_ON_NOTIFICATION(code)
  }
}


void static_level::initiate_level(){

}

void static_level::clear_level(){

}


void static_level::game_start(){
  if(_player_spawner)
    _player_spawner->spawn_entity();
}