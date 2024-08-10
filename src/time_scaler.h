#ifndef TIME_SCALER_HEADER
#define TIME_SCALER_HEADER

#include "defines.h"
#include "godot_macro.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/timer.hpp"

#include "godot_cpp/variant/packed_vector2_array.hpp"


#define _TIME_SCALER_SIGNAL_PROCESS_BYPASS "_pb"
#define _TIME_SCALER_SIGNAL_PHYSICS_PROCESS_BYPASS "_ppb"


namespace Game{
  class time_scaler: public godot::Node{
  GDCLASS(time_scaler, godot::Node)

    GD_PROPERTY_VAR(bool, time_scale_smooth_bezier_use_x)
    GD_PROPERTY_VAR(godot::PackedVector2Array, time_scale_smooth_bezier)
    
    private:
      double _smooth_time_wait_time;
      double _smooth_time_current_time;

      double _smooth_time_scale_target;
      double _smooth_time_scale_prev;

      double _current_scale;
      double _prev_scale;
      double _next_scale;

      bool _paused;

      double _get_scale_dist_normalized(double from, double to, double val);
      double _get_scale_dist(double from, double to, double norm_val);

      void _set_time_scale(double scale);
      void _set_smooth_timer(double time, double seek = -1);
    
    protected:
      static void _bind_methods();

    public:
      void _ready() override;

      void _process(double delta) override;
      void _physics_process(double delta) override;

      void set_time_scale(double scale);
      void set_time_scale_smooth(double scale, double smooth_time = 0.4);

      void continue_game(bool slow_gradually = false);
      void pause_game(bool slow_gradually = false);

      double get_process_time_bypass();
      double get_physics_process_time_bypass();
  };
}

#endif