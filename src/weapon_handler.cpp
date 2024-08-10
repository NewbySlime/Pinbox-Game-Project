#include "logger.h"
#include "weapon_handler.h"
#include "modifier_factory.h"

#include "generic_damager.h"
#include "godot_macro.h"
#include "weapon_stats_data.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/scene_tree_timer.hpp"


using namespace Game;
using namespace Game::Modifiers;
using namespace Game::Item;
using namespace Game::Item::Factory;
using namespace GameUtils::Memory;
using namespace godot;



ATTR_INTERFACE_HELPER_DEFINE(weapon_handler)


class _weapon_action_helper: public I_action_class, public custom_data{
  private:
    weapon_handler* _handler;
    bool _is_secondary;

  public:
    _weapon_action_helper(weapon_handler* handler, bool secondary){
      _handler = handler;
      _is_secondary = secondary;
    }

    ~_weapon_action_helper(){}

    
    void on_action_priming() override{
      _handler->on_action_priming(_is_secondary, false);
    }

    void on_action_continuing() override{
      _handler->on_action_continuing(_is_secondary, false);
    }

    void on_action_cancelled() override{
      _handler->on_action_cancelled(_is_secondary, false);
    }


    bool is_alternate_viable() override{
      return _handler->is_alternate_viable(_is_secondary);
    }

    void on_alternate_action_priming() override{
      _handler->on_action_priming(_is_secondary, true);
    }

    void on_alternate_action_continuing() override{
      _handler->on_action_continuing(_is_secondary, true);
    }

    void on_alternate_action_cancelled() override{
      _handler->on_action_cancelled(_is_secondary, true);
    }


    void on_update(const action_class_update_data& update_data) override{
      _handler->on_update(update_data);
    }
};




hit_register_start_data weapon_handler::_get_hitreg_start_data(){
  hit_register_start_data _res;
  godot::Vector2 length_vec = _target_pos - _global_pos;

  _res.position = _global_pos;
  _res.dir = length_vec.normalized();
  _res.length = length_vec.length();
  _res.is_from_player = true;

  return _res;
}


void weapon_handler::_on_bind_container_item_changed(uint64_t _param){
  _reconstruct_modifier_combination();
  _recalculate_damager();
}

void weapon_handler::_on_bind_container_item_resized(){
  _reconstruct_modifier_combination();
  _recalculate_damager();
}


void weapon_handler::_bind_container_connect_signals(Node* _container){
  _container->connect(SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED, Callable(this, "_on_bind_container_item_changed"));
  _container->connect(SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED, Callable(this, "_on_bind_container_item_resized"));
}

void weapon_handler::_bind_container_disconnect_signals(Node* _container){
  _container->disconnect(SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED, Callable(this, "_on_bind_container_item_changed"));
  _container->disconnect(SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED, Callable(this, "_on_bind_container_item_resized"));
}


void weapon_handler::_bind_item_pouch(const item_pouch* _pouch){
  if(_curr_pouch)
    _bind_container_disconnect_signals(_curr_pouch->get_pouch_node());

  if(_pouch)
    _bind_container_connect_signals(_pouch->get_pouch_node());

  _curr_pouch = _pouch;
  _reconstruct_modifier_combination();
  _recalculate_damager();
}


void weapon_handler::_set_stats_metadata(weapon_item::stats_type type, bool is_alternate, const weapon_item* item){
  _stats_metadata* _metadata;
  if(is_alternate)
    _metadata = _damager_alternate_metadata[type];
  else
    _metadata = _damager_metadata[type];

  _metadata->reset_metadata();
  std::shared_ptr<const custom_memory> _stats_data_cm = item->get_stats_shared(type);
  const weapon_stats_data* _stats_data = item->get_stats(type);
  if(is_alternate){
    _stats_alternate_metadata* _alt_metadata = _damager_alternate_metadata[type];
    const alternate_data* _alt_data = _stats_data->get_subdata<alternate_data>();
    if(!_alt_data)
      goto skip_alt_set;

    _alt_metadata->_alternate_data = _alt_data;
    _alt_metadata->_cd_alternate_data = _stats_data->get_subdata_shared<alternate_data>();

    const weapon_stats_data* _alt_stats_data = _alt_data->get_subdata<weapon_stats_data>();
    if(!_alt_stats_data)
      goto skip_alt_set;

    _stats_data = _alt_stats_data;
    _stats_data_cm = _alt_data->get_subdata_shared<weapon_stats_data>();

    skip_alt_set:{}
  }

  if(!_stats_data)
    return;

  _metadata->_cd_weapon_stats = _stats_data_cm;
  _metadata->_weapon_stats = _stats_data;

  const hitreg_data* _hitreg = _stats_data->get_subdata<hitreg_data>();
  _metadata->_hitreg_data.reset();
  if(_hitreg)
    _metadata->_hitreg_data = _hitreg->get_hitreg_graph();

  // don't recalculate damager yet, no other part of source code that uses this function seperately
}


