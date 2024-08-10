#ifndef MAIN_UI_HANDLER_HEADER
#define MAIN_UI_HANDLER_HEADER

#include "godot_macro.h"
#include "subautoload_compatibles.h"

#include "game_counter.h"
#include "ui_combo_tracker.h"

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/label.hpp"

#include "godot_cpp/variant/node_path.hpp"


namespace Game{
  class ui_score_board: public godot_sac::Control_sac{
  GDCLASS(ui_score_board, godot_sac::Control_sac)
    GD_PROPERTY_VAR(godot::NodePath, score_label)
    GD_PROPERTY_VAR(godot::NodePath, combo_tracker_bumper)

    private:
      Game::game_counter* _game_counter;
      godot::Label* _score_label;

      Game::ui_combo_tracker* _combo_tracker_bumper;

      void _on_score_incremented();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;
  };
}

#endif