#include "damage_indicator.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::Effect;
using namespace godot;


#define ANIMATION_ID_INDICATOR_POP "indicator_pop"


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(damage_indicator)
ATTR_BASE_INTERFACE_HELPER_DEFINE(damage_indicator)
ATTR_INTERFACE_HELPER_DEFINE(damage_indicator)


void damage_indicator::_on_animation_done(String animation_id){
  queue_free();
}


void damage_indicator::_bind_methods(){  
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(damage_indicator)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(damage_indicator)
  ATTR_INTERFACE_HELPER_BIND_GODOT(damage_indicator)

  GD_PROPERTY_BIND(damage_indicator, Variant::NODE_PATH, animation_node)
  GD_PROPERTY_BIND(damage_indicator, Variant::NODE_PATH, rt_label_node)

  GD_PROPERTY_BIND(damage_indicator, Variant::FLOAT, random_angle_min)
  GD_PROPERTY_BIND(damage_indicator, Variant::FLOAT, random_angle_max)

  GD_PROPERTY_BIND(damage_indicator, Variant::FLOAT, random_speed_min)
  GD_PROPERTY_BIND(damage_indicator, Variant::FLOAT, random_speed_max)

  ClassDB::bind_method(D_METHOD("_on_animation_done"), &damage_indicator::_on_animation_done);
}


void damage_indicator::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    GET_SUBAUTOLOAD(rng_server, sa_container, _rng_server)
  }
}

void damage_indicator::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_freeze_enabled(true);
    _animation_node = get_node<AnimationPlayer>(animation_node);
    _rt_label_node = get_node<RichTextLabel>(rt_label_node);

    _animation_node->connect("animation_finished", Callable(this, "_on_animation_done"));

    _rt_label_node->set_position(-_rt_label_node->get_size()/2);
    _rt_label_node->set_pivot_offset(_rt_label_node->get_size()/2);
  }
}


void damage_indicator::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void damage_indicator::start_indication(double damage){
  RandomNumberGenerator* _rng = _rng_server->get_randomizer(rng_type_global);

  Array _paramarr;{
    _paramarr.append(String::num(damage, 1));
  }

  _rt_label_node->set_text(String("[center]{0}").format(_paramarr));

  double _angle = _rng->randf_range(random_angle_max, random_angle_min) * (1 - (_rng->randi_range(0, 1) * 2));
  double _speed = _rng->randf_range(random_speed_min, random_speed_max);

  Vector2 _dir = Vector2(0, -1).rotated(Math::deg_to_rad(_angle));
  set_linear_velocity(_dir * _speed);
  set_freeze_enabled(false);

  _animation_node->play(ANIMATION_ID_INDICATOR_POP);
}