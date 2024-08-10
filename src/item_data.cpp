#include "item_data.h"


using namespace Game::Item;
using namespace godot;


String item_data::get_item_id() const{
  return _item_id;
}

String item_data::get_item_type() const{
  return _item_type;
}


String item_data::get_item_name() const{
  return _item_name;
}

String item_data::get_item_description() const{
  return _item_desc;
}