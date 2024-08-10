#include "damageable_object.h"
#include "defines.h"
#include "hit_registers.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"


using namespace Game;
using namespace Game::HitRegisters;
using namespace godot;


void Game::HitRegisters::register_class_hit_registers(){
  ClassDB::register_class<hr_raycast>();
}


void hr_raycast::_do_start_casting(){
  set_global_position(_start_data.position);
  set_collision_mask(
    get_collision_mask() |
    (_start_data.is_from_player? COLLISION_LAYER_ENEMY_LAYER: COLLISION_LAYER_PLAYER_LAYER)
  );

  set_target_position((_start_data.dir * _metadata.current_hitreg_data->length));
  clear_exceptions();

  queue_redraw();

  while(true){
    force_raycast_update();
    if(!is_colliding())
      break;

    CollisionObject2D* collider = (CollisionObject2D*)get_collider();

    hit_register_state _state;{
      _state.hitreg_type = hrte_raycast;
    }

    hit_register_pass_data _pass_data;
    _pass_data.collider = collider;
    _pass_data.do_continue = true;
    _pass_data.current_hitreg_state = &_state;
    
    _metadata.damager->on_hit(&_pass_data);
    add_exception_rid(collider->get_rid());

    if(!_pass_data.do_continue)
      break;
  }

  on_hitreg_finished();
}


void hr_raycast::_bind_methods(){
  
}


void hr_raycast::start_casting(){
  _do_cast = true;
  if(is_node_ready())
    _do_start_casting();
}


hr_raycast::hr_raycast(){
  _do_cast = false;
}

void hr_raycast::_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    set_collide_with_areas(true);
    set_collide_with_bodies(true);
    set_enabled(false);
    set_exclude_parent_body(false);
    set_hit_from_inside(false);
    set_collision_mask(
      COLLISION_LAYER_COLLISION_LAYER |
      COLLISION_LAYER_PROJECTILE_LAYER |
      COLLISION_LAYER_STATIC_LAYER
    );

    if(_do_cast)
      _do_start_casting();
  }
}

void hr_raycast::_draw(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    draw_line(_start_data.position - get_global_position(), _start_data.position + _start_data.dir * _metadata.current_hitreg_data->length - get_global_position(), Color(0.2, 0.1, 1));
  }
}