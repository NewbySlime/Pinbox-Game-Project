#include "defines.h"
#include "godot_error.h"
#include "vital_info.h"

#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"


using namespace Game::Entity;
using namespace GameUtils::Memory;
using namespace godot;



void vital_info::_parse_data(const Dictionary& _data_part){
  Array _paramarr;
  Variant _v;

  _CREATE_CHECKER_DICT2(VARNAME_VITAL_INFO, VARNAME_VITAL_INFO_MAX_HEALTH, Variant::FLOAT)
  _max_health_point = _v;
  
  _CREATE_CHECKER_DICT2(VARNAME_VITAL_INFO, VARNAME_VITAL_INFO_MAX_ENERGY, Variant::FLOAT)
  _max_energy_point = _v;

  _CREATE_CHECKER_DICT2(VARNAME_VITAL_INFO, VARNAME_VITAL_INFO_EP_RESTORE_DELAY, Variant::FLOAT)
  _ep_restore_delay = _v;

  _CREATE_CHECKER_DICT2(VARNAME_VITAL_INFO, VARNAME_VITAL_INFO_EP_RESTORE_RATE, Variant::FLOAT)
  _ep_restore_rate = _v;
}


std::string vital_info::get_subdata_name_stdstr(){
  return VARNAME_VITAL_INFO;
}

String vital_info::get_subdata_name(){
  return VARNAME_VITAL_INFO;
}

void vital_info::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<vital_info>();
  vital_info* _this = mem->get_data<vital_info>();

  _this->_parse_data(*data);
}


void vital_info::copy_subdata_to(custom_memory* mem){
  mem->change_data<vital_info>();
  vital_info* _target = mem->get_data<vital_info>();

  _target->_max_health_point = _max_health_point;
  _target->_max_energy_point = _max_energy_point;
  _target->_ep_restore_delay = _ep_restore_delay;
  _target->_ep_restore_rate = _ep_restore_rate;
}



float vital_info::get_max_hp(){
  return _max_health_point;
}


float vital_info::get_max_ep(){
  return _max_energy_point;
}

float vital_info::get_ep_restore_delay(){
  return _ep_restore_delay;
}

float vital_info::get_ep_restore_rate(){
  return _ep_restore_rate;
}