void weapon_handler::_reconstruct_modifier_combination(){
  _modifier_combination = std::shared_ptr<modifier_combination_helper>(new modifier_combination_helper(_curr_pouch->get_pouch()));
}

void weapon_handler::_recalculate_damager(){
  for(auto _pair: _damager_metadata){
    _stats_metadata* _metadata = _pair.second;
    _metadata->_damager.reset();
    if(_metadata->_weapon_stats == NULL)
      continue;

    _metadata->_damager = generic_damager::create_from_data(_metadata->_weapon_stats, _modifier_combination, _bus_event);
  }

  for(auto _pair: _damager_alternate_metadata){
    _stats_metadata* _metadata = _pair.second;
    _metadata->_damager.reset();
    if(_metadata == NULL || _metadata->_weapon_stats == NULL)
      continue;

    _metadata->_damager = generic_damager::create_from_data(_metadata->_weapon_stats, _modifier_combination, _bus_event);
  }
}


void weapon_handler::_create_hitreg(hit_register_start_data* _start, _stats_metadata* _metadata){
  if(!_metadata->_hitreg_data.get() || !_metadata->_damager)
    return;

  _hitreg_handler->cast_hit_reg(_metadata->_hitreg_data, _metadata->_damager, _start); 
}


void weapon_handler::_bind_methods(){
  ATTR_INTERFACE_HELPER_BIND_GODOT(weapon_handler);

  ClassDB::bind_method(
    D_METHOD("_on_bind_container_item_changed",
      "param"
    ),

    &weapon_handler::_on_bind_container_item_changed
  );

  ClassDB::bind_method(
    D_METHOD("_on_bind_container_item_resized"),
    &weapon_handler::_on_bind_container_item_resized
  );

}


void weapon_handler::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(hit_register_handler, sa_container, _hitreg_handler)
    GET_SUBAUTOLOAD(bus_event, sa_container, _bus_event)
  }
}

void weapon_handler::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){

  }
}


void weapon_handler::_on_item_set(const item_object* item){
  const weapon_item* _item = item->get_subdata<weapon_item>();
  _curr_wdata = _item;

  std::shared_ptr<const custom_memory> _cd_pouch = item->get_subdata_shared<item_pouch>();
  const item_pouch* _pouch = item->get_subdata<item_pouch>();
  _bind_item_pouch(_pouch);
  _cd_curr_pouch = _cd_pouch;


  // setting _stats_metadata
  _set_stats_metadata(weapon_item::stats_type_primary, false, _item);
  _set_stats_metadata(weapon_item::stats_type_secondary, false, _item);
  _set_stats_metadata(weapon_item::stats_type_primary, true, _item);
  _set_stats_metadata(weapon_item::stats_type_secondary, true, _item);

  _recalculate_damager();
}


weapon_handler::weapon_handler(){
  _damager_metadata = {
    {weapon_item::stats_type_primary, new _stats_metadata()},
    {weapon_item::stats_type_secondary, new _stats_metadata()}
  };

  _damager_alternate_metadata = {
    {weapon_item::stats_type_primary, new _stats_alternate_metadata()},
    {weapon_item::stats_type_secondary, new _stats_alternate_metadata()}
  };

  _curr_wdata = NULL;
  _curr_pouch = NULL;
}


void weapon_handler::_draw(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    draw_line(_global_pos - get_global_position(), _target_pos - get_global_position(), Color(1, 0.2, 0.2), 10);
  }
}

