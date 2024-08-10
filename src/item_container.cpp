#include "item_container.h"
#include "item_server.h"


using namespace Game;
using namespace Game::Item;
using namespace godot;



void item_container::_emit_signal_index_removed(item_position_data& pos_data, uint64_t item_id){
  item_container_on_index_remove_param _param(pos_data, item_id);
  emit_signal(SIGNAL_ITEM_CONTAINER_ON_INDEX_REMOVED, (uint64_t)&_param);
}

void item_container::_emit_signal_index_added(item_position_data& pos_data, uint64_t item_id){
  item_container_on_index_add_param _add_param(pos_data, item_id);
  emit_signal(SIGNAL_ITEM_CONTAINER_ON_INDEX_ADD, (uint64_t)&_add_param);
}

void item_container::_emit_signal_index_changed(item_position_data& pos_data){
  item_container_on_index_changed_param _changed_param(pos_data);
  emit_signal(SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED, (uint64_t)&_changed_param);
}

void item_container::_emit_signal_container_resized(){
  emit_signal(SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED);
}


bool item_container::_check_index(int index){
  return index >= -1 && index < (int)_item_list.size();
}


int item_container::_get_new_index(){
  if(_item_count >= _item_list.size())
    return -1;

  // TODO optimize this
  for(int i = 0; i < _item_list.size(); i++){
    if(_item_list[i] == ITEM_ID_NULL)
      return i;
  }

  return -1;
}


uint64_t item_container::_remove_item(int index){
  if(!_check_index(index))
    return ITEM_ID_NULL;

  item_position_data _pos_item_data;
  _pos_item_data.container_id = _container_id;
  _pos_item_data.index_position = index;

  uint64_t _id = _item_list[index];
  _item_list[index] = ITEM_ID_NULL;
  _item_count--;

  _emit_signal_index_removed(_pos_item_data, _id);
  _emit_signal_index_changed(_pos_item_data);
  
  return _id;
}

int item_container::_add_item(const item_object* item, int index, bool force, uint64_t* _removed_id){
  INIT_ASSERT()

  LOG_ASSERT("test1")

  Array _paramarr;{
    _paramarr.append(_item_filter_type);
    _paramarr.append(item->get_item_type());
  }
  LOG_ASSERT(String("filter {0}, type {1}").format(_paramarr))

  if(!_check_index(index) || (_item_filter_type & item->get_item_type()) == 0)
    return -1;

  LOG_ASSERT("test2")
  if(index < 0)
    index = this->_get_new_index();

  LOG_ASSERT("test3")
  // if cannot find a new index
  if(index < 0)
    return -1;

  LOG_ASSERT("test4")
  item_inst_id id = item->get_id();
  item_position_data _pos_item_data;
  _pos_item_data.container_id = _container_id;
  _pos_item_data.index_position = index;

  LOG_ASSERT("test5")
  if(_item_list[index] != ITEM_ID_NULL){
    if(force)
      *_removed_id = this->_remove_item(index);
    else
      return -1;
  }
  
  LOG_ASSERT("test6")
  _item_list[index] = id;
  _item_count++;

  _emit_signal_index_added(_pos_item_data, id);
  _emit_signal_index_changed(_pos_item_data);

  return index;
}


void item_container::_bind_methods(){
  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_ON_INDEX_ADD,
    PropertyInfo(Variant::INT, "parameter")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED,
    PropertyInfo(Variant::INT, "parameter")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_ON_INDEX_REMOVED,
    PropertyInfo(Variant::INT, "parameter")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED
  ));
}


item_container::item_container(){
  _item_filter_type = 0xFFFFFFFF;
  _item_count = 0;
}

const item_object* item_container::get_item(int index){
  if(!_check_index(index))
    return NULL;

  return _server->get_item_object(_item_list[index]);
}

uint64_t item_container::get_item_id(int index){
  if(!_check_index(index))
    return ITEM_ID_NULL;

  return _item_list[index];
}


uint64_t item_container::get_container_id(){
  return _container_id;
}


bool item_container::is_item_exist(int index){
  if(!_check_index(index))
    return false;

  return _item_list[index] != ITEM_ID_NULL;
}

bool item_container::is_item_can_exist(const item_object* item){
  if((_item_filter_type & item->get_item_type()) == 0)
    return false;

  return true;
}

bool item_container::is_item_can_exist(uint64_t item_id){
  if(item_id == ITEM_ID_NULL)
    return false;

  const item_object* _curr_item = _server->get_item_object(item_id);
  if(!_curr_item)
    return false;

  return is_item_can_exist(_curr_item);
}


bool item_container::set_item_filter_type(uint32_t new_type){
  _item_filter_type = new_type;
  drop_all_item_filter(~_item_filter_type);

  return true;
}

uint32_t item_container::get_item_filter_type(){
  return _item_filter_type;
}


int item_container::empty_slot_count(){
  return _item_list.size() - _item_count;
}

int item_container::get_container_size(){
  return _item_list.size();
}

void item_container::resize_container(int new_size){
  _item_list.resize(new_size, ITEM_ID_NULL);
  _emit_signal_container_resized();
}


// this only forwarding to item_server
bool item_container::add_to_container(item_inst_id id, int index){
  return _server->add_to_container(_container_id, id, index);
}

// this only forwarding to item_server
item_inst_id item_container::remove_from_container(int index){
  return _server->remove_from_container_index(_container_id, index);
}


// this only forwarding to item_server
bool item_container::swap_item(int index, item_position_data& pos_data){
  item_position_data _this_pos_data;{
    _this_pos_data.container_id = _container_id;
    _this_pos_data.index_position = index;
  }

  return _server->swap_item(_this_pos_data, pos_data);
}


// this only forwarding to item_server
bool item_container::drop_item(int index){
  item_position_data _pos_data;{
    _pos_data.container_id = _container_id;
    _pos_data.index_position = index;
  }

  return _server->drop_item(_pos_data);
}


// this only forwarding to item_server
void item_container::drop_all_item(){
  for(int i = 0; i < _item_list.size(); i++)
    drop_item(i);
}

// this only forwarding to item_server
void item_container::drop_all_item_filter(uint32_t filter){
  for(int i = 0; i < _item_list.size(); i++){
    item_inst_id _curr_id = _item_list[i];
    if(_curr_id == ITEM_ID_NULL)
      continue;

    const item_object* _item = _server->get_item_object(_curr_id);
    if(!_item)
      continue;
    
    if(_item->get_item_type() & filter)
      drop_item(_curr_id);
  }
}