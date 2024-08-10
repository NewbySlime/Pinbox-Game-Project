#ifndef CONSUMABLES_HANDLER_HEADER
#define CONSUMABLES_HANDLER_HEADER

#include "I_action_class.h"

#include "godot_cpp/classes/node2d.hpp"


namespace Game{
  class consumables_handler: public godot::Node2D, I_action_class{
  GDCLASS(consumables_handler, godot::Node2D)

    protected:
      static void _bind_methods();

    public:

  };
}

#endif