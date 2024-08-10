#include "algorithm_container.h"
#include "algorithm_identification.h"
#include "defines.h"
#include "logger.h"
#include "godot_error.h"
#include "godot_signal.h"
#include "item_server.h"
#include "item_world_entity_2d_handler.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/json.hpp"



using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::Memory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_server)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_server)
ATTR_INTERFACE_HELPER_DEFINE(item_server)


struct static_data: public custom_data{
  public:
    static String get_id(){ return "item_server.static_data"; }

    const std::map<std::string, premade_item_container_type> _item_container_types_parse = {
      {"player_item_container", pict_player},
      {"player_weapon_item_container", pict_player_weapon}
    };

    const std::map<item_type, std::string> _item_types_parse_inverse = {
      {it_modifier, "modifier"},
      {it_weapon, "weapon"},
      {it_material, "material"}
    };

};


uint64_t item_server::_find_new_item_id(){
  return get_new_id_from_map<_item_metadata*>(_item_metadata_map, ITEM_ID_NULL);
}

uint64_t item_server::_add_new_item(item_object* _item){
  uint64_t _new_id = _find_new_item_id();
  _item->_this_id = _new_id;

  _item_metadata* _metadata = new _item_metadata();
  _metadata->_self = _item;
  _metadata->_main_container_id = ITEM_CONTAINER_ID_NULL;
  
  _item_count++;
  _current_item_id_iter = _new_id;
  _item_metadata_map[_new_id] = _metadata;

  return _new_id;
}


uint64_t item_server::_find_new_container_id(){
  return get_new_id_from_map<_item_container_metadata*>(_container_map, ITEM_CONTAINER_ID_NULL);
}

I_item_container* item_server::_create_new_container(int size, uint32_t type, uint64_t new_id){
  auto _iter_container_creator = _item_container_type_pckscene.find(type);
  if(_iter_container_creator == _item_container_type_pckscene.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(type);
    }

    ERR_ASSERT(String("Cannot find container creator for type {0}.").format(_paramarr))
    return NULL;
  }


  if(new_id == ITEM_CONTAINER_ID_NULL)
    new_id = _find_new_container_id();

  else if(_container_map.find(new_id) != _container_map.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(new_id);
    }

    ERR_ASSERT(String("ID {0} for container has been taken.").format(_paramarr))
    return NULL;
  }

  item_container* _new_container = (item_container*)_iter_container_creator->second->instantiate();
  _new_container->_container_id = new_id;
  _new_container->_server = this;

  _item_container_metadata* _cont_metadata = new _item_container_metadata();
  _cont_metadata->_container_obj = _new_container;
  _cont_metadata->_node_obj = _new_container;
  _container_map[new_id] = _cont_metadata;

  add_child_sa(_new_container);
  _new_container->resize_container(size);

  return _new_container;
}


bool item_server::_add_to_container(I_item_container* _container, uint64_t item_id, int index_pos){
  INIT_ASSERT()

  LOG_ASSERT("test1")
  if(_container->empty_slot_count() <= 0)
    return false;

  LOG_ASSERT("test2")
  auto _metadata_iter = _item_metadata_map.find(item_id);
  if(_metadata_iter == _item_metadata_map.end())
    return false;
  
  LOG_ASSERT("test3")
  auto _container_iter = _metadata_iter->second->container_ids.find(_container->get_container_id());
  if(_container_iter != _metadata_iter->second->container_ids.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(item_id);
      _paramarr.append(_container->get_container_id());
    }

    ERR_ASSERT(String("Item (id:{0}) already in container (id:{1}).").format(_paramarr))
    return false;
  }

  LOG_ASSERT("test4")
  index_pos = _container->_add_item(_metadata_iter->second->_self, index_pos);
  if(index_pos < 0)
    return false;

  LOG_ASSERT("test5")
  item_position_data _pos_data;
  _pos_data.container_id = _container->get_container_id();
  _pos_data.index_position = index_pos;

  LOG_ASSERT("test6")
  if(_metadata_iter->second->container_ids.size() <= 0)
    _metadata_iter->second->_main_container_id = _pos_data.container_id;

  LOG_ASSERT("test")
  _metadata_iter->second->container_ids[_pos_data.container_id] = _pos_data;
  return true;
}

