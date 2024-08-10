#include "logger.h"

#include "ui_combo_tracker.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/image_texture.hpp"


using namespace Game;
using namespace godot;


void ui_combo_tracker::_change_progress_bar(double value){
  double _val = _progress_bar_max_value - _progress_bar_min_value;
  _val *= value;
  _val += _progress_bar_min_value;

  _progress_bar->set_value(_val);
}


void ui_combo_tracker::_bind_methods(){
  GD_PROPERTY_BIND(ui_combo_tracker, Variant::NODE_PATH, label)
  GD_PROPERTY_BIND(ui_combo_tracker, Variant::NODE_PATH, progress_bar)
  GD_PROPERTY_BIND(ui_combo_tracker, Variant::NODE_PATH, combo_logo)
}


void ui_combo_tracker::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _g_var = sa_container->get_singleton_node<game_global_variable>();
  }
}


void ui_combo_tracker::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _label = get_node<Label>(label);
    _progress_bar = get_node<TextureProgressBar>(progress_bar);
    _combo_logo = get_node<TextureRect>(combo_logo);

    _progress_bar_min_value = _progress_bar->get_min();
    _progress_bar_max_value = _progress_bar->get_max();
  }
}


void ui_combo_tracker::update_combo_progress(float _val){
  _change_progress_bar(_val);
}

void ui_combo_tracker::set_combo(uint32_t multiplier){
  Array _paramarr;{
    _paramarr.append(multiplier);
  }

  _label->set_text(String("X{0}").format(_paramarr));
}


void ui_combo_tracker::set_combo_type(uint32_t type){
  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append((uint64_t)this);
    _paramarr.append((uint64_t)_g_var);
  }

  LOG_ASSERT(String("this {0}, g_var {1}").format(_paramarr))

  const ui_combo_tracker_data_struct* _type_data = _g_var->get_ui_combo_tracker_data(type);
  if(!_type_data)
    return;

  _combo_logo->set_texture(_type_data->logo_sprite);
}