void weapon_handler::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_global_rotation_degrees(0);

    hit_register_start_data _start_data = _get_hitreg_start_data();
    
    auto _check_reset_function = [](_stats_metadata* metadata){
      if(!metadata->_weapon_stats)
        return;

      if(metadata->fr_ms_left < 0)
        metadata->fr_ms_left = 0;

      if(metadata->tr_ms_left < 0)
        metadata->tr_ms_left = 0;

      if(metadata->charge_left < 0)
        metadata->charge_left = 0;
    };

    auto _delay_decrement_function = [](double delta, _stats_metadata* metadata){
      if(!metadata->_weapon_stats)
        return;

      if(metadata->fr_ms_left > 0)
        metadata->fr_ms_left -= delta;

      if(metadata->tr_ms_left > 0)
        metadata->tr_ms_left -= delta;

      if(metadata->charge_left > 0)
        metadata->charge_left -= delta;
    };

    // only check recurring fire modes (like burst or automatic)
    auto _check_stats_function = [&_start_data](_stats_metadata* metadata, weapon_handler* _this){
      if(!metadata->_weapon_stats)
        return;

      // TODO check energy, then return

      switch(metadata->_weapon_stats->get_fire_mode() & weapon_stats_data::basic_config_filter){
        break; case weapon_stats_data::burst_fire:{
          if(metadata->fr_ms_left > 0)
            break;

          // keep shooting when fire rate is double than tick rate
          while(metadata->fr_ms_left <= 0){
            if(metadata->burst_iteration_left <= 0){
              if(metadata->tr_ms_left > 0 || !metadata->fire_until_exhausted)
                break;

              metadata->burst_iteration_left = metadata->_weapon_stats->get_burst_count();
              metadata->tr_ms_left += metadata->tr_ms_delay;
            }

            metadata->fr_ms_left += metadata->fr_ms_delay;
            _this->_create_hitreg(&_start_data, metadata);
            metadata->burst_iteration_left--;

            // TODO check energy, then set data based on the fire mode
          }
        }

        break; case weapon_stats_data::automatic:{
          if(metadata->fr_ms_left > 0 || (!metadata->do_fire && !metadata->fire_until_exhausted))
            break;

          // keep shooting when fire rate is double than tick rate
          while(metadata->fr_ms_left <= 0){
            metadata->fr_ms_left += metadata->fr_ms_delay;
            _this->_create_hitreg(&_start_data, metadata);
            
            // TODO check energy, then set data based on the fire mode
          }
        }
      }
    };


    // set delay in weapon metadata (main)
    for(auto _pair: _damager_metadata){
      // reset first, in case if the fire rate still in negative even from last tick of physics process
      _check_reset_function(_pair.second);

      // then decrement the fire rate time
      _delay_decrement_function(delta, _pair.second);

      // check the metadata (fire rate and stuff) then shoot if it's okay to shoot
      _check_stats_function(_pair.second, this);
    }

    // set delay in weapon metadata (alternative)
    for(auto _pair: _damager_alternate_metadata){
      // check previous for loop for comment
      _check_reset_function(_pair.second);
      _delay_decrement_function(delta, _pair.second);
      _check_stats_function(_pair.second, this);
    }
  }
}


void weapon_handler::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _damager_metadata)
        delete _pair.second;

      for(auto _pair: _damager_alternate_metadata)
        delete _pair.second;
    }
  }
}


