#include "damageable_object.h"
#include "godot_var_extended.h"
#include "logger.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace Game::Entity;
using namespace GameUtils::Memory;
using namespace godot;




//      damageable_object defines
ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(damageable_object)
ATTR_BASE_INTERFACE_HELPER_DEFINE(damageable_object)
ATTR_INTERFACE_HELPER_DEFINE(damageable_object)


void damageable_object::_check_health(){
  if(_vital_data->current_hp < 0){
    _vital_data->current_hp = 0;
    this->_on_dead();

    emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_DEAD, this);
  }
  else if(_vital_data->current_hp > _vital_data->max_hp)
    _vital_data->current_hp = _vital_data->max_hp;
}


void damageable_object::_on_cooldown_ep_timeout(){
  _do_restore_energy = true;
}


void damageable_object::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(damageable_object)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(damageable_object);
  ATTR_INTERFACE_HELPER_BIND_GODOT(damageable_object);


  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_HP_CHANGED,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_HEALED,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_DAMAGED,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));
  
  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_DEAD,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));
  
  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_REVIVED,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_DAMAGEABLE_OBJECT_ON_EP_CHANGED,
    PropertyInfo(Variant::OBJECT, "dmgable_obj")
  ));


  ClassDB::bind_method(D_METHOD("_on_cooldown_ep_timeout"), &damageable_object::_on_cooldown_ep_timeout);
}


void damageable_object::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    // TODO initialize subdata for entity_object

    _cooldown_ep_timer = new Timer();
    _cooldown_ep_timer->connect("timeout", Callable(this, "_on_cooldown_ep_timeout"));
    add_child_sa(_cooldown_ep_timer);
  }
}


void damageable_object::_set_max_hp(double max_hp, bool restore_hp){
  _vital_data->max_hp = max_hp;
  if(restore_hp)
    _set_health_point(max_hp);
}

void damageable_object::_set_max_ep(double max_ep, bool restore_ep){
  _vital_data->max_ep = max_ep;
  if(restore_ep)
    _set_energy_point(max_ep);
}


void damageable_object::_set_health_point(double new_hp, bool capped){
  _vital_data->current_hp = new_hp;
  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_HP_CHANGED, GDVAR_CUSTOM_FROM_PTR(this));
}

void damageable_object::_set_energy_point(double new_ep, bool capped){
  if(_vital_data->current_ep > new_ep){
    _do_restore_energy = false;
    _cooldown_ep_timer->start(_vital_data->restore_delay_ep);
  }

  _vital_data->current_ep = new_ep;
  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_EP_CHANGED, GDVAR_CUSTOM_FROM_PTR(this));
}


void damageable_object::_set_restore_rate_ep(double restore_rate){
  _vital_data->restore_rate_ep = restore_rate;
}

void damageable_object::_set_restore_delay_ep(double restore_delay){
  double _time_cooldown_left = _cooldown_ep_timer->get_time_left();
  if(_time_cooldown_left > 0){
    double _ratio = _time_cooldown_left / _vital_data->restore_delay_ep;
    double _new_time = restore_delay * _ratio;

    _cooldown_ep_timer->start(_new_time);
  }

  _vital_data->restore_delay_ep = restore_delay;
}


damageable_object::damageable_object(){
  _cooldown_ep_timer = NULL;
}


void damageable_object::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_do_restore_energy && _vital_data->current_ep < _vital_data->max_ep){
      _vital_data->current_ep += _vital_data->restore_rate_ep * delta;
      if(_vital_data->current_ep > _vital_data->max_ep)
        _vital_data->current_ep = _vital_data->max_ep;
    }
  }
}


void damageable_object::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_cooldown_ep_timer)
        delete _cooldown_ep_timer;
    }
  }
}


double damageable_object::do_damage(const damage_info& d_info){
  damage_info _new_d_info = d_info;
  this->_on_damaged(_new_d_info);
  this->_on_hp_changed();
  
  double _total_damage = _new_d_info.damage;
  _vital_data->current_hp -= _total_damage;

  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append(_vital_data->current_hp);
  }

  LOG_ASSERT(String("hp {0}").format(_paramarr))

  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_DAMAGED, this);
  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_HP_CHANGED, this);
  _check_health();

  _on_vital_data_changed();
  return _total_damage;
}

double damageable_object::do_heal(const heal_info& h_info){
  heal_info _new_h_info = h_info;
  this->_on_healed(_new_h_info);
  this->_on_hp_changed();

  double _total_heal = _new_h_info.heal_amount;
  double _new_hp = _vital_data->current_hp + _total_heal;

  _set_health_point(_new_hp);
  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_HEALED, this);
  return _total_heal;
}

double damageable_object::do_revive(const revive_info& r_info){
  revive_info _new_r_info = r_info;
  _new_r_info.full_revive = true;

  this->_on_revived(_new_r_info);
  if(!_new_r_info.full_revive){
    if(_new_r_info.hp > _vital_data->max_hp)
      _new_r_info.hp = _vital_data->max_hp;

    _vital_data->max_hp = _new_r_info.hp;
  }
  else{
    _vital_data->current_hp = _vital_data->max_hp;
  }

  emit_signal(SIGNAL_DAMAGEABLE_OBJECT_ON_REVIVED, this);
  return _vital_data->max_hp;
}


bool damageable_object::use_energy(const double e_point){
  double _new_ep = _vital_data->current_ep - e_point;
  if(_new_ep < 0)
    return false;

  _set_energy_point(_new_ep);
  return true;
}

double damageable_object::get_energy_point(){
  return _vital_data->current_ep;
}


const vital_data* damageable_object::get_vital_data(){
  return _vital_data;
}




//      vital_data defines
String vital_data::get_subdata_name(){
  return "vital_data";
}

void vital_data::copy_subdata_to(custom_memory* mem){
  mem->change_data<vital_data>();

  vital_data* _data = mem->get_data<vital_data>();
  *_data = *this;
}


double vital_data::get_max_hp() const{
  return max_hp;
}

double vital_data::get_current_hp() const{
  return current_hp;
}


double vital_data::get_max_ep() const{
  return max_ep;
}

double vital_data::get_current_ep() const{
  return current_ep;
}