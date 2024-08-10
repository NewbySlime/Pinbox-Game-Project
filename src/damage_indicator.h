#ifndef DAMAGE_INDICATOR_HEADER
#define DAMAGE_INDICATOR_HEADER

#include "damageable_object.h"
#include "godot_macro.h"
#include "interface_helper.h"
#include "rng_server.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/rich_text_label.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/variant/node_path.hpp"


namespace Game::Effect{
  class damage_indicator: public godot::RigidBody2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(damage_indicator, godot::RigidBody2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(damage_indicator)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(damage_indicator)
  ATTR_INTERFACE_HELPER_INHERIT(damage_indicator)
  
    GD_PROPERTY_VAR(godot::NodePath, animation_node);
    GD_PROPERTY_VAR(godot::NodePath, rt_label_node);

    GD_PROPERTY_VAR(double, random_angle_min)
    GD_PROPERTY_VAR(double, random_angle_max)

    GD_PROPERTY_VAR(double, random_speed_min)
    GD_PROPERTY_VAR(double, random_speed_max)

    private:
      Game::rng_server* _rng_server;

      godot::AnimationPlayer* _animation_node;
      godot::RichTextLabel* _rt_label_node;

      void _on_animation_done(godot::String animation_id);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

      void start_indication(double damage);
  };
}

#endif