uint64_t item_server::_remove_from_container(I_item_container* _container, int index_pos){
  uint64_t _r_id = _container->_remove_item(index_pos);
  if(_r_id == ITEM_ID_NULL)
    return _r_id;

  auto _metadata_iter = _item_metadata_map.find(_r_id);
  if(_metadata_iter != _item_metadata_map.end()){
    _metadata_iter->second->container_ids.erase(_container->get_container_id());
    if(_container->get_container_id() == _metadata_iter->second->_main_container_id){
      _metadata_iter->second->_main_container_id = ITEM_CONTAINER_ID_NULL;
      remove_from_all_container(_r_id);
    }
  }
  else{
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(_r_id);
    }

    ERR_ASSERT(String("Cannot find item id:{0}, possible item \"leak\"?").format(_paramarr))
  }

  return _r_id;
}


bool item_server::_clear_container(I_item_container* _container){
  for(int i = 0; i < _container->get_container_size(); i++)
    _remove_from_container(_container, i);

  return true;
}


void item_server::_parse_item_container_premade_json(Dictionary* _data_dict){
  static_data* _static = _static_data->get_data<static_data>();

  INIT_ASSERT()
  Array _paramarr;
  Variant _v;

  ResourceLoader* _res_loader = ResourceLoader::get_singleton();
  Array _keys = _data_dict->keys();
  for(int i = 0; i < _keys.size(); i++){
    String _key = _keys[i];
    
    try{
      uint64_t _ict_enum;
      {auto _iter = _static->_item_container_types_parse.find(GDSTR_TO_STDSTR(_key));
        if(_iter == _static->_item_container_types_parse.end())
          throw gdstr_exception("item_container_premade_data id key isn't valid.");

        _ict_enum = _iter->second;
      }

      item_container_premade_data* _curr_premade_data;
      {auto _iter = _item_cont_premade_data_map.find(_ict_enum);
        if(_iter == _item_cont_premade_data_map.end())
          throw gdstr_exception("Cannot find a item_container_premade_data with current id key.");

        _curr_premade_data = _iter->second;
      }

      _v = _data_dict->operator[](_key);
      _CREATE_CHECKER_DICT1("", _key, Variant::DICTIONARY)
      Dictionary _data_part = _v;

      _CREATE_CHECKER_DICT2(_key, VARNAME_ITEM_CONTAINER_PREMADE_DATA_SIZE, Variant::FLOAT)
      _curr_premade_data->container_size = _v;
    }
    catch(gdstr_exception e){
      _paramarr.clear();{
        _paramarr.append(_key);
      }

      ERR_ASSERT(String("For item_container_premade_data with id: {0}").format(_paramarr))
      ERR_ASSERT(String(e.what()))
    }
  }
}


void item_server::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_server)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_server)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_server)

  GD_PROPERTY_BIND_HINT(item_server, Variant::STRING, item_container_premade_data_json, PropertyHint::PROPERTY_HINT_FILE)

  GD_PROPERTY_BIND_HINT(item_server, Variant::STRING, item_world_entity_2d_handler_pckscene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(item_server, Variant::PACKED_STRING_ARRAY, item_container_pckscene_array, PropertyHint::PROPERTY_HINT_FILE)

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_SERVER_ITEM_REMOVED,
    PropertyInfo(SIGNAL_PARAM_CUSTOM_VARIANT_TYPE, "item_id_data")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_SERVER_ITEM_CONTAINER_REMOVING,
    PropertyInfo(SIGNAL_PARAM_CUSTOM_VARIANT_TYPE, "item_id_data")
  ));
}


void item_server::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data)
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data.")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    for(auto _pair: _static->_item_container_types_parse){
      _item_cont_premade_data_map[_pair.second] = new item_container_premade_data();
    }

    _paramarr.clear();{
      _paramarr.append((uint64_t)this);
    }

    LOG_ASSERT(String("item_server address {0}").format(_paramarr))

    ResourceLoader* _res_loader = ResourceLoader::get_singleton();
    for(int i = 0; i < item_container_pckscene_array.size(); i++){
      Ref<PackedScene> _test_pckscene;
      CREATE_LOAD_RESOURCE_CHECKER(_test_pckscene, item_container_pckscene_array[i], PackedScene)
      Node* _test_node = _test_pckscene->instantiate();
      if(_test_node->is_class("item_container")){
        item_container* _test_container = (item_container*)_test_node;
        _item_container_type_pckscene[_test_container->get_container_type()] = _test_pckscene;
      }
      else{
        ERR_ASSERT(String("Instantiated PackedScene is not an item_container."))
      }

      _test_node->queue_free();
    }

    PROCESS_JSON_PATH(
      item_container_premade_data_json,
      _parse_item_container_premade_json
    )

    for(auto _pair: _item_cont_premade_data_map){
      _create_new_container(_pair.second->container_size, ict_generic, _pair.first);
    }


    Ref<PackedScene> _iwe2d_handler_pckscene;
    CREATE_LOAD_PACKED_SCENE_CHECKER(_iwe2d_handler_pckscene, item_world_entity_2d_handler_pckscene, item_world_entity_2d_handler)
    _iwe2d_handler = (item_world_entity_2d_handler*)_iwe2d_handler_pckscene->instantiate();
    _iwe2d_handler->_item_server = this;
    add_child_sa(_iwe2d_handler);
  }
}


