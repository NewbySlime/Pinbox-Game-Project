#include "main_view_handler.h"

#include "game_global_variable.h"
#include "game_custom_input.h"

#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/mesh.hpp"
#include "godot_cpp/classes/plane_mesh.hpp"


using namespace Game;
using namespace godot;


void main_view_handler::_change_resolution(MeshInstance3D* viewer, SubViewport* subviewport, double min_size){
  godot::Ref<Mesh> _current_mesh = viewer->get_mesh();
  if(_current_mesh->is_class("PlaneMesh")){
    PlaneMesh* _plane_mesh = (PlaneMesh*)_current_mesh.ptr();
    Vector2 _mesh_size = _plane_mesh->get_size();
    double _min_mesh_val = Math::min(_mesh_size.x, _mesh_size.y);

    _mesh_size /= _min_mesh_val;
    Vector2 _resolution_size = _mesh_size * min_size;
    subviewport->set_size(_resolution_size);
  }
}


void main_view_handler::_bind_methods(){
  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_view_camera)
  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_view_pointer)

  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_scene_viewport_mesh)
  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_scene_viewport)
  GD_PROPERTY_BIND(main_view_handler, Variant::FLOAT, main_scene_min_size)

  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_ui_viewport)
  GD_PROPERTY_BIND(main_view_handler, Variant::NODE_PATH, main_ui_viewport_mesh)
  GD_PROPERTY_BIND(main_view_handler, Variant::FLOAT, main_ui_min_size)

  GD_PROPERTY_BIND(main_view_handler, Variant::FLOAT, max_main_scene_camera_speed)

  GD_PROPERTY_BIND(main_view_handler, Variant::FLOAT, max_main_ui_viewport_tilt)
}


void main_view_handler::_bind_subautoload(subautoload_container* sa_container){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    _g_var = sa_container->get_singleton_node<game_global_variable>();
  }
}

void main_view_handler::_on_gamenode_ready(){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    Camera3D* _main_view_camera = get_node<Camera3D>(main_view_camera);
    RayCast3D* _main_view_pointer = get_node<RayCast3D>(main_view_pointer);

    MeshInstance3D* _main_scene_viewport_mesh = get_node<MeshInstance3D>(main_scene_viewport_mesh);
    SubViewport* _main_scene_viewport =  get_node<SubViewport>(main_scene_viewport);
    _change_resolution(_main_scene_viewport_mesh, _main_scene_viewport, main_scene_min_size);

    _main_ui_viewport_mesh = get_node<MeshInstance3D>(main_ui_viewport_mesh);
    SubViewport* _main_ui_viewport = get_node<SubViewport>(main_ui_viewport);
    _change_resolution(_main_ui_viewport_mesh, _main_ui_viewport, main_ui_min_size);

    Rect2 _mesh_rect;
    godot::Ref<Mesh> _current_mesh = _main_scene_viewport_mesh->get_mesh();
    if(_current_mesh->is_class("PlaneMesh")){
      PlaneMesh* _m = (PlaneMesh*)_current_mesh.ptr();
      _mesh_rect = Rect2(Vector2(0,0), _m->get_size());
    }

    _g_var->main_view_camera = _main_view_camera;
    _g_var->main_scene_viewport = _main_scene_viewport;
    _g_var->main_scene_viewport_size = _mesh_rect;
    _g_var->main_scene_viewport_pos = _main_scene_viewport_mesh->get_global_position();
    _g_var->main_view_pointer = _main_view_pointer;

    _main_ui_viewport_mesh_def_rot = _main_ui_viewport_mesh->get_rotation_degrees();
  }
}


void main_view_handler::_physics_process(double delta){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    Camera2D* _main_scene_camera = _g_var->get_main_scene_camera();
    if(!_main_scene_camera)
      return;
      
    Vector2 _camera_global_pos = _main_scene_camera->get_global_position();
    Vector2 _camera_speed = (_camera_global_pos - _last_main_camera_pos)/delta;
    double _camera_scalar_speed = _camera_speed.length();
    Vector2 _camera_dir = _camera_speed.normalized();

    if(_camera_scalar_speed > max_main_scene_camera_speed)
      _camera_scalar_speed = max_main_scene_camera_speed;

    double _val = _camera_scalar_speed / max_main_scene_camera_speed;
    _camera_speed = _camera_dir * _val;

    _main_ui_viewport_mesh->set_rotation_degrees(
      _main_ui_viewport_mesh_def_rot +
      Vector3(
        _camera_speed.y * max_main_ui_viewport_tilt,
        0,
        0
      )
    );

    _last_main_camera_pos = _camera_global_pos;
  }
}