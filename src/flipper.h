#ifndef FLIPPER_HEADER
#define FLIPPER_HEADER

#include "godot_macro.h"

#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/sprite2d.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/physics_direct_body_state2d.hpp"

#include "godot_cpp/variant/variant.hpp"


namespace Game{
  class flipper: public godot::RigidBody2D{
  GDCLASS(flipper, godot::RigidBody2D)
    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(bool, is_right_hand)

    GD_PROPERTY_VAR(bool, use_right_hand_input)
    GD_PROPERTY_VAR(double, max_turn)
    GD_PROPERTY_VAR(double, turn_speed)

    private:
      // in degrees
      double _angle_offset;

      bool _do_turn = false;
      bool _do_turn_check = false;

      bool _disabled = false;

    protected:
      void _check_input(int _flag);
      static void _bind_methods();

    public:
      flipper();
      
      void _ready() override;

      void _physics_process(double delta) override;

      void set_flipper_disabled(bool b);
  };
}

#endif