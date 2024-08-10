#include "defines.h"
#include "logger.h"

#include "alternate_data.h"
#include "godot_error.h"
#include "weapon_stats_data.h"

#include "map"
#include "set"


using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;



#define _SUBDATA_PARSER_MAP_HELPER(subdata_name) {subdata_name::get_subdata_name(), &weapon_stats_data::__add_new_subdata<subdata_name>}
void weapon_stats_data::_parse_weapon_stats_dict(Dictionary* data){
  typedef void(weapon_stats_data::*subdata_parser_function)(Dictionary*);
  const std::map<String, subdata_parser_function> _subdata_parser_map = std::map<String, subdata_parser_function>{
    _SUBDATA_PARSER_MAP_HELPER(alternate_data),
    _SUBDATA_PARSER_MAP_HELPER(hitreg_data)
  };


  Array _paramarr;
  Variant _v;

  Dictionary& _data_part = *data;
  std::set<String> _key_left;{
    Array _key_list = _data_part.keys();
    for(int i = 0; i < _key_list.size(); i++)
      _key_left.insert((String)_key_list[i]);
  }

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_DAMAGE, Variant::FLOAT)
  _damage = _v;
  _key_left.erase(VARNAME_WEAPON_STATS_DAMAGE);

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_CHARGE_DELAY, Variant::FLOAT)
  _charge_delay = _v;
  _key_left.erase(VARNAME_WEAPON_STATS_CHARGE_DELAY);

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_FIRE_MODE, Variant::STRING)
  _fire_mode = _parse_fire_mode(_v);
  if(!(_fire_mode & fire_mode::basic_config_filter)){
    _paramarr.clear();{
      _paramarr.append(VARNAME_WEAPON_STATS);
      _paramarr.append(VARNAME_WEAPON_STATS_FIRE_MODE);
    }

    throw gdstr_exception(String("In '{0}': '{1}'; \"basic\" fire mode flag is not set.").format(_paramarr));
  }
  _key_left.erase(VARNAME_WEAPON_STATS_FIRE_MODE);

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_FIRE_RATE, Variant::FLOAT)
  _fire_rate = _v;
  _key_left.erase(VARNAME_WEAPON_STATS_FIRE_RATE);

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_TAP_RATE, Variant::FLOAT)
  _tap_rate = _v;
  _key_left.erase(VARNAME_WEAPON_STATS_TAP_RATE);

  _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, VARNAME_WEAPON_STATS_BURST_COUNT, Variant::FLOAT)
  _burst_count = _v;
  _key_left.erase(VARNAME_WEAPON_STATS_BURST_COUNT);

  for(String _subdata_name: _key_left){
    _paramarr.clear();{
      _paramarr.append(VARNAME_WEAPON_STATS);
      _paramarr.append(_subdata_name);
    }

    auto _iter = _subdata_parser_map.find(_subdata_name);
    if(_iter == _subdata_parser_map.end()){
      throw gdstr_exception(String("In '{0}'; '{1}' is not a valid subdata in this context.").format(_paramarr));
    }

    _CREATE_CHECKER_DICT2(VARNAME_WEAPON_STATS, _subdata_name, Variant::DICTIONARY)
    Dictionary _subdata_data = _v;

    (this->*_iter->second)(&_subdata_data);
  }
}


weapon_stats_data::fire_mode weapon_stats_data::_parse_fire_mode(String mode){
  const std::map<String, weapon_stats_data::fire_mode> _fire_mode_map = {
    {VARNAME_WEAPON_STATS_FIRE_MODE_AUTOMATIC, weapon_stats_data::automatic},
    {VARNAME_WEAPON_STATS_FIRE_MODE_BURST, weapon_stats_data::burst_fire},
    {VARNAME_WEAPON_STATS_FIRE_MODE_SINGLE, weapon_stats_data::single_fire},
    {VARNAME_WEAPON_STATS_FIRE_MODE_CHARGE, weapon_stats_data::charge},
    {VARNAME_WEAPON_STATS_FIRE_MODE_FULL_EXHAUST, weapon_stats_data::full_exhaust}
  };


  fire_mode _new_fire_mode = (fire_mode)0;
  PackedStringArray _array_string = mode.split(",");
  for(int i = 0; i < _array_string.size(); i++){
    String _flag = _array_string[i].replace(" ", ""); _flag = _flag.to_lower();
    auto _flag_iter = _fire_mode_map.find(_flag);
    if(_flag_iter == _fire_mode_map.end())
      continue;

    if(_flag_iter->second & fire_mode::basic_config_filter)
      _new_fire_mode = (fire_mode)((_new_fire_mode & ~fire_mode::basic_config_filter) | _flag_iter->second);
    else
      _new_fire_mode = (fire_mode)(_flag_iter->second | _new_fire_mode);
  }

  return _new_fire_mode;
}


weapon_stats_data::weapon_stats_data(){
  
}


godot::String weapon_stats_data::get_subdata_name(){
  return VARNAME_WEAPON_STATS;
}

void weapon_stats_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  mem->change_data<weapon_stats_data>();

  weapon_stats_data* _stats = mem->get_data<weapon_stats_data>();
  _stats->_parse_weapon_stats_dict(data);
}


void weapon_stats_data::copy_subdata_to(custom_memory* mem) const{
  mem->change_data<weapon_stats_data>();
  weapon_stats_data* _stats = mem->get_data<weapon_stats_data>();
  
  _stats->_damage = _damage;
  __copy_subdata_to_subdata(_stats);
}


double weapon_stats_data::get_damage() const{
  return _damage;
}

void weapon_stats_data::set_damage(double damage){
  _damage = damage;
}


double weapon_stats_data::get_charge_delay() const{
  return _charge_delay;
}

void weapon_stats_data::set_charge_delay(double delay){
  _charge_delay = delay;
}


weapon_stats_data::fire_mode weapon_stats_data::get_fire_mode() const{
  return _fire_mode;
}

void weapon_stats_data::set_fire_mode(weapon_stats_data::fire_mode mode){
  _fire_mode = mode;
}


double weapon_stats_data::get_fire_rate() const{
  return _fire_rate;
}

void weapon_stats_data::set_fire_rate(double rate){
  _fire_rate = rate;
}


double weapon_stats_data::get_tap_rate() const{
  return _tap_rate;
}

void weapon_stats_data::set_tap_rate(double rate){
  _tap_rate = rate;
}


uint64_t weapon_stats_data::get_burst_count() const{
  return _burst_count;
}

void weapon_stats_data::set_burst_count(uint64_t count){
  _burst_count = count;
}