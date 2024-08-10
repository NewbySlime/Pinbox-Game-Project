#ifndef GODOT_SIGNAL_HEADER
#define GODOT_SIGNAL_HEADER

#include "godot_var_extended.h"

#include "godot_cpp/variant/variant.hpp"


typedef gdvar_custom_object gdsignal_param_ref;

// converts from an object reference to godot-readable data
#define SIGNAL_PARAM_TO_CUSTOMPTR(variable)\
  GDVAR_CUSTOM_FROM_PTR(variable)

// converts from an object to godot-readable data
#define SIGNAL_PARAM_TO_CUSTOM(variable)\
  GDVAR_CUSTOM_FROM(variable)

// converts from godot-readable data to a reference of user-defined type
#define SIGNAL_PARAM_GET_CUSTOMPTR(variable, type)\
  GDVAR_CUSTOM_TO(variable, type)

#define SIGNAL_PARAM_CUSTOM_VARIANT_TYPE GDVAR_CUSTOM_VARIANT_TYPE

#endif