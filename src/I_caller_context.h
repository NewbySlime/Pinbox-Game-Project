#ifndef I_CALLER_CONTEXT_HEADER
#define I_CALLER_CONTEXT_HEADER

#include "interface_helper.h"

#include "godot_cpp/classes/node.hpp"

#define CALLER_CONTEXT_WRAPPER(p_target_node, context_data, function, ...){ \
  auto __macro_function_var = function; \
  I_caller_context* __macro_interface_context = NULL; \
  INTERFACE_GET(p_target_node, I_caller_context, __macro_interface_context) \
  if(__macro_interface_context){ \
    __macro_interface_context->add_current_context(context_data); \
    __macro_function_var(__VA_ARGS__); \
    __macro_interface_context->delete_current_context(); \
  } \
  else{ \
    __macro_function_var(__VA_ARGS__); \
  } \
}


namespace GameUtils::Function{
  struct caller_data{
    godot::Node* node = NULL;
  };

  class I_caller_context{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_caller_context)

    public:
      virtual void add_current_context(const caller_data& caller_context){}
      virtual void delete_current_context(){}

      virtual const caller_data* get_current_context(){ return NULL; }
  };
}

#endif