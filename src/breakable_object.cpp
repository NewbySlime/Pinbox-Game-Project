#include "logger.h"
#include "breakable_object.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::Entity;
using namespace godot;


void breakable_object::_bind_methods(){
  GD_PROPERTY_BIND_HINT(breakable_object, Variant::STRING, object_id, PropertyHint::PROPERTY_HINT_ENUM)
}



void breakable_object::_on_damaged(damage_info& d_info){
  INIT_ASSERT()

  const vital_data* current_vital_data = get_vital_data();

  Array _paramarr;{
    _paramarr.append(d_info.damage);
    _paramarr.append(current_vital_data->get_current_hp());
    _paramarr.append(current_vital_data->get_max_hp());
  }
  LOG_ASSERT(String("Getting hit! Damage: {0}, {1}/{2}"));
}

void breakable_object::_on_dead(){
  //on_revived();
}


breakable_object::breakable_object(){

}


void breakable_object::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    
  }
}