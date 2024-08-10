#ifndef LEVEL_GENERATOR_HEADER
#define LEVEL_GENERATOR_HEADER

#include "godot_cpp/classes/node2d.hpp"


namespace Game{
  class level_generator: public godot::Node2D{
  GDCLASS(level_generator, godot::Node2D)

    private:
      
    protected:
      static void _bind_methods();

    public:
      void _ready() override;

      void start_generating();
      
  };
}

#endif