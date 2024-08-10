#ifndef MAIN_VIEW_HANDLER_HEADER
#define MAIN_VIEW_HANDLER_HEADER

#include "godot_macro.h"
#include "subautoload_compatibles.h"

#include "game_global_variable.h"

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"

#include "godot_cpp/variant/node_path.hpp"


namespace Game{
  class main_view_handler: public godot_sac::Node3D_sac{
  GDCLASS(main_view_handler, godot_sac::Node3D_sac)
    GD_PROPERTY_VAR(godot::NodePath, main_view_camera)
    GD_PROPERTY_VAR(godot::NodePath, main_view_pointer)

    GD_PROPERTY_VAR(godot::NodePath, main_scene_viewport_mesh)
    GD_PROPERTY_VAR(godot::NodePath, main_scene_viewport)
    GD_PROPERTY_VAR(double, main_scene_min_size)

    GD_PROPERTY_VAR(godot::NodePath, main_ui_viewport)
    GD_PROPERTY_VAR(godot::NodePath, main_ui_viewport_mesh)
    GD_PROPERTY_VAR(double, main_ui_min_size)

    GD_PROPERTY_VAR(double, max_main_scene_camera_speed)

    GD_PROPERTY_VAR(double, max_main_ui_viewport_tilt)

    GD_PROPERTY_VAR(godot::NodePath, game_node_subviewport)

    private:
      Game::game_global_variable* _g_var;
      godot::MeshInstance3D* _main_ui_viewport_mesh;

      godot::Vector3 _main_ui_viewport_mesh_def_rot;
      godot::Vector2 _last_main_camera_pos;

      void _change_resolution(godot::MeshInstance3D* viewer, godot::SubViewport* subviewport, double min_size);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      void _physics_process(double delta) override;

      void change_game_node(godot::Node* game_node);
  };
}

#endif