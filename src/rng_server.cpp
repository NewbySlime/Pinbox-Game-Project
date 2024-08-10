#include "logger.h"
#include "rng_server.h"
#include "static_variable_storage.h"

#include "godot_cpp/classes/engine.hpp"

#include "vector"


#define TOOL_RNG_TYPE rng_type_global


using namespace Game;
using namespace GameUtils::Memory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(rng_server)
ATTR_BASE_INTERFACE_HELPER_DEFINE(rng_server)
ATTR_INTERFACE_HELPER_DEFINE(rng_server)


struct static_data: public custom_data{
  public:
    static String get_id(){ return "rng_server.static_data"; }

    const std::vector<rng_types> _rng_init_list = {
      rng_type_global,
      rng_type_item_generation,
      rng_type_enemy_generation,
      rng_type_dungeon_generation
    };

};



void rng_server::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(rng_server)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(rng_server)
  ATTR_INTERFACE_HELPER_BIND_GODOT(rng_server)
}


void rng_server::_bind_subautoload(subautoload_container* container){}

void rng_server::_on_gamenode_ready(){
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
    for(rng_types _types: _static->_rng_init_list)
      _rng_type_map[_types] = new RandomNumberGenerator();

    set_seed_random();
  }
}


void rng_server::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _rng_type_map)
        delete _pair.second;
    }
  }
}


int rng_server::get_seed(){
  return _current_seed;
}

void rng_server::set_seed(int seed){
  _current_seed = seed;
  for(auto _pair: _rng_type_map)
    _pair.second->set_seed(_current_seed);
}

void rng_server::set_seed_random(){
  RandomNumberGenerator* _tool_rng = _rng_type_map[TOOL_RNG_TYPE];
  
  _tool_rng->randomize();
  set_seed(_tool_rng->randi());
}


RandomNumberGenerator* rng_server::get_randomizer(rng_types rtype){
  auto _iter = _rng_type_map.find(rtype);
  if(_iter == _rng_type_map.end())
    return NULL;

  return _iter->second;
}