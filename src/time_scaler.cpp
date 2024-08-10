#include "time_scaler.h"
#include "logger.h"

#include "algorithm_bezier.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

#include "godot_cpp/core/math.hpp"

using namespace godot;
using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace GameUtils;


#define MINIMUM_SPEED_TO_PAUSE 0.001
#define CONTINUE_TIME 0.2
#define PAUSE_TIME 0.8




double time_scaler::_get_scale_dist_normalized(double from, double to, double val){
  if(from == to)
    return 0;

  return Math::abs((val-from)/(to-from));
}

double time_scaler::_get_scale_dist(double from, double to, double norm_val){
  if(from == to)
    return 0;

  return Math::abs((to-from)*norm_val+from);
}


void time_scaler::_set_time_scale(double scale){
  SceneTree* _scenetree = get_tree();
  Engine* _engine = Engine::get_singleton();
  if(scale < MINIMUM_SPEED_TO_PAUSE){
    _scenetree->set_pause(true);
    scale = 1;
  }
  else if(_scenetree->is_paused()){
    _scenetree->set_pause(false);
    scale = MINIMUM_SPEED_TO_PAUSE;
  }

  Engine::get_singleton()->set_time_scale(scale);
  _current_scale = scale;
}

void time_scaler::_set_smooth_timer(double time, double seek){
  if(seek < 0)
    seek = 0;

  _smooth_time_current_time = seek;
  _smooth_time_wait_time = time;
}


void time_scaler::_bind_methods(){
  GD_PROPERTY_BIND(time_scaler, Variant::BOOL, time_scale_smooth_bezier_use_x)
  GD_PROPERTY_BIND(time_scaler, Variant::PACKED_VECTOR2_ARRAY, time_scale_smooth_bezier)

  ADD_SIGNAL(MethodInfo(_TIME_SCALER_SIGNAL_PROCESS_BYPASS, PropertyInfo(Variant::FLOAT, "delta")));
  ADD_SIGNAL(MethodInfo(_TIME_SCALER_SIGNAL_PHYSICS_PROCESS_BYPASS, PropertyInfo(Variant::FLOAT, "delta")));
}


void time_scaler::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _current_scale = _engine->get_time_scale();
    _prev_scale = _engine->get_time_scale();
    _next_scale = _engine->get_time_scale();
    set_process_mode(PROCESS_MODE_ALWAYS);

    _set_smooth_timer(0, 0);
  }
}


void time_scaler::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    delta = get_process_time_bypass();
    emit_signal(_TIME_SCALER_SIGNAL_PROCESS_BYPASS, delta);
  }
}

void time_scaler::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    delta = get_physics_process_time_bypass();
    emit_signal(_TIME_SCALER_SIGNAL_PHYSICS_PROCESS_BYPASS, delta);

    if(_smooth_time_current_time < _smooth_time_wait_time){
      _smooth_time_current_time += delta;

      double _next_scale = _smooth_time_scale_target;
      if(_smooth_time_current_time < _smooth_time_wait_time){
        Vector2 _bezier_result = bezier_curve_function(
          &time_scale_smooth_bezier,
          1 - (_smooth_time_current_time / _smooth_time_wait_time)
        );

        double _val;
        if(time_scale_smooth_bezier_use_x)
          _val = _bezier_result.x;
        else
          _val = _bezier_result.y;

        double _time_scale = (_smooth_time_scale_target - _smooth_time_scale_prev) * _val + _smooth_time_scale_prev;
        _next_scale = (_smooth_time_scale_target - _smooth_time_scale_prev) * _val + _smooth_time_scale_prev;
      }

      _set_time_scale(_next_scale);
    }
  }
}


void time_scaler::set_time_scale(double scale){
  _set_time_scale(scale);

  _prev_scale = _next_scale;
  _next_scale = scale;
}

void time_scaler::set_time_scale_smooth(double scale, double smooth_time){
  _smooth_time_scale_prev = _next_scale;
  _smooth_time_scale_target = scale;

  if(_smooth_time_current_time < _smooth_time_wait_time){
    double _norm_val = _get_scale_dist_normalized(_prev_scale, _next_scale, _current_scale);
    double _seek_time = _get_scale_dist(0, smooth_time, _norm_val);

    _set_smooth_timer(smooth_time, _seek_time);
  }
  else
    _set_smooth_timer(smooth_time);

  _prev_scale = _next_scale;
  _next_scale = scale;
}


void time_scaler::continue_game(bool slow_gradually){
  if(!_paused)
    return;

  _paused = false;
  if(slow_gradually)
    set_time_scale_smooth(_prev_scale, CONTINUE_TIME);
  else
    _set_time_scale(_prev_scale);
}


void time_scaler::pause_game(bool slow_gradually){
  if(_paused)
    return;

  _paused = true;
  if(slow_gradually)
    set_time_scale_smooth(0, PAUSE_TIME);
  else
    _set_time_scale(0);
}


double time_scaler::get_process_time_bypass(){
  return get_process_delta_time() * (1/_current_scale);
}

double time_scaler::get_physics_process_time_bypass(){
  return get_physics_process_delta_time() * (1/_current_scale);
}