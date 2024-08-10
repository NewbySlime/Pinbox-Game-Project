#include "logger.h"
#include "game_counter.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace godot;



void game_counter::_update_bumper_score(double score){
  _bumper_score += score;
  if(_bumper_score < 0)
    _bumper_score = 0;

  double _new_bumper_mult_float = Math::log((_bumper_score + bumper_base_limit * bumper_base_multiplier) / bumper_base_limit) / Math::log(bumper_base_multiplier);

  int _new_bumper_mult = _new_bumper_mult_float;
  if(_new_bumper_mult > bumper_max_multiplier)
    _new_bumper_mult = bumper_max_multiplier;

  if(_bumper_multiplier != _new_bumper_mult){
    _bumper_multiplier = _new_bumper_mult;
  }

  emit_signal(SIGNAL_GAME_COUNTER_SCORE_ON_INCREMENT);
}


void game_counter::_bind_methods(){
  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_COUNTER_SCORE_ON_INCREMENT
  ));

  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_COUNTER_SCORE_MULTIPLIER_CHANGED,
    PropertyInfo(Variant::INT, "multiplier_type")
  ));
}


void game_counter::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _g_var = sa_container->get_singleton_node<game_global_variable>();
  }
}

void game_counter::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    reset_score();

    const ui_combo_tracker_data_struct* _bumper_combo_data = _g_var->get_ui_combo_tracker_data(sat_bumper);
    if(_bumper_combo_data){
      bumper_base_limit = _bumper_combo_data->base_limit;
      bumper_base_multiplier = _bumper_combo_data->base_multiplier;
      bumper_multiplier_decrement = _bumper_combo_data->multiplier_decrement;
      bumper_max_multiplier = _bumper_combo_data->max_multiplier;
    }
  }
}


game_counter::game_counter(){
  _bumper_multiplier = 1;
  _bumper_score = 0;
}


void game_counter::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_bumper_score > 0){
      _update_bumper_score(-delta * bumper_multiplier_decrement);
    }
  }
}


void game_counter::add_score(const score_increment_data& increment_data){
  if(increment_data.base_score > 0){
    int _base_score = increment_data.base_score;

    switch(increment_data.score_type){
      break; case sat_bumper:{
        _update_bumper_score(_base_score);
        _base_score *= _bumper_multiplier;
      }
    }

    _score.score += _base_score;
    emit_signal(SIGNAL_GAME_COUNTER_SCORE_ON_INCREMENT);
  }
}

score_data* game_counter::get_score_data(){
  return &_score;
}

int game_counter::get_combo_multiplier_bumper(){
  return _bumper_multiplier;
}

double game_counter::get_combo_val_bumper(){
  double _bumper_mult_val = Math::log((_bumper_score + bumper_base_limit * bumper_base_multiplier) / bumper_base_limit) / Math::log(bumper_base_multiplier);
  
  double _dumpf;
  double _bumper_mult_val_mod = modf(_bumper_mult_val, &_dumpf);

  return _bumper_mult_val_mod;
}


void game_counter::reset_score(){
  _score.score = 0;

  _bumper_multiplier = 0;
  _bumper_score = 0;
}