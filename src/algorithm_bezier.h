#ifndef ALGORITHM_BEZIER_HEADER
#define ALGORITHM_BEZIER_HEADER

#include "godot_cpp/variant/packed_vector2_array.hpp"


namespace Game::Misc::Algorithm{
  /// @brief The title says it all. Used for smoothing out animation or other timed based values.
  /// @param array The "graph" array that will affect the smoothing or results
  /// @param t Current normalized "time" value, number between 0 - 1
  /// @param inverse Inverse the output (1 - 0)?
  /// @param clamped Clamp the value between min and max values in the "graph" array
  /// @return The processed value in Vector2 (based on each axis in the "graph" array)
  godot::Vector2 bezier_curve_function(godot::PackedVector2Array* array, double t, bool inverse = false, bool clameped = false);
}

#endif