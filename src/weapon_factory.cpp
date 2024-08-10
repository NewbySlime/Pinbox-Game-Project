#include "defines.h"
#include "logger.h"

#include "godot_error.h"
#include "item_server.h"
#include "weapon_factory.h"
#include "weapon_item.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/array.hpp"


using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace GameUtils::Memory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(weapon_factory)
ATTR_BASE_INTERFACE_HELPER_DEFINE(weapon_factory)
ATTR_INTERFACE_HELPER_DEFINE(weapon_factory)


bool weapon_factory::_is_item_prepped(item_object* _item_object){
  return _item_object->get_subdata<weapon_item>() != NULL;
}


void weapon_factory::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(weapon_factory)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(weapon_factory)
  ATTR_INTERFACE_HELPER_BIND_GODOT(weapon_factory)
}


void weapon_factory::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(item_server, sa_container, _item_server)
  }
}


void weapon_factory::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void weapon_factory::prep_item(item_inst_id id){
  item_object* _item = _get_item_object(_item_server, id);
  if(!_item)
    return;

  if(_is_item_prepped(_item))
    return;

  const item_data* _data = _item->get_item_data();

  // create a temporary weapon_item
  weapon_item _tmp_weapon_item;{
    // primary
    weapon_item::copy_stats_to(_data->get_subdata<weapon_stats_data_primary>(),
      &_tmp_weapon_item._cd_primary_stats,
      &_tmp_weapon_item._primary_stats
    );

    // secondary
    weapon_item::copy_stats_to(_data->get_subdata<weapon_stats_data_secondary>(),
      &_tmp_weapon_item._cd_secondary_stats,
      &_tmp_weapon_item._secondary_stats
    );
  }

  // the class will create another copy of it
  _item->set_subdata(&_tmp_weapon_item);
}


void weapon_factory::randomize_item_stats(item_inst_id id){
  item_object* _item = _get_item_object(_item_server, id);
  if(!_item)
    return;

  if(!_is_item_prepped(_item))
    prep_item(id);

  // TODO do something to randomize
}