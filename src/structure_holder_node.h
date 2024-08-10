#ifndef STRUCTURE_HOLDER_NODE_HEADER
#define STRUCTURE_HOLDER_NODE_HEADER

#include "godot_macro.h"

#include "godot_cpp/classes/node.hpp"


namespace GameUtils::Helper{
  class structure_holder_node: public godot::Node{
  GDCLASS(structure_holder_node, godot::Node)

  GD_PROPERTY_VAR(bool, reparent_retain_global_position)

    protected:
      static void _bind_methods();

    public:
      structure_holder_node();

      void _ready() override;
  };
}

#endif