#include "algorithm_random.h"
#include "defines.h"
#include "godot_error.h"
#include "modifier_container_item.h"
#include "modifier_factory.h"
#include "modifiers.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/json.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

#include "map"
#include "string"


using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace Game::Misc::Algorithm;
using namespace Game::Modifiers;
using namespace GameUtils::Memory;
using namespace godot;



ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(modifier_factory)
ATTR_BASE_INTERFACE_HELPER_DEFINE(modifier_factory)
ATTR_INTERFACE_HELPER_DEFINE(modifier_factory)


void modifier_factory::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(modifier_factory)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(modifier_factory)
  ATTR_INTERFACE_HELPER_BIND_GODOT(modifier_factory)

  GD_PROPERTY_BIND_HINT(modifier_factory, Variant::STRING, modifier_factory_JSON, PropertyHint::PROPERTY_HINT_FILE)

  GD_PROPERTY_BIND(modifier_factory, Variant::INT, min_modifier_combination)
  GD_PROPERTY_BIND(modifier_factory, Variant::INT, max_modifier_combination)
}


void modifier_factory::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(rng_server, sa_container, _rng_server)

    GET_SUBAUTOLOAD(modifier_database, sa_container, _modifier_database)
    GET_SUBAUTOLOAD(item_server, sa_container, _item_server)
  }
}


void modifier_factory::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void modifier_factory::prep_item(item_inst_id id){
  item_object* _item = _get_item_object(_item_server, id);
  modifier_container_item* _modifier_data = _item->get_subdata_mutable<modifier_container_item>();
  if(_modifier_data)
    return;

  modifier_container_item _new_modifier_data;
  _item->set_subdata(&_new_modifier_data);
}

void modifier_factory::randomize_item_stats(item_inst_id id){
  // to check or to prep
  prep_item(id);

  item_object* _item = _get_item_object(_item_server, id);
  modifier_container_item* _modifier_data = _item->get_subdata_mutable<modifier_container_item>();

  _modifier_data->clear_modifiers();

  RandomNumberGenerator* _rng = _rng_server->get_randomizer(rng_types::rng_type_item_generation);
  int _modifier_count = _rng->randi_range(min_modifier_combination, max_modifier_combination);
  for(int i = 0; i < _modifier_count; i++){
    std::string _modifier_id = _modifier_database->get_random_modifier_id(_rng);
    try{
      std::shared_ptr<I_modifier> _new_modifier = _modifier_database->create_specific_modifier(_modifier_id);
      _modifier_database->randomize_modifier(_rng, _new_modifier);
      
      _modifier_data->add_modifier(_new_modifier);
    }
    catch(gdstr_exception e){
      INIT_ASSERT()
      Array _paramarr;{
        _paramarr.append(_modifier_id.c_str());
        _paramarr.append(e.what());
      }

      ERR_ASSERT(String("Cannot create randomized modifier with ID '{0}'. Error; {1}").format(_paramarr))
    }
  }
}