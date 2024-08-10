#ifndef BUMPER_HEADER
#define BUMPER_HEADER

#include "godot_macro.h"
#include "subautoload_container.h"
#include "subautoload_compatibles.h"

#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/timer.hpp"

#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/variant.hpp"


namespace Game{
  class bumper: public godot_sac::RigidBody2D_sac{
  GDCLASS(bumper, godot_sac::RigidBody2D_sac)
    GD_PROPERTY_VAR(double, bounce_force)
    GD_PROPERTY_VAR(godot::NodePath, animated_sprite)

    GD_PROPERTY_VAR(double, led_activation_time)

    GD_PROPERTY_VAR(double, scale_on_activation)

    GD_PROPERTY_VAR(double, bumper_base_score)

    private:
      godot::Timer *_bumper_led_activation_timer;
      godot::AnimatedSprite2D *_animated_sprite;

      godot::Vector2 _sprite_def_size;

      void _on_collision(godot::Node* body);

      void _activate_led();
      void _on_bumper_led_activation_timeout();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;
    
    public:
      bumper();
      ~bumper();
    
      void _process(double delta) override;
  };
}

#endif