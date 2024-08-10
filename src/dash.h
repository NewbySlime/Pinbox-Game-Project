#ifndef PLAYER_DASH_HEADER
#define PLAYER_DASH_HEADER

#include "I_action_class.h"
#include "I_graphics_handler.h"
#include "I_physics_handler.h"

#include "godot_cpp/classes/collision_shape2d.hpp"
#include "godot_cpp/classes/circle_shape2d.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/gpu_particles2d.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/ray_cast2d.hpp"

#include "godot_cpp/variant/vector2.hpp"


namespace Game::EntityFunctionality{
  class dash_action: Game::I_action_class, Game::I_physics_handler, Game::I_graphics_handler{
    public:    
      enum dash_action_graphics_obj{
        dago_gpu_particle_node
      };

      enum dash_action_physics_obj{
        dapo_physics_body,
        dapo_collision_shape,
        dapo_raycast
      };

    private:
      enum physics_body_type{
        pbt_rb2
      };

      enum dashing_type{
        dt_type = 0x00FF,
        dt_no_action = 0x0,
        dt_normal_dash = 0x1,
        dt_ultra_dash = 0x2,

        dt_opt_flag = 0xFF00,
        dt_opt_init = 0x0100
      };

      godot::RayCast2D* _raycast = NULL;
      godot::PhysicsBody2D* _body = NULL;
      uint32_t _body_type;

      uint16_t _dash_type = dashing_type::dt_no_action;

      godot::Vector2 _global_pos;
      godot::Vector2 _target_pos;

      double _dash_margin = 0;

      double _dash_speed = 1000;
      double _dash_length;
      godot::Vector2 _dash_start;
      godot::Vector2 _direction;

      double _dash_length_normal = 100;
      double _dash_length_ultra = 900;

      godot::GPUParticles2D* _particle_node;
      double _particle_emit_max_length = 50;


    public:
      void on_action_priming() override;
      void on_action_continuing() override;
      void on_action_cancelled() override;

      bool is_alternate_viable() override;
      void on_alternate_action_priming() override;
      void on_alternate_action_continuing() override;
      void on_alternate_action_cancelled() override;

      void on_update(const action_class_update_data &update_data) override;
      bool on_physics_update(double delta) override;
      bool on_integrate_forces(godot::PhysicsDirectBodyState2D* state) override;

      void bind_physics_object(godot::Object* obj, uint32_t type) override;
      void bind_graphics_object(godot::Object* obj, uint32_t type) override;
  };
}

#endif