#include "defines.h"
#include "logger.h"

#include "item_world_entity_2d.h"

#include "item_picker_area.h"
#include "item_world_entity_2d_handler.h"
#include "item_server.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/physics_direct_body_state2d.hpp"
#include "godot_cpp/classes/physics_server2d.hpp"


#define IWE2D_PICKED_ANIMATION "iwe_picked"


using namespace Game;
using namespace Game::Item;
using namespace Game::Misc::Algorithm;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(item_world_entity_2d)
ATTR_BASE_INTERFACE_HELPER_DEFINE(item_world_entity_2d)
ATTR_INTERFACE_HELPER_DEFINE(item_world_entity_2d)
ATTR_PARENT_CHILD_WATCHER_HELPER_DEFINE(item_world_entity_2d)



void item_world_entity_2d::_on_animation_finished(String animation_name){
  if(animation_name == IWE2D_PICKED_ANIMATION && _queue_free)
    queue_free();
}


void item_world_entity_2d::_on_picker_deleted(uint64_t id){
  remove_picker_gravity(id);
}


void item_world_entity_2d::_on_pickable_timer_timeout(){
  set_pickable(true);
}


void item_world_entity_2d::_start_animation_picked(){
  if(_anim_player->has_animation(IWE2D_PICKED_ANIMATION))
    _anim_player->play(IWE2D_PICKED_ANIMATION);
  else{
    INIT_ASSERT()
    WARN_ASSERT("'" IWE2D_PICKED_ANIMATION "' Animation doesn't exist.")

    _on_animation_finished(IWE2D_PICKED_ANIMATION);
  }
}


void item_world_entity_2d::_on_item_deleted(gdvar_custom_object param){
  item_server_item_removed_param* _param = GDVAR_CUSTOM_TO(param, item_server_item_removed_param);
  item_inst_id _item_id = _param->get_id();
  if(_item_id != _bound_item_id)
    return;

  _bound_item_id = ITEM_ID_NULL;
  _bound_item = NULL;
  // retain the resource data, as it will use picked animation

  _is_pickable = false;
  on_object_picked_up();
}

void item_world_entity_2d::_on_item_changed(){
  for(auto _pair: _item_handler_interfacer_map)
    _pair.second->set_item_object(_bound_item);
}


void item_world_entity_2d::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(item_world_entity_2d)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(item_world_entity_2d)
  ATTR_INTERFACE_HELPER_BIND_GODOT(item_world_entity_2d)
  ATTR_PARENT_CHILD_WATCHER_HELPER_BIND_GODOT(item_world_entity_2d)

  GD_PROPERTY_BIND(item_world_entity_2d, Variant::FLOAT, default_throw_smooth_time)
  GD_PROPERTY_BIND(item_world_entity_2d, Variant::FLOAT, time_until_pickable)

  GD_PROPERTY_BIND(item_world_entity_2d, Variant::PACKED_VECTOR2_ARRAY, smooth_throw_bezier)

  GD_PROPERTY_BIND(item_world_entity_2d, Variant::NODE_PATH, animated_player_node)

  ClassDB::bind_method(
    D_METHOD("_on_animation_finished", "anim_name"),
    &item_world_entity_2d::_on_animation_finished
  );

  ClassDB::bind_method(
    D_METHOD("_on_picker_deleted"
      "picker_id"
    ),
    
    &item_world_entity_2d::_on_picker_deleted
  );

  ClassDB::bind_method(
    D_METHOD("_on_pickable_timer_timeout"),
    &item_world_entity_2d::_on_pickable_timer_timeout
  );

  ClassDB::bind_method(
    D_METHOD("_on_item_deleted", "param_obj"),
    &item_world_entity_2d::_on_item_deleted
  );
}


void item_world_entity_2d::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    __child_watcher_change_parent(this);

    CREATE_GET_NODE_TESTER(_anim_player, animated_player_node, AnimationPlayer)

    _throw_timer = new Timer();
    _throw_timer->set_autostart(false);
    _throw_timer->set_one_shot(true);
    _throw_timer->set_timer_process_callback(Timer::TIMER_PROCESS_PHYSICS);
    add_child_sa(_throw_timer);

    _pickable_timer = new Timer();
    _pickable_timer->set_autostart(false);
    _pickable_timer->set_one_shot(true);
    _pickable_timer->set_timer_process_callback(Timer::TIMER_PROCESS_PHYSICS);
    _pickable_timer->connect("timeout", Callable(this, "_on_pickable_timer_timeout"));
    add_child_sa(_pickable_timer);
  
    _item_server->connect(SIGNAL_ITEM_SERVER_ITEM_REMOVED, Callable(this, "_on_item_deleted"));
  }
}


