#include "godot_macro.h"
#include "logger.h"
#include "player_actor.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Item;
using namespace godot;



void player_actor::_set_player_entity_object(entity_inst_id entity_id){
  _player_entity_id = entity_id;

  entity_actor::bind_to_entity(entity_id);
}


void player_actor::_bind_methods(){
  GD_PROPERTY_BIND_HINT(player_actor, Variant::STRING, action_overrider_scene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(player_actor, Variant::STRING, movement_overrider_scene, PropertyHint::PROPERTY_HINT_FILE)
}


void player_actor::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(entity_server, container, _entity_server)
  }
}


void player_actor::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ResourceLoader* _res_loader = ResourceLoader::get_singleton();
    {Ref<PackedScene> _pckscene = _res_loader->load(action_overrider_scene);
      if(_pckscene.is_null()){
        _paramarr.clear();{
          _paramarr.append(PackedScene::get_class_static());
        }

        ERR_ASSERT(String("action_overrider_scene is not a {0}.").format(_paramarr))
        throw ERR_UNCONFIGURED;
      }

      _add_overrider(USAGE_GENERIC_ENTITY_ACTION, _pckscene);
    }

    {Ref<PackedScene> _pckscene = _res_loader->load(movement_overrider_scene);
      if(_pckscene.is_null()){
        _paramarr.clear();{
          _paramarr.append(PackedScene::get_class_static());
        }

        ERR_ASSERT(String("movement_overrider_scene is not a {0}.").format(_paramarr))
        throw ERR_UNCONFIGURED;
      }

      _add_overrider(USAGE_GENERIC_ENTITY_MOVEMENT, _pckscene);
    }
  }
}


void player_actor::bind_to_entity(entity_inst_id entity_id){
  // maybe timeout for overriding an entity?

  entity_actor::bind_to_entity(entity_id);
}


void player_actor::spawn_player(player_spawner* spawner){
  entity_object* _player_entity;
  if(_player_entity_id == ENTITY_ID_NULL){
    _player_entity = _entity_server->create_entity(entity_type::et_player, "player_entity");
    _set_player_entity_object(_player_entity->get_instance_id()); 
  }

  // set player position
  Node2D* _player_pos = _player_entity->get_space2d_pivot_node();
  if(!_player_pos){
    INIT_ASSERT()
    ERR_ASSERT("Cannot get Space2D pivot Node from Player Entity.")
    return;
  }

  _player_pos->set_global_position(spawner->get_global_position());
}