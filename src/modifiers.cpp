#include "defines.h"
#include "godot_error.h"
#include "I_modifier_database_helper.h"
#include "logger.h"
#include "modifiers.h"
#include "modifier_factory.h"


using namespace Game;
using namespace Game::Error;
using namespace Game::Modifiers;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace GameUtils::Memory;


class damage_additive_modifier_data: public custom_data{
  public:
    double range_low;
    double range_high;
};


MODDB_HELPER_ADD_STATIC_JSON_PARSER_DEFINE(damage_normal_additive_modifier){
  INIT_ASSERT()
  Array _paramarr;
  Variant _v;

  std::shared_ptr<custom_memory> _res = custom_memory::create_shared<damage_additive_modifier_data>();
  damage_additive_modifier_data* _data = _res->get_data<damage_additive_modifier_data>();
  ZeroMemory(_data, sizeof(damage_additive_modifier_data));

  try{
    Dictionary& _data_part = *json_data;

    _CREATE_CHECKER_DICT2(VARNAME_MODIFIER_DATA_MODIFIER_DATA, "range_low", Variant::FLOAT)
    _data->range_low = _v;

    _CREATE_CHECKER_DICT2(VARNAME_MODIFIER_DATA_MODIFIER_DATA, "range_high", Variant::FLOAT)
    _data->range_high = _v;
  }
  catch(gdstr_exception e){
    ERR_ASSERT(String(e.what()))
  }

  return _res;
}


void damage_normal_additive_modifier::modify_weapon_stats(weapon_stats_data* stats, const hit_register_state* hitreg_state) const{
  stats->set_damage(stats->get_damage() + _addition);
}


void damage_normal_additive_modifier::randomize_modifier(RandomNumberGenerator* rng, std::shared_ptr<custom_memory> memory){
  damage_additive_modifier_data* _data = memory->get_data<damage_additive_modifier_data>();

  _addition = rng->randf_range(_data->range_low, _data->range_high);
}


String damage_normal_additive_modifier::get_modifier_title() const{
  return "Damage Addition";
}

String damage_normal_additive_modifier::get_modifier_description() const{
  Array _paramarr;{
    _paramarr.append(String::num(_addition, 1));
  }

  return String("+{0} Dmg").format(_paramarr);
}




MODDB_HELPER_ADD_STATIC_JSON_PARSER_DEFINE(damage_percentage_additive_modifier){
  return damage_normal_additive_modifier::parse_json_data_static(json_data);
}


void damage_percentage_additive_modifier::modify_weapon_stats(weapon_stats_data* stats, const hit_register_state* hitreg_state) const{
  stats->set_damage(stats->get_damage() * ((_percentage/100) + 1));
}


void damage_percentage_additive_modifier::randomize_modifier(RandomNumberGenerator* rng, std::shared_ptr<custom_memory> memory){
  damage_additive_modifier_data* _data = memory->get_data<damage_additive_modifier_data>();

  _percentage = rng->randf_range(_data->range_low, _data->range_high);
}


String damage_percentage_additive_modifier::get_modifier_title() const{
  return "Damage Addition";
}

String damage_percentage_additive_modifier::get_modifier_description() const{
  Array _paramarr;{
    _paramarr.append(String::num(_percentage, 2));
  }

  return String("+{0}% Dmg").format(_paramarr);
}