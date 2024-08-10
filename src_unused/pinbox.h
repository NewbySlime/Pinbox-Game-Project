#ifndef PINBOX_HEADER
#define PINBOX_HEADER


#include "damageable_object.h"
#include "dash.h"
#include "game_custom_input.h"
#include "godot_macro.h"
#include "I_movement.h"
#include "item_server.h"
#include "item_world_entity_2d_handler.h"
#include "item_dropper.h"
#include "item_picker_area.h"
#include "modifier_factory.h"
#include "subautoload_container.h"
#include "time_scaler.h"
#include "weapon_factory.h"
#include "weapon_handler.h"

#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/physics_direct_body_state2d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/classes/gpu_particles2d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector2.hpp"

#include "memory"


namespace Game{
  class pinbox: public Game::damageable_object, public Game::I_movement{
  GDCLASS(pinbox, Game::damageable_object)
    GD_PROPERTY_VAR(double, start_player_velocity)
    GD_PROPERTY_VAR(double, hold_action_timeout)

    GD_PROPERTY_VAR(double, bullet_time_scale)
    GD_PROPERTY_VAR(double, bullet_time_scale_smooth_time)

    GD_PROPERTY_VAR(godot::Vector2, movement_multiplier)

    GD_PROPERTY_VAR(godot::NodePath, hitbox_node)
    GD_PROPERTY_VAR(godot::NodePath, particle_node)

    GD_PROPERTY_VAR(godot::NodePath, rigidbody_collision_body)
    GD_PROPERTY_VAR(godot::NodePath, dash_raycast)

    private:
      enum _action_key_type{
        akt__key_mask = 0x00FF,
        akt_no_key = 0x0000,
        akt_primary_key = 0x0001,
        akt_secondary_key = 0x0002,

        akt__type_mask = 0xFF00,
        akt_normal = 0x0000,
        akt_alternate = 0x0100
      };

      Game::Item::item_server* _item_server;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;
      Game::game_custom_input* _custom_input;
      Game::time_scaler* _time_scaler;

      Game::damageable_object* _hitbox_node;

      godot::GPUParticles2D* _particle_node;
      godot::Timer* _hold_action_timer;
      godot::Timer* _hold_dash_timer;

      Game::Item::item_dropper* _item_dropper;
      Game::Item::item_picker_area* _item_picker;


      int _input_flags = 0;
      int _input_flags_prev = 0;

      uint32_t _bullet_time_counter = 0;

      double _player_velocity;

      uint16_t _current_action = _action_key_type::akt_no_key;
      Game::weapon_handler* _primary_action;
      I_action_class* _secondary_action;

      uint16_t _current_dash_action = _action_key_type::akt_no_key;
      Game::EntityFunctionality::dash_action* _dash_action;

      int32_t _time_scale_count = 0;

      void _on_hold_action_timer_timeout();
      void _on_hold_dash_timer_timeout();

      void _do_bullet_time();
      void _cancel_bullet_time();

      void _cancel_all_action();

      void _time_scale_slow_down();
      void _time_scale_normal();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

      void _on_hp_changed() override;

    public:
      pinbox();
      ~pinbox();

      void _physics_process(double delta) override;
      void _integrate_forces(godot::PhysicsDirectBodyState2D* state) override;

      void custom_input(godot::Object* event);
  };
}

#endif