#ifndef I_PHYSICS_BODY_MODIFIER_HEADER
#define I_PHYSICS_BODY_MODIFIER_HEADER

#include "godot_cpp/classes/rigid_body2d.hpp"
#include "godot_cpp/classes/physics_direct_body_state2d.hpp"


namespace Game{
  class I_physics_handler{
    public:
      virtual void bind_physics_object(godot::Object* obj, uint32_t type) = 0;
      
      // returns is modified
      virtual bool on_physics_update(double delta) = 0;

      // returns is modifier
      virtual bool on_integrate_forces(godot::PhysicsDirectBodyState2D* state) = 0;
  };
}

#endif