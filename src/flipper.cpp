#include "algorithm_node.h"
#include "flipper.h"
#include "flipper_handler.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Handlers;
using namespace Game::Misc::Algorithm;
using namespace GameUtils;
using namespace godot;


void flipper::_check_input(int _flag){
  bool _do_turn_previous = _do_turn;

  if(use_right_hand_input){
    if(_flag & flipper_handler::right_hand_flipper)
      _do_turn = true;
    else
      _do_turn = false;
  }
  else{
    if(_flag & flipper_handler::left_hand_flipper)
      _do_turn = true;
    else
      _do_turn = false;
  }

  if(_do_turn_check == _do_turn && _do_turn != _do_turn_previous)
    _do_turn_check = !_do_turn_check;
}

void flipper::_bind_methods(){
  GD_PROPERTY_BIND(flipper, Variant::BOOL, is_right_hand)

  GD_PROPERTY_BIND(flipper, Variant::BOOL, use_right_hand_input)
  GD_PROPERTY_BIND(flipper, Variant::FLOAT, max_turn)
  GD_PROPERTY_BIND(flipper, Variant::FLOAT, turn_speed)

  ClassDB::bind_method(D_METHOD("_check_input"), &flipper::_check_input);
}


flipper::flipper(){
  is_right_hand = false;
  use_right_hand_input = false;
  max_turn = 35;
  
  // turns until max_turn for 50ms
  turn_speed = max_turn * 1000/50; 
}


void flipper::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    flipper_handler* _handler = find_autoload<flipper_handler>(this);
    if(_handler)
      _handler->connect(SIGNAL_FLIPPER_HANDLER_ON_INPUT, Callable(this, "_check_input"));
    else{
      INIT_ASSERT()
      ERR_ASSERT("Cannot get flipper_handler for flipper's input.")
    }
  
    _angle_offset = get_rotation_degrees();

    _check_input(_handler->get_input_flag());
  }
}


void flipper::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(!_disabled && _do_turn_check != _do_turn){
      double _current = get_rotation_degrees();
      double _speed;
      double _offset = _angle_offset;
      double _target = _offset;

      if(_do_turn){
        if(is_right_hand){
          _speed = turn_speed;
          _target = _offset + max_turn;
        }
        else{
          _speed = -turn_speed;
          _target = _offset - max_turn;
        }
      }
      else{
        if(is_right_hand)
          _speed = -turn_speed;
        else
          _speed = turn_speed;
          
        _target = _offset;
      }

      double _reach = _speed * delta;
      double _dist = _target - _current;
      if(Math::abs(_reach) >= Math::abs(_dist)){
        _do_turn_check = _do_turn;
        _reach = _dist;

        set_freeze_enabled(false);
        set_freeze_enabled(true);
      }

      set_rotation_degrees(_current + _reach);
    }
  }
}


void flipper::set_flipper_disabled(bool b){
  _disabled = b;
}


GD_PROPERTY_GETTER_FUNC(flipper, bool, is_right_hand){
  return is_right_hand;
}

GD_PROPERTY_SETTER_FUNC(flipper, bool, is_right_hand){
  if(is_right_hand != var_is_right_hand){
    if(var_is_right_hand){
      set_scale(Vector2(-1, get_scale().y));
    }else{
      set_scale(Vector2(1, get_scale().y));
    }

    is_right_hand = var_is_right_hand;
  }
}