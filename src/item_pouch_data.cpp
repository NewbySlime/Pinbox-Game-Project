#include "defines.h"
#include "item_pouch_data.h"
#include "godot_error.h"

#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"


using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


void item_pouch_data::_parse_item_pouch_data(Dictionary* data){
  Array _paramarr;
  Variant _v;

  Dictionary& _data_part = *data;
  _CREATE_CHECKER_DICT2(VARNAME_ITEM_POUCH_DATA, VARNAME_ITEM_POUCH_DATA_SIZE, Variant::FLOAT)
  _pouch_size = _v;

  _CREATE_CHECKER_DICT2(VARNAME_ITEM_POUCH_DATA, VARNAME_ITEM_POUCH_DATA_TYPE, Variant::INT)
  _container_type = _v;

  _CREATE_CHECKER_DICT2(VARNAME_ITEM_POUCH_DATA, VARNAME_ITEM_POUCH_DATA_FILTER, Variant::INT)
  _item_filter = _v;
}


String item_pouch_data::get_subdata_name(){
  return VARNAME_ITEM_POUCH_DATA;
}


void item_pouch_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<item_pouch_data>();
  item_pouch_data* _data = mem->get_data<item_pouch_data>();

  _data->_parse_item_pouch_data(data);
}


void item_pouch_data::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<item_pouch_data>();
  item_pouch_data* _data = mem->get_data<item_pouch_data>();
  
  _data->_pouch_size = _pouch_size;
  _data->_container_type = _container_type;
  _data->_item_filter = _item_filter;
}


uint64_t item_pouch_data::get_pouch_size() const{
  return _pouch_size;
}

uint32_t item_pouch_data::get_container_type() const{
  return _container_type;
}

uint32_t item_pouch_data::get_item_filter() const{
  return _item_filter;
}