#include "item_pouch.h"
#include "item_pouch_data.h"
#include "item_pouch_factory.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_pouch_factory)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_pouch_factory)
ATTR_INTERFACE_HELPER_DEFINE(item_pouch_factory)


bool item_pouch_factory::_is_item_prepped(item_object* _item){
  return _item->get_subdata<item_pouch>() != NULL;
}


void item_pouch_factory::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_pouch_factory)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_pouch_factory)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_pouch_factory)
}


void item_pouch_factory::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(item_server, container, _item_server)
  }
}


void item_pouch_factory::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void item_pouch_factory::prep_item(item_inst_id id){
  item_object* _item = _get_item_object(_item_server, id);
  if(_is_item_prepped(_item))
    return;

  const item_data* _item_data = _item->get_item_data();
  const item_pouch_data* _pouch_data = _item_data->get_subdata<item_pouch_data>();
  if(!_pouch_data)
    return;

  I_item_container* _item_container = _item_server->create_item_container(_pouch_data->get_pouch_size(), _pouch_data->get_container_type());
  if(!_item_container){
    INIT_ASSERT()
    ERR_ASSERT("Cannot create item container.")

    return;
  }

  _item_container->set_item_filter_type(_pouch_data->get_item_filter());
  item_pouch _tmp_pouch;{
    _tmp_pouch._container = _item_container;
    _tmp_pouch._container_node = _item_server->get_item_container_node(_item_container->get_container_id());
  }

  _item->set_subdata(&_tmp_pouch);
}