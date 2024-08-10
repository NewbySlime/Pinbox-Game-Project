#include "item_container_infinite.h"


using namespace Game;
using namespace Game::Item;
using namespace godot;



void item_container_infinite::_bind_methods(){

}


int item_container_infinite::_get_new_index(){
  return _item_list.size();
}


uint64_t item_container_infinite::_remove_item(int index){
  if(!this->_check_index(index))
    return ITEM_ID_NULL;

  item_position_data _pos_item_data;
  _pos_item_data.container_id = _container_id;
  _pos_item_data.index_position = index;

  uint64_t _removed_id = _item_list[index];

  int _moved_index_pos = _item_list.size()-1;
  uint64_t _moved_id = ITEM_ID_NULL;
  if(_item_list.size() > 1 && index >= (_moved_index_pos))
    _moved_id = _item_list[_moved_index_pos];

  this->resize_container(_moved_index_pos);

  _emit_signal_index_removed(_pos_item_data, _removed_id);
  _emit_signal_index_changed(_pos_item_data);

  if(_moved_id != ITEM_ID_NULL){
    item_position_data _moved_pos_item_data;
    _moved_pos_item_data.container_id = _container_id;
    _moved_pos_item_data.index_position = _moved_index_pos;

    _item_list[index] = _moved_id;
    
    _emit_signal_index_removed(_moved_pos_item_data, _moved_id);
    _emit_signal_index_changed(_moved_pos_item_data);

    _emit_signal_index_added(_pos_item_data, _moved_id);
    _emit_signal_index_changed(_pos_item_data);
  }

  return _removed_id;
}

int item_container_infinite::_add_item(uint64_t id, int index, bool force, uint64_t* _removed_id){
  index = this->_get_new_index();

  item_position_data _pos_item_data;
  _pos_item_data.container_id = _container_id;
  _pos_item_data.index_position = index;

  _item_list.insert(_item_list.end(), id);

  _emit_signal_index_added(_pos_item_data, id);
  _emit_signal_index_changed(_pos_item_data);

  return index;
}


int item_container_infinite::get_container_size(){
  return INT_MAX;
}

void item_container_infinite::resize_container(int new_size){
  // do nothing since it's "infinite"
}