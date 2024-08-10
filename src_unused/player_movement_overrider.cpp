#include "logger.h"
#include "player_movement_overrider.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input_event_key.hpp"

#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Overrider;
using namespace godot;


void player_movement_overrider::_bind_methods(){
  ClassDB::bind_method(D_METHOD("custom_input", "input_event"), &player_movement_overrider::custom_input);
}


void player_movement_overrider::_on_added(){

}

void player_movement_overrider::_on_removed(){

}


void player_movement_overrider::_bind_subautoload(subautoload_container* container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _custom_input = _container->get_singleton_node<game_custom_input>();
  }
}

void player_movement_overrider::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _custom_input->connect(SIGNAL_GAME_CUSTOM_INPUT_CUSTOM_INPUT, Callable(this, "custom_input"))
  }
}


void player_movement_overrider::custom_input(Object* _evnet){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint() && _event->is_class("InputEvent")){
    InputEvent* event = (InputEvent*)_event;
    if(event->is_class("InputEventKey")){
      InputEventKey* event_key = (InputEventKey*)event;
      if(event->is_action_pressed("ui_up")){
        LOG_ASSERT("test up")
      }
    }
  }
}