#ifndef I_GRAPHICS_HANDLER_HEADER
#define I_GRAPHICS_HANDLER_HEADER

#include "godot_cpp/classes/node.hpp"


namespace Game{
  class I_graphics_handler{
    public:
      virtual void bind_graphics_object(godot::Object* obj, uint32_t type) = 0;
  };
}

#endif