void weapon_handler::on_action_priming(bool is_secondary, bool is_alternate){
  hit_register_start_data _start_data = _get_hitreg_start_data();
  _stats_metadata* _metadata;

  if(is_alternate){
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_secondary;
    _metadata = _damager_alternate_metadata[_type];
  }
  else{
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_secondary;
    _metadata = _damager_metadata[_type];
  }

  // don't check for energy consumption, here's an analogy
  // player already trigger the weapon, but the weapon cannot fire due to no energy
  // basically a dummy delay, without the weapon shooting

  // check if the weapon still firing or not
  if(_metadata->do_fire || _metadata->fire_until_exhausted || _metadata->burst_iteration_left > 0)
    return;

  if(_metadata->tr_ms_left > 0)
    return;

  uint64_t _full_fm = _metadata->_weapon_stats->get_fire_mode();
  uint64_t _additional_fm = _full_fm & weapon_stats_data::additional_config_filter;
  uint64_t _basic_fm = _full_fm & weapon_stats_data::basic_config_filter;

  if(_additional_fm & weapon_stats_data::charge){
    _metadata->charge_left = _metadata->_weapon_stats->get_charge_delay();
    
    // return so the tap rate will not be accounted and not firing the weapon
    // but will be accounted after continuing (releasing)
    return;
  }
  // this will not trigger when charge fire_mode is available 
  else if(_additional_fm & weapon_stats_data::full_exhaust){
    _metadata->fire_until_exhausted = true;
    // don't set do_fire, that will be decided by the next following code
  }

  // apply to all basic fire_mode, it will be used when calculating in every tick
  _metadata->tr_ms_left += _metadata->tr_ms_delay;
  _metadata->fr_ms_left += _metadata->fr_ms_delay;
  switch(_basic_fm){
    break; case weapon_stats_data::single_fire:
      // do nothing, only shoot once
    
    break; case weapon_stats_data::burst_fire:
      // set with reduced -1, since it will be shot once primed
      _metadata->burst_iteration_left = _metadata->_weapon_stats->get_burst_count()-1;

    break; case weapon_stats_data::automatic:
      _metadata->do_fire = true;
  }

  _create_hitreg(&_start_data, _metadata);
}

void weapon_handler::on_action_continuing(bool is_secondary, bool is_alternate){
  hit_register_start_data _start_data = _get_hitreg_start_data();
  _stats_metadata* _metadata;

  if(is_alternate){
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_primary;
    _metadata = _damager_alternate_metadata[_type];
  }
  else{
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_primary;
    _metadata = _damager_metadata[_type];
  }

  uint64_t _full_fm = _metadata->_weapon_stats->get_fire_mode();
  uint64_t _additional_fm = _full_fm & weapon_stats_data::additional_config_filter;
  uint64_t _basic_fm = _full_fm & weapon_stats_data::basic_config_filter;

  if(_additional_fm & weapon_stats_data::charge){
    // give delay to tap_rate first, even if the charge is not ready
    _metadata->tr_ms_left += _metadata->tr_ms_delay;
    if(_metadata->charge_left > 0)
      return;

    if(_additional_fm & weapon_stats_data::full_exhaust)
      _metadata->fire_until_exhausted = true;

    switch(_basic_fm){
      break; case weapon_stats_data::single_fire:
        // do nothing, only shoot once

      break; case weapon_stats_data::burst_fire:
        // set with reduced -1, since it will be shot once primed
        _metadata->burst_iteration_left = _metadata->_weapon_stats->get_burst_count()-1;

      // for automotic, only shoot once, but will become automatic if the fire_mode has full_exhaust enum
    }

    _metadata->fr_ms_left += _metadata->fr_ms_delay;
    _create_hitreg(&_start_data, _metadata);
    return;
  }

  if(_additional_fm & weapon_stats_data::full_exhaust){
    // quickly return, it cannot be stopped
    return;
  }

  // don't set burst iteration, cannot be stopped
  //_metadata->burst_iteration_left = 0;

  _metadata->do_fire = false;
  
  // no way that it will be set true if not full_exhaust
  // _metadata->fire_until_exhausted = false;
}

void weapon_handler::on_action_cancelled(bool is_secondary, bool is_alternate){
  _stats_metadata* _metadata;

  if(is_alternate){
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_primary;
    _metadata = _damager_alternate_metadata[_type];
  }
  else{
    weapon_item::stats_type _type = is_secondary? weapon_item::stats_type_secondary: weapon_item::stats_type_primary;
    _metadata = _damager_metadata[_type];
  }

  // reset all firing criteria
  _metadata->burst_iteration_left = 0;
  _metadata->do_fire = false;
  _metadata->fire_until_exhausted = false;
}


bool weapon_handler::is_alternate_viable(bool is_secondary){
  return true;
}


void weapon_handler::on_update(const action_class_update_data& update_data){
  _global_pos = update_data.global_pos;
  _target_pos = update_data.target_global_pos;

  queue_redraw();
}


I_action_class* weapon_handler::get_primary_class(){
  _weapon_action_helper* _action_helper = _action_helper_cm.get_data<_weapon_action_helper>();

  return _action_helper;
}

I_action_class* weapon_handler::get_secondary_class(){
  _weapon_action_helper* _action_helper = _alt_action_helper_cm.get_data<_weapon_action_helper>();

  return _action_helper;
}