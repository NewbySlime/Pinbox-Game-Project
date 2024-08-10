#include "algorithm_identification.h"
#include "algorithm_node.h"
#include "defines.h"
#include "entity_server.h"
#include "item_dropper.h"
#include "item_picker_area.h"
#include "item_world_entity_2d_handler.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Item;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::DataStorage;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(entity_server)
ATTR_BASE_INTERFACE_HELPER_DEFINE(entity_server)
ATTR_INTERFACE_HELPER_DEFINE(entity_server)


I_item_container* entity_server::_create_item_container(inventory_config* config){
  I_item_container* _container = _item_server->create_item_container(
    config->get_inventory_size(),
    config->get_inventory_type()
  );

  return _container;
}


void entity_server::_init_entity_item_container(entity_inst_id id){
  auto _object_iter = _object_list_map.find(id);
  if(_object_iter == _object_list_map.end())
    return;

  _entity_metadata* _metadata = &_object_iter->second;
  // initiate ref
  entity_item_container_ref* _entity_icont = _metadata->_entity_subdata->get_subdata_mutable<entity_item_container_ref>();
  if(!_entity_icont){
    _metadata->_entity_subdata->create_subdata<entity_item_container_ref>();
    _entity_icont = _metadata->_entity_subdata->get_subdata_mutable<entity_item_container_ref>();
  }
  
  const entity_data* _data = _metadata->_this_object->_this_data;
  // initiate inventory
  entity_data::inv_config_data* _inv_config = _data->get_subdata<entity_data::inv_config_data>();
  if(_inv_config){
    I_item_container* _item_container = _create_item_container(_inv_config);
    _entity_icont->add_item_container(
      USAGE_ENTITY_ICONT_MAIN_INVENTORY,
      _item_container->get_container_id()
    );
  }

  // initiate loadout
  entity_data::loadout_inv_config_data* _loadout_inv_config = _data->get_subdata<entity_data::loadout_inv_config_data>();
  if(_loadout_inv_config){
    I_item_container* _item_container = _create_item_container(_loadout_inv_config);
    _entity_icont->add_item_container(
      USAGE_ENTITY_ICONT_LOADOUT_INVENTORY,
      _item_container->get_container_id()
    );
  }
}

void entity_server::_init_entity_iwe_tools(entity_inst_id id){
  auto _object_iter = _object_list_map.find(id);
  if(_object_iter == _object_list_map.end())
    return;

  item_world_entity_2d_handler* _iwe2d_handler = _item_server->get_iwe2d_handler();
  _entity_metadata* _metadata = &_object_iter->second;
  
  item_dropper* _idropper = find_child_node_type<item_dropper>(_metadata->_this_object);
  if(!_idropper){
    _idropper = _iwe2d_handler->create_item_dropper();
    _metadata->_this_object->add_child_sa(_idropper);
  }

  item_picker_area* _ipicker_area = find_child_node_type<item_picker_area>(_metadata->_this_object);
  if(!_ipicker_area){
    _ipicker_area = _iwe2d_handler->create_item_picker_area();
    _metadata->_this_object->add_child_sa(_ipicker_area);
  }
}


void entity_server::_on_item_container_removed(gdvar_custom_object param){
  item_server_item_container_removed_param* _param =
    GDVAR_CUSTOM_TO(param, item_server_item_container_removed_param);

  item_container_inst_id _inst_id = _param->get_container_id();
  auto _icontainer_iter = _item_container_owner_refback.find(_inst_id);
  if(_icontainer_iter == _item_container_owner_refback.end())
    return;

  entity_inst_id _entity_inst_id = _icontainer_iter->second;
  auto _econtainer_iter = _object_list_map.find(_entity_inst_id);
  if(_econtainer_iter == _object_list_map.end())
    return;

  entity_item_container_ref* _entity_icont = _econtainer_iter->second._entity_subdata->get_subdata_mutable<entity_item_container_ref>();
  if(_entity_icont)
   _entity_icont->remove_item_container(_inst_id);
}


