#include "item_dropper.h"
#include "item_world_entity_2d_handler.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::Item;
using namespace godot;


void item_dropper::_bind_methods(){

}


item_dropper::item_dropper(){
  _iwe2d_handler = NULL;
  _last_pos = Vector2(0,0);
}


void item_dropper::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_iwe2d_handler)
        _iwe2d_handler->delete_item_dropper(_dropper_id);
    }
  }
}


void item_dropper::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    Vector2 _current_pos = get_global_position();

    _passive_speed = (_current_pos - _last_pos)/delta;
    _last_pos = _current_pos;
  }
}


item_dropper_inst_id item_dropper::get_id(){
  return _dropper_id;
}

Vector2 item_dropper::get_speed(){
  return _passive_speed;
}