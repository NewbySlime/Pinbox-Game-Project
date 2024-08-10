#ifndef HIT_REGISTERS_HEADER
#define HIT_REGISTERS_HEADER

#include "hit_register_handler.h"

#include "godot_cpp/classes/ray_cast2d.hpp"


namespace Game::HitRegisters{
  void register_class_hit_registers();

  class hr_raycast: public godot::RayCast2D, Game::hit_register{
  GDCLASS(hr_raycast, godot::RayCast2D)

    private:
      bool _do_cast;

      void _do_start_casting();

    protected:
      static void _bind_methods();

      void start_casting() override;

    public:
      hr_raycast();
      void _ready() override;
      void _draw() override;
  };
}

#endif