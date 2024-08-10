#include "logger.h"

#include "ui_score_board.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"
#include "godot_cpp/classes/viewport.hpp"

#include "godot_cpp/core/class_db.hpp"

#include "godot_cpp/variant/vector2.hpp"


using namespace Game;
using namespace godot;


void ui_score_board::_on_score_incremented(){
  score_data* _score_data = _game_counter->get_score_data();

  Array _paramarr;{
    _paramarr.append(_score_data->get_score());
  }
  _score_label->set_text(String("Score: {0}").format(_paramarr));

  int _multiplier_combo = _game_counter->get_combo_multiplier_bumper();
  double _multiplier_val = _game_counter->get_combo_val_bumper();

  _combo_tracker_bumper->set_combo(_multiplier_combo);
  _combo_tracker_bumper->update_combo_progress(_multiplier_val);
}


void ui_score_board::_bind_methods(){
  GD_PROPERTY_BIND(ui_score_board, Variant::NODE_PATH, score_label)
  GD_PROPERTY_BIND(ui_score_board, Variant::NODE_PATH, combo_tracker_bumper)

  ClassDB::bind_method(D_METHOD("_on_score_incremented"), &ui_score_board::_on_score_incremented);
}


void ui_score_board::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _game_counter = sa_container->get_singleton_node<game_counter>();
  }
}


void ui_score_board::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    INIT_ASSERT()
    LOG_ASSERT("test")
    _game_counter->connect(SIGNAL_GAME_COUNTER_SCORE_ON_INCREMENT, Callable(this, "_on_score_incremented"));
    LOG_ASSERT("test")
    _score_label = get_node<Label>(score_label);
    LOG_ASSERT("test")
    
    CREATE_GET_NODE_TESTER(_combo_tracker_bumper, combo_tracker_bumper, ui_combo_tracker);
    //_combo_tracker_bumper = get_node<ui_combo_tracker>(combo_tracker_bumper);
    LOG_ASSERT("test")
    _combo_tracker_bumper->set_combo_type(sat_bumper);

    LOG_ASSERT("test")
    _on_score_incremented();
    LOG_ASSERT("test end")
  }
}