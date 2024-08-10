#include "algorithm_identification.h"
#include "defines.h"
#include "id_types.h"
#include "item_dropper.h"
#include "item_object.h"
#include "item_picker_area.h"
#include "item_server.h"
#include "item_world_entity_2d.h"
#include "item_world_entity_2d_handler.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/resource_loader.hpp"


using namespace Game;
using namespace Game::Item;
using namespace Game::Misc::Algorithm;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_world_entity_2d_handler)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_world_entity_2d_handler)
ATTR_INTERFACE_HELPER_DEFINE(item_world_entity_2d_handler)


item_dropper_inst_id item_world_entity_2d_handler::_get_new_dropper_id(){
  return get_new_id_from_map(_item_dropper_map, ITEM_DROPPER_ID_NULL);
}

item_picker_inst_id item_world_entity_2d_handler::_get_new_picker_id(){
  return get_new_id_from_map(_item_picker_area_map, ITEM_PICKER_ID_NULL);
}


void item_world_entity_2d_handler::_on_item_removed(gdsignal_param_ref item_id){
  item_server_item_removed_param* _item_id = SIGNAL_PARAM_GET_CUSTOMPTR(item_id, item_server_item_removed_param);
  _delete_iwe2d(_item_id->get_id());
}


void item_world_entity_2d_handler::_on_item_container_deleted(gdsignal_param_ref container_id){
  item_server_item_container_removed_param* _container_id = SIGNAL_PARAM_GET_CUSTOMPTR(container_id, item_server_item_container_removed_param);
  
  // dropper part
  auto _iter_bound_dropper = _bound_dropper_containers_map.find(_container_id->get_container_id());
  if(_iter_bound_dropper != _bound_dropper_containers_map.end()){
    _item_dropper_metadata* _metadata = _iter_bound_dropper->second;
    _metadata->_bound_containers_id.erase(_container_id->get_container_id());

    _bound_dropper_containers_map.erase(_iter_bound_dropper);
  }

  // picker part
  auto _iter_bound_picker = _bound_picker_containers_map.find(_container_id->get_container_id());
  if(_iter_bound_picker != _bound_picker_containers_map.end()){
    _item_picker_area_metadata* _metadata = _iter_bound_picker->second;
    _metadata->_node->unbind_item_container();

    _bound_picker_containers_map.erase(_iter_bound_picker);
  }
}


void item_world_entity_2d_handler::_bind_methods(){  
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_world_entity_2d_handler)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_world_entity_2d_handler)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_world_entity_2d_handler)

  GD_PROPERTY_BIND(item_world_entity_2d_handler, Variant::FLOAT, random_speed)
  GD_PROPERTY_BIND(item_world_entity_2d_handler, Variant::FLOAT, maximum_throw_speed)

  GD_PROPERTY_BIND_HINT(item_world_entity_2d_handler, Variant::STRING, item_dropper_pckscene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(item_world_entity_2d_handler, Variant::STRING, item_picker_area_pckscene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(item_world_entity_2d_handler, Variant::STRING, item_world_entity_pckscene, PropertyHint::PROPERTY_HINT_FILE)

  ClassDB::bind_method(
    D_METHOD("_on_item_removed",
      "item_id"
    ),

    &item_world_entity_2d_handler::_on_item_removed
  );

  ClassDB::bind_method(
    D_METHOD("_on_item_container_deleted",
      "container_id"
    ),

    &item_world_entity_2d_handler::_on_item_container_deleted
  );
}


void item_world_entity_2d_handler::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(rng_server, sa_container, _rng_server)
    GET_SUBAUTOLOAD(game_handler, sa_container, _game_handler)
  }
}

void item_world_entity_2d_handler::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_dropper_pckscene, item_dropper_pckscene, item_dropper)
    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_picker_area_pckscene, item_picker_area_pckscene, item_picker_area)
    CREATE_LOAD_PACKED_SCENE_CHECKER(_item_world_entity_pckscene, item_world_entity_pckscene, item_world_entity_2d)

    _item_server->create_item_container(0, ict_infinite, &_drop_container_id);
    _item_server->connect(SIGNAL_ITEM_SERVER_ITEM_REMOVED, Callable(this, "_on_item_removed"));
    _item_server->connect(SIGNAL_ITEM_SERVER_ITEM_CONTAINER_REMOVING, Callable(this, "_on_item_container_deleted"));
  }
}


