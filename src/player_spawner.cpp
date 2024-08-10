#include "algorithm_node.h"
#include "player_actor.h"
#include "player_spawner.h"

#include "godot_cpp/classes/engine.hpp"



using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(player_spawner)
ATTR_BASE_INTERFACE_HELPER_DEFINE(player_spawner)
ATTR_INTERFACE_HELPER_DEFINE(player_spawner)


void player_spawner::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(player_spawner)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(player_spawner)
}


void player_spawner::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(game_handler, get_subautoload_root(), _game_handler)
  }
}

void player_spawner::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _player_actor = find_child_node_type<player_actor>(_game_handler);
    if(!_player_actor){
      INIT_ASSERT()
      Array _paramarr;{
        _paramarr.append(player_actor::get_class_static());
      }

      WARN_ASSERT(String("Cannot get '{0}', spawner will be useless.").format(_paramarr))
    }
  }
}


void player_spawner::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void player_spawner::spawn_entity(){
  if(!_player_actor){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(player_actor::get_class_static());
    }

    WARN_ASSERT(String("Cannot spawn Player Object. '{0}' is missing at start.").format(_paramarr))
    return;
  }

  _player_actor->spawn_player(this);
}


Vector2 player_spawner::get_spawn_position(){
  return get_position();
}