#include "overrider_base.h"


using namespace Game;
using namespace godot;



void overrider_base::_set_overrider_custom_id(String custom_id){
  _custom_id = custom_id;
}


overrider_base::overrider_base(){
  _static_id = this->___overrider_base_get_custom_id_from_static();
  _custom_id = "";
}


String overrider_base::get_overrider_id(){
  if(_custom_id.is_empty())
    return _static_id;

  return _custom_id;
}