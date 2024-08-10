#include "item_object.h"


using namespace Game;
using namespace Game::Item;
using namespace godot;




String item_object::get_item_id() const{
  return _this_item_data->get_item_id();
}


String item_object::get_item_type_str() const{
  return _this_item_data->get_item_type();
}

uint32_t item_object::get_item_type() const{
  return _this_type;
}


const item_data* item_object::get_item_data() const{
  return _this_item_data;
}

const item_resource_data* item_object::get_resource_data() const{
  return _this_item_res;
}

uint64_t item_object::get_id() const{
  return _this_id;
}


const godot::String item_object::get_item_name() const{
  return _this_item_data->get_item_name();
}

const godot::String item_object::get_description() const{
  return _this_item_data->get_item_description();
}