void entity_server::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(entity_server)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(entity_server)
  ATTR_INTERFACE_HELPER_BIND_GODOT(entity_server)

  GD_PROPERTY_BIND(entity_server, Variant::DICTIONARY, list_entity_packed_scenes)

  ClassDB::bind_method(
    D_METHOD("_on_item_container_removed", "param"),
    &entity_server::_on_item_container_removed
  );
}


void entity_server::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(entity_database, container, _entity_database)
    GET_SUBAUTOLOAD(entity_resource_database, container, _entity_res_database)

    GET_SUBAUTOLOAD(item_server, container, _item_server)
  }
}

void entity_server::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _item_server->connect(SIGNAL_ITEM_SERVER_ITEM_CONTAINER_REMOVING, Callable(this, "_on_item_container_removed"));
  }
}


void entity_server::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      auto _iter = _object_list_map.begin();
      while(_iter != _object_list_map.end()){
        remove_entity(_iter->second._this_object->_inst_id);
        _iter = _object_list_map.begin();
      }
    }
  }
}


entity_object* entity_server::create_entity(uint64_t type, String entity_id){
  const entity_data* _data = _entity_database->get_entity_data(type, entity_id);
  const entity_resource_data* _res_data = _entity_res_database->get_resource_data(type, entity_id);
  object_subdata* _subdata = new object_subdata();

  Array _paramarr;{
    _paramarr.append(type);
    _paramarr.append(entity_id);
  }

  if(!_data || !_res_data){
    INIT_ASSERT()
    ERR_ASSERT(String("Cannot get entity_data or the resource data for {0}.{1}.").format(_paramarr))
    return NULL;
  }

  entity_inst_id _new_inst_id = get_new_id_from_map(_object_list_map, ENTITY_ID_NULL);
  if(_new_inst_id == ENTITY_ID_NULL){
    INIT_ASSERT()
    ERR_ASSERT(String("Server overloaded, cannot generate ID."))
    return NULL;
  }

  Ref<PackedScene> _entity_scene = _entity_database->get_entity_pckscene(type, entity_id);
  if(_entity_scene.is_null()){
    INIT_ASSERT()
    ERR_ASSERT(String("Cannot instantiate Entity object. ID: {0}.{1}").format(_paramarr))
    return NULL;
  }

  Node* _new_node = _entity_scene->instantiate();
  entity_object* _entity = (entity_object*)_new_node;
  _entity->_inst_id = _new_inst_id;
  _entity->_this_data = _data;
  _entity->_this_res_data = _res_data;
  _entity->_entity_subdata = _subdata;

  _entity_metadata _metadata;{
    _metadata._this_object = _entity;
    _metadata._entity_subdata = _subdata;
  } _object_list_map[_new_inst_id] = _metadata;
  _init_entity_item_container(_new_inst_id);

  return _entity;
}

entity_object* entity_server::create_entity_on(uint64_t type, String entity_id, Node* parent){
  entity_object* _new_entity = create_entity(type, entity_id);
  if(!_new_entity)
    return NULL;

  get_subautoload_root()->add_child_node(parent, _new_entity);
  return _new_entity;
}


entity_object* entity_server::get_entity(entity_inst_id instance_id){
  auto _iter = _object_list_map.find(instance_id);
  if(_iter == _object_list_map.end())
    return NULL;

  return _iter->second._this_object;
}


bool entity_server::remove_entity(entity_inst_id instance_id){
  auto _iter = _object_list_map.find(instance_id);
  if(_iter == _object_list_map.end())
    return false;

  _on_entity_deinstantiated(_iter->second._this_object);
  _iter->second._this_object->queue_free();
  return true;
}



void entity_server::_on_entity_deinstantiated(entity_object* _object){
  auto _iter = _object_list_map.find(_object->_inst_id);
  if(_iter != _object_list_map.end()){
    delete _iter->second._entity_subdata;

    _object_list_map.erase(_iter);
  }
}