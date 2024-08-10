#include "logger.h"
#include "game_custom_input.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/viewport.hpp"

#include "godot_cpp/core/math.hpp"


using namespace Game;
using namespace godot;




void game_custom_input::_update_mouse_pos(){
  if(!_g_var->is_in_game_scene())
    return;

  Camera2D* _main_scene_camera = _g_var->get_main_scene_camera();
  Camera3D* _main_view_camera = _g_var->get_main_view_camera();
  RayCast3D* _main_view_pointer = _g_var->get_main_view_pointer();
  SubViewport* _main_scene_viewport = _g_var->get_main_scene_viewport();

  Rect2 _main_scene_viewport_size = _g_var->get_main_scene_viewport_size();
  Vector3 _main_scene_viewport_pos = _g_var->get_main_scene_viewport_pos();
  Vector2 _screen_pos = Vector2(_main_scene_viewport_pos.x, _main_scene_viewport_pos.y) - (Vector2(1, -1) * _main_scene_viewport_size.get_size() / 2);

  Vector2 _relative_mouse_pos = Vector2(_main_view_mouse_global_pos.x, _main_view_mouse_global_pos.y) - _screen_pos;
  _relative_mouse_pos /= _main_scene_viewport_size.get_size();
  _relative_mouse_pos *= _main_scene_viewport->get_size();
  _relative_mouse_pos *= Vector2(1, -1);
  
  _main_scene_mouse_global_pos = (_relative_mouse_pos * (Vector2(1,1) / _main_scene_camera->get_zoom())) + _main_scene_camera->get_global_position();
}


void game_custom_input::_bind_methods(){
  ADD_SIGNAL(MethodInfo(
    SIGNAL_GAME_CUSTOM_INPUT_CUSTOM_INPUT,
    PropertyInfo(Variant::OBJECT, "input_event")
  ));
}


void game_custom_input::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _g_var = sa_container->get_singleton_node<game_global_variable>();
  }
}

void game_custom_input::_on_gamenode_ready(){

}


void game_custom_input::_input(const Ref<InputEvent>& _event){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    if(_event->is_class("InputEventMouseMotion")){
      Camera2D* _main_scene_camera = _g_var->get_main_scene_camera();

      if(_g_var->is_in_game_scene()){
        Camera3D* _main_view_camera = _g_var->get_main_view_camera();
        RayCast3D* _main_view_pointer = _g_var->get_main_view_pointer();
        SubViewport* _main_scene_viewport = _g_var->get_main_scene_viewport();

        Vector2 _mouse_position = get_viewport()->get_mouse_position();
        Vector3 _project_dir = _main_view_camera->project_ray_normal(_mouse_position);
        Vector3 _project_origin = _main_view_camera->project_ray_origin(_mouse_position);

        _main_view_pointer->set_target_position(_project_dir * 10000);
        _main_view_pointer->set_global_position(_project_origin);

        _main_view_pointer->force_raycast_update();
        _main_view_mouse_global_pos = _main_view_pointer->get_collision_point();

        _update_mouse_pos();
      }
      else if(_main_scene_camera){
        _main_scene_mouse_global_pos = _main_scene_camera->get_global_mouse_position();
      }
    }

    emit_signal(SIGNAL_GAME_CUSTOM_INPUT_CUSTOM_INPUT, _event.ptr());
  }
}

void game_custom_input::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _update_mouse_pos();
  }
}


Vector2 game_custom_input::get_main_scene_global_pos(){
  return _main_scene_mouse_global_pos;
}