item_object* item_server::_get_item_object_mutable(item_inst_id id){
  auto _iter = _item_metadata_map.find(id);
  if(_iter == _item_metadata_map.end())
    return NULL;

  return _iter->second->_self;
}


item_server::item_server(){
  _iwe2d_handler = NULL;

  _current_item_id_iter = 0;
  _item_count = 0;

  _current_container_id_iter = 0;
  _container_count = 0;
}


void item_server::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_iwe2d_handler)
        _iwe2d_handler->queue_free();

      // deleting containers
      while(_container_map.size() > 0){
        auto _iter = _container_map.begin();
        delete_item_container(_iter->first);
      }
      
      // deleting items
      while(_item_metadata_map.size() > 0){
        auto _iter = _item_metadata_map.begin();
        delete_item(_iter->first);
      }

      for(auto _pair: _item_cont_premade_data_map){
        delete _pair.second;
      }
    }
  }
}


const item_object* item_server::get_item_object(uint64_t item_id){
  if(item_id == ITEM_ID_NULL)
    return NULL;

  auto _iter = _item_metadata_map.find(item_id);
  if(_iter == _item_metadata_map.end())
    return NULL;

  return _iter->second->_self;
}


I_item_container* item_server::get_item_container(uint64_t id){
  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append((uint64_t)this);
  }

  LOG_ASSERT(String("address {0}").format(_paramarr))

  LOG_ASSERT("test1")
  if(id == ITEM_CONTAINER_ID_NULL)
    return NULL;

  LOG_ASSERT("test2")
  auto _iter = _container_map.find(id);
  LOG_ASSERT("test3")
  if(_iter == _container_map.end())
    return NULL;

  return _iter->second->_container_obj;
}

Node* item_server::get_item_container_node(uint64_t id){
  if(id == ITEM_CONTAINER_ID_NULL)
    return NULL;

  auto _iter = _container_map.find(id);
  if(_iter == _container_map.end())
    return NULL;

  return _iter->second->_node_obj;
}


uint64_t item_server::find_container_has_item(uint64_t item_id){
  if(item_id == ITEM_ID_NULL)
    return ITEM_CONTAINER_ID_NULL;

  auto _iter = _item_metadata_map.find(item_id);
  if(_iter == _item_metadata_map.end())
    return ITEM_CONTAINER_ID_NULL;

  return _iter->second->_main_container_id;
}


bool item_server::set_item_container_filter_type(item_container_inst_id id, uint32_t new_type){
  if(id == ITEM_CONTAINER_ID_NULL)
    return false;

  I_item_container* _curr_cont = get_item_container(id);
  if(!_curr_cont)
    return false;

  return _curr_cont->set_item_filter_type(new_type);
}


bool item_server::swap_item(item_position_data& pos_from, item_position_data& pos_to){
  I_item_container* container_from = get_item_container(pos_from.container_id);
  I_item_container* container_to = get_item_container(pos_to.container_id);
  
  if(!container_from || !container_to)
    return false;

  uint64_t id_from = container_from->get_item_id(pos_from.index_position);
  uint64_t id_to = container_to->get_item_id(pos_to.index_position);

  // check item from
  {const item_object* _item_from = get_item_object(id_from);
    if(!_item_from || !container_to->is_item_can_exist(_item_from))
      return false;
  }

  // check item to
  {const item_object* _item_to = get_item_object(id_to);
    // if item null, then don't cancel it
    if(_item_to){
      if(!container_from->is_item_can_exist(_item_to))
        return false;
    }
  }

  _remove_from_container(container_from, pos_from.index_position);
  if(id_to != ITEM_ID_NULL){
    _remove_from_container(container_to, pos_to.index_position);
    _add_to_container(container_from, id_to, pos_from.index_position);
  }

  _add_to_container(container_to, id_from, pos_to.index_position);
  return true;
}


bool item_server::add_to_container(uint64_t container_id, uint64_t item_id, int index){
  I_item_container* _container = get_item_container(container_id);
  if(!_container)
    return false;

  return _add_to_container(_container, item_id, index);
}


