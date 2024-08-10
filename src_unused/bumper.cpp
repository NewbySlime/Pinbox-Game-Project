#include "logger.h"

#include "bumper.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

#define ANIMATION_STATE_NAME_BUMPED "bumped"
#define ANIMATION_STATE_NAME_IDLE "idle"


using namespace godot;
using namespace Game;



void bumper::_on_collision(Node* body){
  if(body->is_class("RigidBody2D")){
    RigidBody2D* _rb = (RigidBody2D*)body;
  
    float _angle = _rb->get_global_position().angle_to_point(get_global_position());
    _angle -= Math::deg_to_rad(180.0);

    Vector2 _dir = Vector2::from_angle(_angle);

    _dir *= bounce_force;
    _rb->apply_force(_dir);

    score_increment_data _increment_data;{
      _increment_data.base_score = bumper_base_score;
      _increment_data.score_type = sat_bumper;
    }

    _activate_led();
    _game_counter->add_score(_increment_data);
  }
}


void bumper::_activate_led(){
  _bumper_led_activation_timer->start(led_activation_time);
  _animated_sprite->set_animation(ANIMATION_STATE_NAME_BUMPED);
}

void bumper::_on_bumper_led_activation_timeout(){
  _animated_sprite->set_animation(ANIMATION_STATE_NAME_IDLE);
  _animated_sprite->set_scale(_sprite_def_size);
}
 

void bumper::_bind_methods(){
  ClassDB::bind_method(D_METHOD("_on_collision", "body"), &bumper::_on_collision);
  ClassDB::bind_method(D_METHOD("_on_bumper_led_activation_timeout"), &bumper::_on_bumper_led_activation_timeout);

  GD_PROPERTY_BIND(bumper, Variant::FLOAT, bounce_force)
  GD_PROPERTY_BIND(bumper, Variant::NODE_PATH, animated_sprite)

  GD_PROPERTY_BIND(bumper, Variant::FLOAT, led_activation_time)

  GD_PROPERTY_BIND(bumper, Variant::FLOAT, scale_on_activation)

  GD_PROPERTY_BIND(bumper, Variant::FLOAT, bumper_base_score)
}


void bumper::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _game_counter = sa_container->get_singleton_node<game_counter>();
  }
}

void bumper::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    INIT_ASSERT()
    LOG_ASSERT("test")
    _animated_sprite = get_node<AnimatedSprite2D>(animated_sprite);

    _sprite_def_size = _animated_sprite->get_scale();

    _bumper_led_activation_timer->set_autostart(false);
    _bumper_led_activation_timer->set_one_shot(true);
    _bumper_led_activation_timer->connect("timeout", Callable(this, "_on_bumper_led_activation_timeout"));
    ADD_CHILD_SA(_bumper_led_activation_timer);

    connect("body_entered", Callable(this, "_on_collision"));
    LOG_ASSERT("test end")
  }
}


bumper::bumper(){
  bounce_force = 100;
  led_activation_time = 0.3;
  scale_on_activation = 1.2;

  _bumper_led_activation_timer = new Timer();
}

bumper::~bumper(){
  delete _bumper_led_activation_timer;
}


void bumper::_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(!_bumper_led_activation_timer->is_stopped()){
      double _val = _bumper_led_activation_timer->get_time_left()/led_activation_time;
      double _scale_diff = scale_on_activation - 1;

      _animated_sprite->set_scale(_sprite_def_size * ((_scale_diff * _val) + 1));
    }
  }
}