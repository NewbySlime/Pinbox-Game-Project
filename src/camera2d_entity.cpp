#include "camera2d_entity.h"
#include "defines.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "vector"


using namespace Game;
using namespace Game::Entity;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(camera2d_entity)
ATTR_BASE_INTERFACE_HELPER_DEFINE(camera2d_entity)
ATTR_INTERFACE_HELPER_DEFINE(camera2d_entity)


void camera2d_entity::_on_node_follower_deleted(entity_inst_id inst_id){
  auto _iter_entity = _entity_follower_inst_map.find(inst_id);
  if(_iter_entity == _entity_follower_inst_map.end())
    return;

  uint64_t _object_inst_id = _iter_entity->second->get_instance_id();
  auto _iter_node = _node_inst_map.find(_object_inst_id);
  if(_iter_node != _node_inst_map.end())
    _node_inst_map.erase(_iter_node);

  _entity_follower_inst_map.erase(_iter_entity);
}

void camera2d_entity::_on_node_follower_position_update(gdvar_custom_object data){
  node2d_object_follower::signal_position_update* _signal_data = GDVAR_CUSTOM_TO(data, node2d_object_follower::signal_position_update);
  
  _target_follow = _signal_data->get_position();
}


void camera2d_entity::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(camera2d_entity)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(camera2d_entity)
  ATTR_INTERFACE_HELPER_BIND_GODOT(camera2d_entity)

  GD_PROPERTY_BIND_HINT(camera2d_entity, Variant::STRING, entity_follower_pckscene_path, PropertyHint::PROPERTY_HINT_FILE);

  ClassDB::bind_method(D_METHOD("_on_node_follower_deleted", "inst_id"), &camera2d_entity::_on_node_follower_deleted);
  ClassDB::bind_method(D_METHOD("_on_node_follower_position_updated", "data"), &camera2d_entity::_on_node_follower_position_update);
}


void camera2d_entity::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(entity_server, container, _entity_server)
  }
}

void camera2d_entity::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_LOAD_PACKED_SCENE_CHECKER(_entity_follower_pckscene, entity_follower_pckscene_path, node2d_object_follower)
  }
}


camera2d_entity::camera2d_entity(){
  _target_follow = Vector2(0,0);
  _last_follow_entity = ENTITY_ID_NULL;
}


void camera2d_entity::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      std::vector<Node*> _list_node;
      for(auto _pair: _entity_follower_inst_map)
        _list_node.insert(_list_node.end(), _pair.second);

      for(int i = 0; i < _list_node.size(); i++)
        _list_node[i]->queue_free();
    }
  }
}


void camera2d_entity::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_global_position(_target_follow);
  }
}


bool camera2d_entity::follow_entity(entity_inst_id id, bool force_smooth){
  auto _entity_follower_iter = _entity_follower_inst_map.find(id);
  node2d_object_follower* _last_follow_node = NULL;
  node2d_object_follower* _new_follow_node = NULL;
  
  auto _last_follower_iter = _entity_follower_inst_map.find(_last_follow_entity);
  if(_last_follower_iter != _entity_follower_inst_map.end())
    _last_follow_node = _last_follower_iter->second;

  // create if no follower exists
  if(_entity_follower_iter == _entity_follower_inst_map.end()){
    Node* _entity_node = _entity_server->get_entity(id);
    if(!_entity_node){
      INIT_ASSERT()
      WARN_ASSERT("Cannot get entity_object to follow.")

      return false;
    }

    _new_follow_node = (node2d_object_follower*)_entity_follower_pckscene->instantiate();
    get_subautoload_root()->add_child_node(_entity_node, _new_follow_node);
    
    _new_follow_node->connect(SIGNAL_NODE2D_OBJECT_FOLLOWER_DELETED, Callable(this, "_on_node_follower_deleted"));
    _new_follow_node->connect(SIGNAL_NODE2D_OBJECT_FOLLOWER_POSITION_UPDATE, Callable(this, "_on_node_follower_position_update"));

    uint64_t _node_inst_id = _new_follow_node->get_instance_id();
    _node_inst_map[_node_inst_id] = id;
    _entity_follower_inst_map[id] = _new_follow_node;
  }
  else
    _new_follow_node = _entity_follower_iter->second;

  _new_follow_node->set_disabled(false);
  if(_last_follow_entity)
    _last_follow_node->set_disabled(true);

  _last_follow_entity = id;

  _new_follow_node->set_position(Vector2(0,0));
  _target_follow = _new_follow_node->get_global_position();
  if(!force_smooth)
    set_global_position(_target_follow);

  return true;
}


bool camera2d_entity::set_player_entity_id(entity_inst_id entity_id){
  entity_object* _player_object = _entity_server->get_entity(entity_id);
  if(!_player_object){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(entity_id);
    }

    ERR_ASSERT(String("Player Entity (ID: {0}) cannot be found.").format(_paramarr))
    return false;
  }

  _player_entity_id = entity_id;
  return true;
}

void camera2d_entity::switch_to_player_entity(bool force_smooth){
  follow_entity(_player_entity_id, force_smooth);
}