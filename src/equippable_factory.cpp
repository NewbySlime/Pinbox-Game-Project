#include "equippable_factory.h"

#include "consumables_handler.h"
#include "item_object.h"
#include "weapon_handler.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Item::Factory;
using namespace godot;


#define ITEM_THROWER_WEAPON_P_ID "0"


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(equippable_factory)
ATTR_BASE_INTERFACE_HELPER_DEFINE(equippable_factory)
ATTR_INTERFACE_HELPER_DEFINE(equippable_factory)


void equippable_factory::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(equippable_factory)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(equippable_factory)
  ATTR_INTERFACE_HELPER_BIND_GODOT(equippable_factory)

  GD_PROPERTY_BIND_HINT(equippable_factory, Variant::STRING, equippable_object_pckscene, PropertyHint::PROPERTY_HINT_FILE)

  GD_PROPERTY_BIND_HINT(equippable_factory, Variant::STRING, consumables_handler_pckscene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(equippable_factory, Variant::STRING, weapon_handler_pckscene, PropertyHint::PROPERTY_HINT_FILE)
}



void equippable_factory::_bind_subautoload(Game::subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(item_server, container, _current_item_server)
  }
}

void equippable_factory::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_LOAD_PACKED_SCENE_CHECKER(_equipable_pckscene, equippable_object_pckscene, equippable_object)
    CREATE_LOAD_PACKED_SCENE_CHECKER(_consumables_pckscene, consumables_handler_pckscene, consumables_handler)
    CREATE_LOAD_PACKED_SCENE_CHECKER(_weapon_pckscene, weapon_handler_pckscene, weapon_handler) 
  }
}


void equippable_factory::_delete_object(equippable_object* object, bool already_deleted){
  const item_object* _current_item = object->_current_item;
  auto _iter = _object_map.find(_current_item->get_id());
  if(_iter == _object_map.end())
    return;

  if(!already_deleted)
    _iter->second->queue_free();

  _object_map.erase(_iter);
}


void equippable_factory::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      while(_object_map.size() > 0){
        auto _iter = _object_map.begin();
        delete_object(_iter->second);

        _object_map.erase(_iter);
      }
    } 
  }
}


equippable_object* equippable_factory::create_equippable_object(item_inst_id id){
  auto _iter = _object_map.find(id);
  if(_iter != _object_map.end())
    return NULL;

  const item_object* item = _current_item_server->get_item_object(id);
  if(!item)
    return NULL;

  equippable_object* _result = NULL;
  switch(item->get_item_type()){
    break;
     case it_material:
     case it_modifier:{
      // TODO set to item thrower
      weapon_handler* _handler = (weapon_handler*)_weapon_pckscene->instantiate();
      //_handler->set_weapon_p_id(ITEM_THROWER_WEAPON_P_ID);

      _result = (equippable_object*)_handler;
    }
    
    break; case it_weapon:{
      weapon_handler* _handler = (weapon_handler*)_weapon_pckscene->instantiate();
      _result = (equippable_object*)_handler;
    }

    break; case it_consumables:{
      consumables_handler* _handler = (consumables_handler*)_consumables_pckscene->instantiate();
      _result = (equippable_object*)_handler;
    }

    break; case it_throwables:{
      weapon_handler* _handler = (weapon_handler*)_weapon_pckscene->instantiate();
      // TODO set to item thrower
      //_handler->set_weapon_p_id(ITEM_THROWER_WEAPON_P_ID);
      
      _result = (equippable_object*)_handler;
    }
  }

  if(_result){
    _result->_current_factory = this;
    _result->_iwe2d_handler = _iwe2d_handler;
    _result->_item_server = _current_item_server;
    
    _result->_set_item(item);
  }

  return _result;
}

equippable_object* equippable_factory::get_equippable_object(item_inst_id id){
  auto _iter = _object_map.find(id);
  if(_iter == _object_map.end())
    return NULL;

  return _iter->second;
}


void equippable_factory::delete_object(equippable_object* object){
  _delete_object(object, false);
}