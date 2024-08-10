#include "hit_register_data_container.h"


using namespace Game;
using namespace godot;


void hit_register_data_container::_clear_data(hit_register_data* _data_start){
  for(int i = 0; i < _data_start->_next.size(); i++)
    _clear_data(_data_start->_next[i]);

  _data_start->_next.clear();
  delete _data_start;
}


hit_register_data_container::hit_register_data_container(hit_register_data& start_data){
  _data_head = new hit_register_data();
  start_data.copy_to(_data_head);
}

hit_register_data_container::~hit_register_data_container(){
  _clear_data(_data_head);
  delete _data_head;
}


hit_register_data* hit_register_data_container::get_head(){
  return _data_head;
}

hit_register_data* hit_register_data_container::add_node(hit_register_data* from, hit_register_data* data){
  hit_register_data* _newdata = new hit_register_data();
  data->copy_to(_newdata);

  from->_next.insert(from->_next.end(), _newdata);
  return _newdata;
}

std::shared_ptr<hit_register_data_container> hit_register_data_container::create_by_json(Dictionary* json_data){
  return std::shared_ptr<hit_register_data_container>(NULL);
}