void item_server::remove_from_all_container(uint64_t item_id, std::set<uint64_t> _exception){
  auto _iter = _item_metadata_map.find(item_id);
  if(_iter == _item_metadata_map.end())
    return;

  size_t _excepted_count = 0;
  while(_iter->second->container_ids.size() > _excepted_count){
    auto _pos_iter = _iter->second->container_ids.begin();
    if(_exception.find(_pos_iter->second.container_id) != _exception.end()){
      _excepted_count++;
      continue;
    }

    remove_from_container_index(_pos_iter->second.container_id, _pos_iter->second.index_position);
  }
}

uint64_t item_server::remove_from_container(uint64_t container_id, uint64_t item_id){
  auto _iter = _item_metadata_map.find(item_id);
  if(_iter == _item_metadata_map.end())
    return ITEM_ID_NULL;

  auto _pos_iter = _iter->second->container_ids.find(container_id);
  if(_pos_iter == _iter->second->container_ids.end())
    return ITEM_ID_NULL;

  return remove_from_container_index(container_id, _pos_iter->second.index_position);
}

uint64_t item_server::remove_from_container_index(uint64_t container_id, int index){
  I_item_container* _container = get_item_container(container_id);
  if(!_container)
    return ITEM_ID_NULL;

  return _remove_from_container(_container, index);
}


bool item_server::delete_item(item_inst_id item_id){
  auto _iter = _item_metadata_map.find(item_id);
  if(_iter == _item_metadata_map.end())
    return false;

  remove_from_all_container(item_id);
  _item_metadata* _metadata = _iter->second;

  _item_metadata_map.erase(item_id);
  _item_count--;

  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append(_item_metadata_map.size());
  }

  LOG_ASSERT(String("new size {0}").format(_paramarr))

  delete _metadata->_self;
  delete _metadata;

  item_server_item_removed_param _item_id;
  _item_id._id = item_id;
  emit_signal(
    SIGNAL_ITEM_SERVER_ITEM_REMOVED,
    SIGNAL_PARAM_TO_CUSTOM(_item_id)
  );

  return true;
}

bool item_server::delete_item_container(item_container_inst_id container_id){
  auto _iter = _container_map.find(container_id);
  if(_iter == _container_map.end())
    return false;

  item_server_item_container_removed_param _param;
  _param._id = container_id;

  emit_signal(
    SIGNAL_ITEM_SERVER_ITEM_CONTAINER_REMOVING,
    SIGNAL_PARAM_TO_CUSTOM(_param)
  );

  _item_container_metadata* _container_metadata = _iter->second;
  _clear_container(_container_metadata->_container_obj);

  _container_map.erase(_iter);
  _container_metadata->_node_obj->queue_free();
  delete _container_metadata;

  return true;
}


bool item_server::drop_item(item_position_data& item_pos){
  auto _container_iter = _container_map.find(item_pos.container_id);
  if(_container_iter == _container_map.end())
    return false;

  item_inst_id id = _container_iter->second->_container_obj->get_item_id(item_pos.index_position);
  if(id == ITEM_ID_NULL)
    return false;

  auto _iter = _item_metadata_map.find(id);
  if(_iter == _item_metadata_map.end())
    return false;

  _iwe2d_handler->drop_item_random(id);
  return true;
}


const item_object* item_server::create_item_object(uint64_t type, const String& item_id){
  static_data* _static = _static_data->get_data<static_data>();


  auto _types_iter = _static->_item_types_parse_inverse.find((item_type)type);
  if(_types_iter == _static->_item_types_parse_inverse.end())
    return NULL;

  std::string _types_str = _types_iter->second;
  item_data* _target_data = _item_database->get_item_data(_types_str.c_str(), item_id);
  if(!_target_data)
    return NULL;

  item_resource_data* _target_res_data = _item_res_database->get_item_res_data(_types_str.c_str(), item_id);
  if(!_target_res_data)
    return NULL;

  item_object* _new_item_obj = new item_object();
  _add_new_item(_new_item_obj);

  _new_item_obj->_this_type = type;
  return _new_item_obj;
}

I_item_container* item_server::create_item_container(int container_size, uint32_t container_type, uint64_t* new_id){
  if(new_id)
    *new_id = ITEM_ID_NULL;

  I_item_container* _container = _create_new_container(container_size, container_type);
  if(_container && new_id)
    *new_id = _container->get_container_id();
  
  return _container;
}


item_world_entity_2d_handler* item_server::get_iwe2d_handler(){
  return _iwe2d_handler;
}