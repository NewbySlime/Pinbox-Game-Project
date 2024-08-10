#ifndef BUS_EVENT_HEADER
#define BUS_EVENT_HEADER

#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"

#define SIGNAL_BUS_EVENT_DAMAGEABLE_DAMAGED "sbedd"


namespace Game{
  class bus_event: public godot::Node{
  GDCLASS(bus_event, godot::Node)

    protected:
      static void _bind_methods();
  };
}

#endif