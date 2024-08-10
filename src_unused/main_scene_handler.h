#ifndef MAIN_SCENE_HANDLER_HEADER
#define MAIN_SCENE_HANDLER_HEADER

#include "godot_macro.h"
#include "subautoload_container.h"

#include "damage_indicator_handler.h"
#include "game_global_variable.h"
#include "pinbox.h"
#include "item_server.h"
#include "item_world_entity_2d_handler.h"


#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/classes/sprite2d.hpp"

#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"


namespace Game{
  class main_scene_handler: public godot_sac::Node2D_sac{
  GDCLASS(main_scene_handler, godot_sac::Node2D_sac)
    GD_PROPERTY_VAR(godot::NodePath, init_margin)

    GD_PROPERTY_VAR(godot::NodePath, player_max_edge_margin)

    GD_PROPERTY_VAR(godot::NodePath, level_low_edge)
    GD_PROPERTY_VAR(godot::NodePath, level_high_edge)

    GD_PROPERTY_VAR(godot::NodePath, camera_path)
    GD_PROPERTY_VAR(godot::NodePath, player_path)

    GD_PROPERTY_VAR(godot::NodePath, map_boundary)

    GD_PROPERTY_VAR(godot::NodePath, hitreg_container)
    GD_PROPERTY_VAR(godot::NodePath, damage_indicator_container)
    GD_PROPERTY_VAR(godot::NodePath, iwe2d_container)

    GD_PROPERTY_VAR(double, smooth_scroll_time)

    GD_PROPERTY_VAR(godot::PackedVector2Array, smooth_scroll_bezier) 

    private:
      Game::Effect::damage_indicator_handler* _di_handler;
      Game::hit_register_handler* _hitreg_handler;
      Game::game_custom_input* _custom_input;
      Game::game_global_variable* _g_var;

      godot::Camera2D* _camera;
      Game::pinbox* _player;

      double _camera_max;
      double _init_margin;

      double _player_max_edge_margin;

      double _player_max_init_scroll;
      double _player_min_init_scroll;
      double _player_max_scroll;
      double _player_min_scroll;

      godot::Vector2 _map_boundary_pos;

      godot::Timer* _scroll_smooth_timer;
      bool _do_scroll = false;
      bool _do_scroll_cancel = false;
      godot::Vector2 _camera_previous_pos;


      void _scroll_calculate_pos();
      void _check_player_pos();

      void _update_viewport_size();

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      main_scene_handler();
      ~main_scene_handler();

      void _physics_process(double delta) override;
  };
}

#endif