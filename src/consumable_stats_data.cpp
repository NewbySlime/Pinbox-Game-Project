#include "defines.h"
#include "logger.h"

#include "consumable_stats_data.h"
#include "godot_error.h"

#include "godot_cpp/variant/array.hpp"


using namespace Game;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;


void consumable_stats_data::_parse_consumable_stats_dict(Dictionary* data){
  INIT_ASSERT()
  Array _paramarr;
  Variant _v;

  Dictionary& _data_part = *data;
  _CREATE_CHECKER_DICT2(VARNAME_CONSUMABLE_STATS, VARNAME_CONSUMABLE_STATS_HEAL, Variant::FLOAT)
  _heal = _v;

  _CREATE_CHECKER_DICT2(VARNAME_CONSUMABLE_STATS, VARNAME_CONSUMABLE_STATS_IS_THROWABLE, Variant::BOOL)
  _is_throwable = _v;
}


String consumable_stats_data::get_subdata_name(){
  return "consumable_stats";
}

void consumable_stats_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<consumable_stats_data>();

  consumable_stats_data* _stats = mem->get_data<consumable_stats_data>();
  _stats->_parse_consumable_stats_dict(data);
}


void consumable_stats_data::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<consumable_stats_data>();

  consumable_stats_data* _stats = mem->get_data<consumable_stats_data>();
  _stats->_heal = _heal;
  _stats->_is_throwable = _is_throwable;
}


double consumable_stats_data::get_heal(){
  return _heal;
}