bool item_world_entity_2d_handler::_delete_dropper(item_dropper_inst_id id, bool inst_already_deleted){
  auto _iter_dropper = _item_dropper_map.find(id);
  if(_iter_dropper == _item_dropper_map.end())
    return false;

  _item_dropper_metadata* _metadata = _iter_dropper->second;
  while(_metadata->_bound_containers_id.size() > 0){
    auto _current_iter = _metadata->_bound_containers_id.begin();
    unbind_container_dropper(*_current_iter, id);
  }

  _item_dropper_map.erase(_iter_dropper);

  if(!inst_already_deleted)
    _metadata->_node->queue_free();

  delete _metadata;

  iwe2d_handler_dropper_deleted_param _param;{
    _param._id = id;
  }
  emit_signal(
    SIGNAL_IWE2DH_ON_DROPPER_DELETED,
    SIGNAL_PARAM_TO_CUSTOM(_param)
  );

  return true;
}

bool item_world_entity_2d_handler::_delete_iwe2d(iwe2d_inst_id id, bool inst_already_deleted){
  auto _iter = _dropped_item_map.find(id);
  if(_iter == _dropped_item_map.end())
    return false;

  _item_world_entity_metadata* _metadata = _iter->second;
  _dropped_item_map.erase(_iter);

  if(!inst_already_deleted)
    _metadata->_node->queue_free();

  delete _metadata;

  iwe2d_handler_entity_deleted_param _param;{
    _param._id = id;
  }
  emit_signal(
    SIGNAL_IWE2DH_ON_PICKER_DELETED,
    SIGNAL_PARAM_TO_CUSTOM(_param)
  );

  return true;
}

bool item_world_entity_2d_handler::_delete_picker_area(item_picker_inst_id id, bool inst_already_deleted){
  auto _iter_picker = _item_picker_area_map.find(id);
  if(_iter_picker == _item_picker_area_map.end())
    return false;

  _item_picker_area_metadata* _metadata = _iter_picker->second;
  unbind_container_picker(_metadata->_bound_container_id, id);

  _item_picker_area_map.erase(_iter_picker);

  if(!inst_already_deleted)
    _metadata->_node->queue_free();

  delete _metadata;

  iwe2d_handler_picker_deleted_param _param;{
    _param._id = id;
  }
  emit_signal(
    SIGNAL_IWE2DH_ON_PICKER_DELETED,
    SIGNAL_PARAM_TO_CUSTOM(_param)
  );

  return true;
}


item_world_entity_2d_handler::item_world_entity_2d_handler(){
  _root_drop = NULL;
}


void item_world_entity_2d_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      while(_item_dropper_map.size() > 0){
        auto _iter = _item_dropper_map.begin();
        delete_item_dropper(_iter->first);
      }

      while(_item_picker_area_map.size() > 0){
        auto _iter = _item_picker_area_map.begin();
        delete_item_picker_area(_iter->first);
      }

      while(_dropped_item_map.size() > 0){
        auto _iter = _dropped_item_map.begin();
        _delete_iwe2d(_iter->first);
      }
    }
  }
}


