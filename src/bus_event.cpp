#include "bus_event.h"

#include "godot_cpp/classes/engine.hpp"


using namespace Game;
using namespace godot;


void bus_event::_bind_methods(){
  ADD_SIGNAL(MethodInfo(
    SIGNAL_BUS_EVENT_DAMAGEABLE_DAMAGED,
    PropertyInfo(Variant::VECTOR2, "origin"),
    PropertyInfo(Variant::FLOAT, "damage")
  ));
}