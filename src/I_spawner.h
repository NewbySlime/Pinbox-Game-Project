#ifndef I_SPAWNER_HEADER
#define I_SPAWNER_HEADER

#include "interface_helper.h"

#include "godot_cpp/variant/vector2.hpp"


namespace Game{
  // This interface/spawner only for giving "spawning" trigger to appropriate actors
  class I_spawner{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_spawner)

    public:
      virtual void spawn_entity(){}

      virtual godot::Vector2 get_spawn_position(){ return Vector2(0,0); }

  };
}

#endif