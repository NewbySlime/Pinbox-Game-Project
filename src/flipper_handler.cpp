#include "flipper_handler.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

using namespace Game::Handlers;
using namespace godot;



void flipper_handler::_bind_methods(){
  ADD_SIGNAL(MethodInfo(
    SIGNAL_FLIPPER_HANDLER_ON_INPUT,
    PropertyInfo(Variant::INT, "input_flag")
  ));
}


flipper_handler::flipper_handler(){
  _input_flag = 0;
}


void flipper_handler::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    
  }
}


void flipper_handler::_input(const Ref<InputEvent>& event){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    int _previous_input_flag = _input_flag;

    if(event->is_action_pressed("flipper_right"))
      _input_flag |= right_hand_flipper;
    else if(event->is_action_released("flipper_right"))
      _input_flag &= ~right_hand_flipper;
      
    if(event->is_action_pressed("flipper_left"))
      _input_flag |= left_hand_flipper;
    else if(event->is_action_released("flipper_left"))
      _input_flag &= ~left_hand_flipper;

    if(_previous_input_flag != _input_flag)
      emit_signal(SIGNAL_FLIPPER_HANDLER_ON_INPUT, Variant(_input_flag));
  }
}


int flipper_handler::get_input_flag(){
  return _input_flag;
}