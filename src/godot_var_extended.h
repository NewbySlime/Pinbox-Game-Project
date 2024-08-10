#ifndef GODOT_VARIANT_EXTENDED_HEADER
#define GODOT_VARIANT_EXTENDED_HEADER

#include "godot_cpp/variant/variant.hpp"

#include "stdint.h"


typedef uint64_t gdvar_custom_object;

#define GDVAR_CUSTOM_TO(variable, type) \
  (type*)variable

#define GDVAR_CUSTOM_FROM(variable) \
  (uint64_t)&variable

#define GDVAR_CUSTOM_FROM_PTR(variable) \
  (uint64_t)variable

#define GDVAR_CUSTOM_VARIANT_TYPE Variant::INT

#endif