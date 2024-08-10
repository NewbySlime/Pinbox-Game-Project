#include "damageable_object.h"
#include "entity_actor.h"
#include "entity_object.h"
#include "overrider_base.h"
#include "predelete_notifier.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Item;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(entity_actor)
ATTR_BASE_INTERFACE_HELPER_DEFINE(entity_actor)
ATTR_INTERFACE_HELPER_DEFINE(entity_actor)



void entity_actor::__metadata_delete(uint64_t id){
  auto _iter = _instanced_overrider_backreference_map.find(id);
  if(_iter == _instanced_overrider_backreference_map.end())
    return;
  
  String _overrider_id = _iter->second;
  _instanced_overrider_backreference_map.erase(_iter);

  __metadata_delete(_overrider_id);
}

void entity_actor::__metadata_delete(String associated_id){
  auto _iter = _overrider_metadata_map.find(associated_id);
  if(_iter == _overrider_metadata_map.end())
    return;

  _overrider_metadata* _metadata = _iter->second;
  _overrider_metadata_map.erase(_iter);

  Node* _metadata_node = _metadata->_current_instance;
  if(_metadata_node){
    _metadata_node->disconnect(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, Callable(this, "_on_overrider_freed"));
    __metadata_delete(_metadata_node->get_instance_id());

    // if doesn't have parent, free the node
    if(!_metadata_node->get_parent())
      _metadata_node->queue_free();
  }

  delete _metadata;
}


void entity_actor::_add_overrider_to_entity(entity_object* _entity_object){
  for(auto _pair: _overrider_metadata_map){
    if(!_pair.second->_current_instance)
      _create_overrider(_pair.first);
    
    Node* _this_overrider = _pair.second->_current_instance;
    if(!_this_overrider->get_parent())
      _this_overrider->reparent(_entity_object, false);
    else
      _entity_object->add_child_sa(_this_overrider);
  }
}

Node* entity_actor::_create_overrider(String associated_id){
  auto _iter = _overrider_metadata_map.find(associated_id);
  if(_iter == _overrider_metadata_map.end())
    return NULL;

  Node* _overrider_node = _iter->second->overrider_packed_scene->instantiate();

  _overrider_node->connect(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, Callable(this, "_on_overrider_freed"));

  _instanced_overrider_backreference_map[_overrider_node->get_instance_id()] = associated_id;
  _iter->second->_current_instance = _overrider_node;
  return _overrider_node;
}


void entity_actor::_on_overrider_freed(gdvar_custom_object param){
  predelete_notifier_signal_param* _param = GDVAR_CUSTOM_TO(param, predelete_notifier_signal_param);
  
  auto _iter_backreference = _instanced_overrider_backreference_map.find(_param->get_node()->get_instance_id());
  if(_iter_backreference == _instanced_overrider_backreference_map.end())
    return;

  auto _iter_metadata = _overrider_metadata_map.find(_iter_backreference->second);
  if(_iter_metadata == _overrider_metadata_map.end())
    return;

  // check if _current_instance is the same
  if(_param->get_node()->get_instance_id() == _iter_metadata->second->_current_instance->get_instance_id())
    _iter_metadata->second->_current_instance = NULL;
  _instanced_overrider_backreference_map.erase(_iter_backreference);
}


void entity_actor::_on_entity_killed(){
  this->_on_entity_killed_callback();
}

void entity_actor::_on_entity_despawned(){
  this->_on_entity_despawned_callback();
}


void entity_actor::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(entity_actor)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(entity_actor)
  ATTR_INTERFACE_HELPER_BIND_GODOT(entity_actor)

  ClassDB::bind_method(
    D_METHOD("_on_overrider_freed", "param"),
    &entity_actor::_on_overrider_freed
  );

  ClassDB::bind_method(
    D_METHOD("_on_entity_killed"),
    &entity_actor::_on_entity_killed
  );

  ClassDB::bind_method(
    D_METHOD("_on_entity_despawned"),
    &entity_actor::_on_entity_despawned
  );
}


void entity_actor::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(entity_server, container, _entity_server)
  }
}

void entity_actor::_on_gamenode_ready(){

}


bool entity_actor::_add_overrider(String associated_id, Ref<PackedScene> pckscene){
  Node* _test_instance = pckscene->instantiate();
  bool _is_failing = false;

  overrider_base* _test_overrider = NULL; INTERFACE_GET(_test_instance, overrider_base, _test_overrider)
  if(!_test_overrider){
    _is_failing = true;

    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(associated_id);
      _paramarr.append(overrider_base::get_id());
    }

    ERR_ASSERT(String("Cannot add overrider; overrider (id: '{0}') does not have '{1}' interface.").format(_paramarr))
  }

  if(!_test_instance->has_signal(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE)){
    _is_failing = true;

    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(associated_id);
    }

    ERR_ASSERT(String("Cannot add overrider; overrider (id: '{0}') does not have predelete_notifier signal.").format(_paramarr))
  }


  // check is_failing
  if(_is_failing){
    _test_instance->queue_free();
    return false;
  }


  _overrider_metadata* _new_metadata = new _overrider_metadata();{
    _new_metadata->overrider_packed_scene = pckscene;
    _new_metadata->_current_instance = NULL;
  }

  auto _iter = _overrider_metadata_map.find(associated_id);
  if(_iter != _overrider_metadata_map.end())
    __metadata_delete(_iter->first);

  _overrider_metadata_map[associated_id] = _new_metadata;
  return true;
}

void entity_actor::_remove_overrider(String associated_id){
  auto _iter = _overrider_metadata_map.find(associated_id);
  if(_iter == _overrider_metadata_map.end())
    return;
  
  __metadata_delete(_iter->first);
}


void entity_actor::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
  
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _overrider_metadata_map)
        __metadata_delete(_pair.first);
    }
  }
}


void entity_actor::bind_to_entity(Game::Entity::entity_inst_id entity_id){
  entity_object* _this_entity = _entity_server->get_entity(entity_id);
  if(!_this_entity){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append((uint64_t)entity_id);
    }

    ERR_ASSERT(String("Cannot get entity_object of ID; {0}").format(_paramarr))
    return;
  }

  _bound_entity_id = entity_id;
  _this_entity->connect(SIGNAL_ENTITY_OBJECT_ON_DESPAWNED, Callable(this, "_on_entity_despawned"));

  if(_this_entity->is_class(damageable_object::get_class_static())){
    _this_entity->connect(SIGNAL_DAMAGEABLE_OBJECT_ON_DEAD, Callable(this, "_on_entity_killed"));
  }

  _add_overrider_to_entity(_this_entity);
}