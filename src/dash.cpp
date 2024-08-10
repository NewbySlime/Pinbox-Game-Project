#include "logger.h"

#include "dash.h"

#include "godot_cpp/classes/physics_server2d.hpp"
#include "godot_cpp/classes/physics_ray_query_parameters2d.hpp"

#include "godot_cpp/variant/dictionary.hpp"


using namespace Game;
using namespace Game::EntityFunctionality;
using namespace godot;


void dash_action::on_action_priming(){
  // do nothing
}

void dash_action::on_action_continuing(){
  if(_dash_type == dt_no_action){
    switch(_body_type){
      break; case pbt_rb2:{
        RigidBody2D* rb2 = (RigidBody2D*)_body;
        rb2->set_freeze_mode(rb2->FREEZE_MODE_KINEMATIC);
        rb2->set_freeze_enabled(true);

        _dash_start = _global_pos;
        _direction = (_target_pos - _global_pos).normalized();
        _dash_type = dt_normal_dash | dt_opt_init;
      }

      break; default:
        return;
    }
  }
}

void dash_action::on_action_cancelled(){
  // do nothing
  // maybe when getting hit (determined by the entity itself)
}


bool dash_action::is_alternate_viable(){
  // always true for now
  return true;
}

void dash_action::on_alternate_action_priming(){
  // do nothing
}

void dash_action::on_alternate_action_continuing(){
  if(_dash_type == dt_no_action){
    _dash_type = dt_ultra_dash;
  }
}

void dash_action::on_alternate_action_cancelled(){
  // do nothing
  // ultra dash will instantly teleport you
}


void dash_action::on_update(const action_class_update_data& update_data){
  _global_pos = update_data.global_pos;
  _target_pos = update_data.target_global_pos;
}

bool dash_action::on_physics_update(double delta){
  if(_body != NULL){
    uint16_t _dash_type_mask = _dash_type & dt_type;
    switch(_dash_type_mask){
      break; case dt_normal_dash:{
        switch(_body_type){
          break; case pbt_rb2:{
            RigidBody2D* rb2 = (RigidBody2D*)_body;
            double _length_from_start = (_global_pos - _dash_start).length();
            double _length_to_target = _dash_length_normal - _length_from_start;
            double _dash_length_increment = _dash_speed * delta;

            bool _done = false;
            uint16_t _dash_opt_flag = _dash_type & dt_opt_flag;
            if(_dash_opt_flag & dt_opt_init)
              _dash_type &= ~dt_opt_init;
            else if(rb2->get_colliding_bodies().size() > 0)
              _done = true;

            if(_length_to_target < _dash_length_increment){
              _dash_length_increment = _length_to_target;
              _done = true;
            }

            Vector2 _next_pos = _direction * _dash_length_increment + _global_pos;
            rb2->set_global_position(_next_pos);
            if(_done){
              rb2->set_linear_velocity(_direction * _dash_speed);
              _dash_type = dt_no_action;
              rb2->set_freeze_enabled(false);
            }

            return true;
          }
        }
      }

      break; case dt_ultra_dash:{
        switch(_body_type){
          break; case pbt_rb2:{
            RigidBody2D* rb2 = (RigidBody2D*)_body;
            rb2->set_freeze_mode(rb2->FREEZE_MODE_KINEMATIC);
            rb2->set_freeze_enabled(true);
            Vector2 _last_velocity = rb2->get_linear_velocity();
            double _last_radial_vel = rb2->get_angular_velocity();

            _direction = (_target_pos - _global_pos).normalized();
            
            Vector2 _new_target_pos = _direction * _dash_length_ultra;
            _raycast->set_global_rotation_degrees(0);
            _raycast->set_global_position(_global_pos);
            _raycast->set_target_position(_new_target_pos);
            _raycast->force_raycast_update();
            
            _new_target_pos += _global_pos;
            
            if(_raycast->is_colliding())
              _new_target_pos = _raycast->get_collision_point() - _raycast->get_collision_normal() * _dash_margin;

            rb2->set_global_position(_new_target_pos);
            rb2->set_freeze_enabled(false);

            PhysicsDirectBodyState2D* _state = PhysicsServer2D::get_singleton()->body_get_direct_state(rb2->get_rid());
            _state->set_linear_velocity(_last_velocity + _direction * _dash_speed);
            _state->set_angular_velocity(_last_radial_vel);
            
            rb2->_integrate_forces(_state);
            _dash_type = dt_no_action;

            return false;
          }
        }
      }
    }
  }

  return false;
}

bool dash_action::on_integrate_forces(PhysicsDirectBodyState2D* state){
  // ignored since it uses freeze mode
  return false;
}


void dash_action::bind_physics_object(Object* obj, uint32_t type){
  switch(type){
    break; case dapo_physics_body:{
      if(!obj->is_class("PhysicsBody2D"))
        break;

      _body = (PhysicsBody2D*)obj;
      if(obj->is_class("RigidBody2D"))
        _body_type = pbt_rb2;
    }

    break; case dapo_collision_shape:{
      if(!obj->is_class("CollisionShape2D"))
        break;

      CollisionShape2D* _col = (CollisionShape2D*)obj;
      Ref<Shape2D> _shape = _col->get_shape();
      if(_shape->is_class("CircleShape2D")){
        CircleShape2D* _circleshape = (CircleShape2D*) _shape.ptr();
        _dash_margin = _circleshape->get_radius();
      }
    }

    break; case dapo_raycast:{
      if(!obj->is_class("RayCast2D"))
        break;

      _raycast = (RayCast2D*)obj;
    }
  }
}

void dash_action::bind_graphics_object(Object* obj, uint32_t type){
  switch(type){
    break; case dago_gpu_particle_node:{
      if(obj->is_class("GPUParticles2D"))
        _particle_node = (GPUParticles2D*)obj;
    }
  }
}