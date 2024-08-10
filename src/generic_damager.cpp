#include "logger.h"

#include "generic_damager.h"

#include "bus_event.h"

#include "damageable_object.h"


using namespace Game;
using namespace Game::Entity;
using namespace Game::Item;
using namespace Game::Modifiers;
using namespace GameUtils::Memory;



generic_damager::generic_damager(bus_event* bus){
  _bus_event = bus;
}


void generic_damager::on_hit(Game::hit_register_pass_data* pass_data){
  if(pass_data->collider->is_class(damageable_object::get_class_static())){
    // apply active modifier first
    _modifier->modify_weapon_stats(&_wstats, pass_data->current_hitreg_state);

    damageable_object* _dmg_obj = (damageable_object*)pass_data->collider;
    damage_info _dmg_info;
    _dmg_info.damage = _wstats.get_damage();
    double _total_damage = _dmg_obj->do_damage(_dmg_info);

    _bus_event->emit_signal(SIGNAL_BUS_EVENT_DAMAGEABLE_DAMAGED, pass_data->collider->get_global_position(), _total_damage);

    pass_data->do_continue = false;
  }
}

void generic_damager::on_hitreg_changed(const hit_register_state* new_state){
  _modifier->modify_weapon_stats(&_wstats, new_state);
}


std::shared_ptr<I_damager> generic_damager::clone_damager(){
  generic_damager* _new_obj = new generic_damager(_bus_event);
  _new_obj->_modifier = _modifier;
  _new_obj->_wstats = _wstats;

  return std::shared_ptr<I_damager>(_new_obj);
}


std::shared_ptr<I_damager> generic_damager::create_from_data(const weapon_stats_data* w_stats, std::shared_ptr<modifier_combination_helper> mod_combination, bus_event* bus){
  generic_damager* _newobj = new generic_damager(bus);
  _newobj->_wstats = *w_stats;
  _newobj->_modifier = mod_combination;

  mod_combination->modify_weapon_stats(&_newobj->_wstats);

  std::shared_ptr<I_damager> _res = std::shared_ptr<I_damager>((I_damager*)_newobj);
  return _res;
}