void item_world_entity_2d_handler::drop_item(uint64_t id, Vector2 speed){
  if(!_root_drop){
    INIT_ASSERT()
    ERR_ASSERT("No root drop, item will not be dropped.")
    return;
  }
  
  INIT_ASSERT()

  LOG_ASSERT("test1")
  uint64_t _container_prev = _item_server->find_container_has_item(id);
  if(_container_prev == ITEM_CONTAINER_ID_NULL || _container_prev == _drop_container_id)
    return;

  LOG_ASSERT("test2")
  auto _iter_bound_container = _bound_dropper_containers_map.find(_container_prev);
  if(_iter_bound_container == _bound_dropper_containers_map.end())
    return;

  LOG_ASSERT("test3")
  const item_object* _item_obj = _item_server->get_item_object(id);

  LOG_ASSERT("test4")
  _item_server->remove_from_all_container(id);
  _item_server->add_to_container(_drop_container_id, id);

  LOG_ASSERT("test5")
  Vector2 _start_pos = _iter_bound_container->second->_node->get_global_position();
  item_world_entity_2d* _new_item_entity = (item_world_entity_2d*)_item_world_entity_pckscene->instantiate();
  _new_item_entity->_item_server = _item_server;
  _new_item_entity->_iwe2d_handler = this;

  speed += _iter_bound_container->second->_node->get_speed();
  if(speed.length() > maximum_throw_speed)
    speed = speed.normalized() * maximum_throw_speed;

  LOG_ASSERT("test6")
  get_subautoload_root()->add_child_node(_root_drop, _new_item_entity);

  LOG_ASSERT("test7")
  _new_item_entity->bind_item(_item_obj);
  _new_item_entity->set_global_position(_start_pos);
  _new_item_entity->throw_object(speed);

  LOG_ASSERT("test8")
  _item_world_entity_metadata* _metadata = new _item_world_entity_metadata();
  _metadata->_item_id = id;
  _metadata->_node = _new_item_entity;

  LOG_ASSERT("test9")
  _dropped_item_map[id] = _metadata; 
  LOG_ASSERT("test10")
}

void item_world_entity_2d_handler::drop_item_random(uint64_t id){
  RandomNumberGenerator* _rng = _rng_server->get_randomizer(rng_type_global);
  double _new_speed = _rng->randf_range(0, random_speed);
  double _new_angle = _rng->randf_range(0, 360);

  Vector2 _new_force =
    Vector2(1, 1).rotated(Math::deg_to_rad(_new_angle)) *
    _new_speed
  ;

  drop_item(id, _new_force);
}


bool item_world_entity_2d_handler::pick_item(iwe2d_inst_id id, item_picker_inst_id picker_id){
  auto _iter = _dropped_item_map.find(id);
  if(_iter == _dropped_item_map.end())
    return false;

  auto _metadata_iter = _item_picker_area_map.find(picker_id);
  if(_metadata_iter == _item_picker_area_map.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(picker_id);
      _paramarr.append(id);
    }

    ERR_ASSERT(String("Cannot find picker_id: {0}. Item (id: {1}) will not be picked").format(_paramarr))
    return false;
  }

  if(!_item_server->add_to_container(_metadata_iter->second->_bound_container_id, _iter->second->_item_id))
    return false;

  _item_server->remove_from_all_container(_iter->second->_item_id, {_metadata_iter->second->_bound_container_id});

  item_world_entity_2d* _iwe2d_object = _iter->second->_node;
  _iwe2d_object->on_object_picked_up();

  _delete_iwe2d(id);
  return true;
}


void item_world_entity_2d_handler::bind_container_dropper(uint64_t container_id, uint64_t dropper_id){
  Node* _container_node = _item_server->get_item_container_node(container_id);
  if(!_container_node)
    return;
  
  auto _iter_dropper = _item_dropper_map.find(dropper_id);
  if(_iter_dropper == _item_dropper_map.end())
    return;

  _item_dropper_metadata* _metadata = _iter_dropper->second;
  _metadata->_bound_containers_id.insert(container_id);
  
  _bound_dropper_containers_map[container_id] = _metadata;
}

void item_world_entity_2d_handler::bind_container_picker(uint64_t container_id, uint64_t picker_id){
  Node* _container_node = _item_server->get_item_container_node(container_id);
  if(!_container_node)
    return;

  auto _iter_picker = _item_picker_area_map.find(picker_id);
  if(_iter_picker == _item_picker_area_map.end())
    return;

  _item_picker_area_metadata* _metadata = _iter_picker->second;
  if(_metadata->_bound_container_id != ITEM_CONTAINER_ID_NULL){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(container_id);
      _paramarr.append(picker_id);
      _paramarr.append(_metadata->_bound_container_id);
    }

    ERR_ASSERT(String("Trying to bound with container (id:{0}). Picker area (id:{1}) already bound with (id:{2})").format(_paramarr))
    return;
  }

  _metadata->_bound_container_id = container_id;
  _metadata->_node->bind_item_container(container_id);

  _bound_picker_containers_map[container_id] = _metadata;
}