void item_world_entity_2d::_on_child_entered(Node* child){
  I_item_handler* _interface;
  INTERFACE_GET(child, I_item_handler, _interface);
  if(!_interface)
    return;

  _interface->set_item_object(_bound_item);
  _item_handler_interfacer_map[child->get_instance_id()] = _interface;
}

void item_world_entity_2d::_on_child_exited(Node* child){
  auto _iter = _item_handler_interfacer_map.find(child->get_instance_id());
  if(_iter == _item_handler_interfacer_map.end())
    return;

  _item_handler_interfacer_map.erase(_iter);
}


item_world_entity_2d::item_world_entity_2d(){
  _item_server = NULL;
  _iwe2d_handler = NULL;

  default_throw_smooth_time = 0.3;

  _bound_item_id = ITEM_ID_NULL;

  _pickable_timer = NULL;
  _is_pickable = false;

  _throw_timer = NULL;
  _throw_done = true;
}


void item_world_entity_2d::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_PARENT_CHILD_WATCHER_HELPER_ON_NOTIFICATION(code)
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      INIT_ASSERT()
      LOG_ASSERT("deleting")

      if(_iwe2d_handler)
        _iwe2d_handler->_delete_iwe2d(_entity_id);

      if(_throw_timer)
        delete _throw_timer;

      if(_pickable_timer)
        delete _pickable_timer;
    }
  }
}


void item_world_entity_2d::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(!_throw_done){
      godot::Vector2 _bezier_val = bezier_curve_function(
        &smooth_throw_bezier,
        _throw_timer->get_time_left()/_throw_timer->get_wait_time(),
        true
      );

      double _new_dist = _bezier_val.x * _throw_dist;
      Vector2 _force = _throw_dir * (_new_dist - _throw_last_dist);
      move_and_collide(_force);

      if(_throw_timer->get_time_left() <= 0)
        _throw_done = true;

      _throw_last_dist = _new_dist;
    }

    if(_is_pickable){
      Vector2 _force;
      for(auto _pair: _picker_inrange_map){
        double _speed = _pair.second->get_picker_gravity_speed(get_global_position(), _bound_item);
        Vector2 _dir = (_pair.second->get_global_position() - get_global_position()).normalized();

        _force += _dir * _speed;
      }

      move_and_collide(_force * delta);
    }
  }
}


void item_world_entity_2d::bind_item(const item_object* item){
  _bound_item = item;
  _bound_item_id = item->get_id();

  _on_item_changed();
}

void item_world_entity_2d::throw_object(Vector2 force, bool is_pickable, double smooth_time){
  _throw_dir = force.normalized();
  _throw_dist = force.length();
  _throw_last_dist = 0;

  if(smooth_time < 0)
    smooth_time = default_throw_smooth_time;

  _throw_timer->start(smooth_time);
  _throw_done = false;

  if(is_pickable)
    _pickable_timer->start(time_until_pickable);
}


void item_world_entity_2d::add_picker_gravity(uint64_t picker_id){
  item_picker_area* _picker = _iwe2d_handler->get_item_picker_area(picker_id);
  if(!_picker)
    return;

  _picker_inrange_map[picker_id] = _picker;
}

void item_world_entity_2d::remove_picker_gravity(uint64_t picker_id){
  auto _iter = _picker_inrange_map.find(picker_id);
  if(_iter == _picker_inrange_map.end())
    return;

  _picker_inrange_map.erase(_iter);
}


void item_world_entity_2d::on_object_picked_up(){
  _queue_free = true;
  _start_animation_picked();
}


void item_world_entity_2d::set_pickable(bool _value){
  _is_pickable = _value;
}

bool item_world_entity_2d::is_pickable(){
  return _is_pickable;
}


uint64_t item_world_entity_2d::get_dropped_item_id(){
  return _bound_item_id;
}