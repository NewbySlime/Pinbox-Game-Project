#include "logger.h"
#include "main_scene_handler.h"

#include "game_custom_input.h"
#include "game_global_variable.h"
#include "hit_register_handler.h"
#include "damage_indicator_handler.h"
#include "algorithm_bezier.h"

#include "godot_cpp/core/math.hpp"

#include "godot_cpp/classes/marker2d.hpp"
#include "godot_cpp/classes/engine.hpp"


using namespace godot;
using namespace Game;
using namespace Game::Effect;
using namespace Game::Misc::Algorithm;



void main_scene_handler::_scroll_calculate_pos(){
  Vector2 _player_pos = _player->get_position();
  Vector2 _camera_pos = _camera->get_position();
  Vector2 _next_camera_pos = _camera_pos;

  // making sure camera not viewing out of bounds
  if(_player_pos.y < _player_min_scroll)
    _next_camera_pos.y = _player_min_scroll - _init_margin;
  else if(_player_pos.y > _player_max_scroll)
    _next_camera_pos.y = _player_max_scroll + _init_margin - _camera_max;
  else{
    // if player goes too fast (jittery)
    double _bottom_dist = _player_pos.y - _camera_pos.y;
    double _top_dist = (_camera_pos.y + _camera_max) - _player_pos.y;
    if(_bottom_dist < _player_max_edge_margin){
      _next_camera_pos.y = _player_pos.y + _player_max_edge_margin - _player_min_init_scroll;

      _scroll_smooth_timer->stop();
    }
    else if(_top_dist < _player_max_edge_margin){
      _next_camera_pos.y = _player_pos.y - _player_max_edge_margin - _player_max_init_scroll;

      _scroll_smooth_timer->stop();
    }
    else{
      // normal state
      double _player_y_relative = _player_pos.y - _camera_pos.y;

      if(_player_y_relative < _player_min_init_scroll)
        _next_camera_pos.y = _player_pos.y - _player_min_init_scroll;
      else if(_player_y_relative > _player_max_init_scroll)
        _next_camera_pos.y = _player_pos.y - _player_max_init_scroll;
    }
  }
    
  if(!_scroll_smooth_timer->is_stopped()){
    Vector2 _val = bezier_curve_function(&smooth_scroll_bezier, _scroll_smooth_timer->get_time_left() / smooth_scroll_time);

    Vector2 _delta2 = _next_camera_pos - _camera_previous_pos;
    _delta2 *= _val;

    _next_camera_pos = _delta2 + _camera_previous_pos;
  }
  else if(_do_scroll_cancel)
    _do_scroll = false;
  
  _camera->set_position(_next_camera_pos);

  _check_player_pos();
}

void main_scene_handler::_check_player_pos(){
  Vector2 _player_pos = _player->get_position();

  bool _cancel_pos_calculation = false;

  if(_player_pos.y > _player_min_scroll && _player_pos.y < _player_max_scroll){
    Vector2 _camera_pos = _camera->get_position();
    double _player_y_relative = _player_pos.y - _camera_pos.y;

    if(_player_y_relative < _player_min_init_scroll || _player_y_relative > _player_max_init_scroll){
      _do_scroll = true;
      _do_scroll_cancel = false;
      _camera_previous_pos = _camera->get_position();
      _scroll_smooth_timer->start(smooth_scroll_time);

      return;
    }
    else if(_do_scroll){
      _cancel_pos_calculation = true;
      //_scroll_smooth_timer->stop();
    }
  }
  else if(_do_scroll)
    _cancel_pos_calculation = true;

  if(_cancel_pos_calculation)
    _do_scroll_cancel = true;
}


void main_scene_handler::_update_viewport_size(){
  Vector2 _viewport_size = get_viewport_rect().get_size();
  double _val_width = _viewport_size.x / _map_boundary_pos.x;
  
  _camera->set_zoom(Vector2(_val_width, _val_width));

  _camera_max = (_viewport_size / _camera->get_zoom()).y;
  _player_max_init_scroll = _camera_max - _init_margin;
  _player_min_init_scroll = _init_margin;

  if(_camera->get_global_position().y < _camera_max){
    _camera->set_position(Vector2(
      _camera->get_position().x,
      -_camera_max
    ));
  }
}


void main_scene_handler::_bind_methods(){
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, init_margin)

  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, player_max_edge_margin)

  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, level_low_edge) 
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, level_high_edge)

  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, camera_path)
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, player_path)

  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, map_boundary)
  
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, hitreg_container)
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, damage_indicator_container)
  GD_PROPERTY_BIND(main_scene_handler, Variant::NODE_PATH, iwe2d_container)

  GD_PROPERTY_BIND(main_scene_handler, Variant::FLOAT, smooth_scroll_time)

  GD_PROPERTY_BIND(main_scene_handler, Variant::PACKED_VECTOR2_ARRAY, smooth_scroll_bezier)

  ClassDB::bind_method(D_METHOD("_update_viewport_size"), &main_scene_handler::_update_viewport_size);
}


void main_scene_handler::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _di_handler = sa_container->get_singleton_node<damage_indicator_handler>();
    _hitreg_handler = sa_container->get_singleton_node<hit_register_handler>();
    _custom_input = sa_container->get_singleton_node<game_custom_input>();
    _g_var = sa_container->get_singleton_node<game_global_variable>();
  }
}

void main_scene_handler::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    Node2D* _hitreg_container = NULL;
    CREATE_GET_NODE_TESTER(_hitreg_container, hitreg_container, Node2D)
    _hitreg_handler->set_root(_hitreg_container);

    Node2D* _di_container = NULL;
    CREATE_GET_NODE_TESTER(_di_container, damage_indicator_container, Node2D)
    _di_handler->set_parent_target(_di_container);

    item_server* _item_server = get_subautoload_root()->get_singleton_node<item_server>();
    Node2D* _iwe2d_container = NULL;
    CREATE_GET_NODE_TESTER(_iwe2d_container, iwe2d_container, Node2D)
    _item_server->get_iwe2d_handler()->set_root_drop(_iwe2d_container);

    get_viewport()->connect("size_changed", Callable(this, "_update_viewport_size"));

    _scroll_smooth_timer->set_autostart(false);
    _scroll_smooth_timer->set_one_shot(true);
    _scroll_smooth_timer->set_timer_process_callback(Timer::TIMER_PROCESS_PHYSICS);

    ADD_CHILD_SA(_scroll_smooth_timer);

    _player = get_node<pinbox>(player_path);
    _camera = get_node<Camera2D>(camera_path);

    _map_boundary_pos = get_node<Marker2D>(map_boundary)->get_position();
    _init_margin = get_node<Marker2D>(init_margin)->get_position().y;

    _player_max_edge_margin = get_node<Marker2D>(player_max_edge_margin)->get_position().y;

    Marker2D* _level_high_edge = get_node<Marker2D>(level_high_edge);
    _player_max_scroll = _level_high_edge->get_position().y - _init_margin;

    Marker2D* _level_low_edge = get_node<Marker2D>(level_low_edge);
    _player_min_scroll = _level_low_edge->get_position().y + _init_margin;

    _update_viewport_size();
    _scroll_calculate_pos();

    _g_var->main_scene_camera = _camera;
  }
}


main_scene_handler::main_scene_handler(){
  _scroll_smooth_timer = new Timer();
}

main_scene_handler::~main_scene_handler(){
  delete _scroll_smooth_timer;
}


void main_scene_handler::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(!_do_scroll)
      _check_player_pos();
    
    if(_do_scroll)
      _scroll_calculate_pos();
  }
}