void item_world_entity_2d_handler::unbind_container_dropper(uint64_t container_id, uint64_t dropper_id){
  auto _iter_bound_container = _bound_dropper_containers_map.find(container_id);
  if(_iter_bound_container != _bound_dropper_containers_map.end()){
    _bound_dropper_containers_map.erase(_iter_bound_container);
  }

  auto _iter_dropper = _item_dropper_map.find(dropper_id);
  if(_iter_dropper != _item_dropper_map.end()){
    _item_dropper_metadata* _metadata = _iter_dropper->second;
    _metadata->_bound_containers_id.erase(container_id);
  }
}

void item_world_entity_2d_handler::unbind_container_picker(uint64_t container_id, uint64_t picker_id){
  auto _iter_picker = _item_picker_area_map.find(picker_id);
  if(_iter_picker == _item_picker_area_map.end()){
    _item_picker_area_metadata* _metadata = _iter_picker->second;
    if(_metadata->_bound_container_id == container_id){
      _metadata->_bound_container_id = ITEM_CONTAINER_ID_NULL;
      _metadata->_node->unbind_item_container();
    }
  }

  auto _iter_bound_container = _bound_picker_containers_map.find(container_id);
  if(_iter_bound_container != _bound_picker_containers_map.end()){
    _bound_picker_containers_map.erase(_iter_bound_container);
  }
}


item_dropper* item_world_entity_2d_handler::create_item_dropper(){
  item_dropper_inst_id _new_id = _get_new_dropper_id();
  if(_new_id == ITEM_DROPPER_ID_NULL){
    INIT_ASSERT()

    ERR_ASSERT("Cannot create more item_dropper.")
    return NULL;
  }

  item_dropper* _new_node = (item_dropper*)_item_dropper_pckscene->instantiate();
  _new_node->_iwe2d_handler = this;
  _new_node->_dropper_id = _new_id;

  _item_dropper_metadata* _metadata = new _item_dropper_metadata();
  _metadata->_node = _new_node;
  _metadata->_item_dropper_id = _new_id;

  _item_dropper_map[_new_id] = _metadata;

  return _new_node;
}

item_picker_area* item_world_entity_2d_handler::create_item_picker_area(){
  item_picker_inst_id _new_id = _get_new_picker_id();
  if(_new_id == ITEM_PICKER_ID_NULL){
    INIT_ASSERT()

    ERR_ASSERT("Cannot create more item_picker_area.")
    return NULL;
  }

  item_picker_area* _new_node = (item_picker_area*)_item_picker_area_pckscene->instantiate();
  _new_node->_item_server = _item_server;
  _new_node->_iwe2d_handler = this;
  _new_node->_item_picker_id = _new_id;

  _item_picker_area_metadata* _metadata = new _item_picker_area_metadata();
  _metadata->_bound_container_id = ITEM_CONTAINER_ID_NULL;
  _metadata->_item_picker_id = _new_id;
  _metadata->_node = _new_node;

  _item_picker_area_map[_new_id] = _metadata;

  return _new_node;
}


item_dropper* item_world_entity_2d_handler::get_item_dropper(item_dropper_inst_id id){
  auto _iter = _item_dropper_map.find(id);
  if(_iter == _item_dropper_map.end())
    return NULL;

  return _iter->second->_node;
}

item_picker_area* item_world_entity_2d_handler::get_item_picker_area(item_picker_inst_id id){
  auto _iter = _item_picker_area_map.find(id);
  if(_iter == _item_picker_area_map.end())
    return NULL;

  return _iter->second->_node;
}


bool item_world_entity_2d_handler::delete_item_dropper(item_dropper_inst_id id){
  return _delete_dropper(id);
}

bool item_world_entity_2d_handler::delete_item_picker_area(item_picker_inst_id id){
  return _delete_picker_area(id);
}