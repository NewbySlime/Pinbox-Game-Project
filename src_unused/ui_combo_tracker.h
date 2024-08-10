#ifndef UI_COMBO_TRACKER_HEADER
#define UI_COMBO_TRACKER_HEADER

#include "godot_macro.h"
#include "game_global_variable.h"
#include "subautoload_compatibles.h"

#include "godot_cpp/classes/margin_container.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/classes/progress_bar.hpp"
#include "godot_cpp/classes/texture_progress_bar.hpp"

#include "godot_cpp/variant/node_path.hpp"

namespace Game{
  class ui_combo_tracker: public godot_sac::MarginContainer_sac{
  GDCLASS(ui_combo_tracker, godot_sac::MarginContainer_sac)
    GD_PROPERTY_VAR(godot::NodePath, label)
    GD_PROPERTY_VAR(godot::NodePath, progress_bar)
    GD_PROPERTY_VAR(godot::NodePath, combo_logo)

    private:
      Game::game_global_variable* _g_var;

      godot::Label* _label;
      godot::TextureProgressBar* _progress_bar;
      godot::TextureRect* _combo_logo;

      double _progress_bar_min_value;
      double _progress_bar_max_value;

      void _change_progress_bar(double value);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      void update_combo_progress(float _val);
      void set_combo(uint32_t multiplier);

      void set_combo_type(uint32_t type);
  };
}

#endif