#ifndef GAME_GLOBAL_VARIABLE_HEADER
#define GAME_GLOBAL_VARIABLE_HEADER

#include "godot_macro.h"

#include "hit_register_handler.h"

#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/mesh_instance3d.hpp"
#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/ray_cast3d.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/texture.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"

#include "godot_cpp/variant/rect2.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"

#include "map"
#include "memory"

#define __GAME_GLOBAL_VARIABLE_CREATE(type, var_name)\
  private: type var_name;\
  public: type get_##var_name(){return var_name;}


namespace Game{
  class game_global_variable;
  class main_scene_handler;
  class ui_score_board;
  class main_view_handler;

  struct ui_combo_tracker_data_struct{
    private:
      friend game_global_variable;
      godot::Ref<godot::Resource> _logo_sprite;

    public:
      godot::Ref<godot::Texture> logo_sprite;

      double
        base_limit,
        base_multiplier,
        multiplier_decrement;

      int max_multiplier;
  };


  class game_global_variable: public godot::Node{
  GDCLASS(game_global_variable, godot::Node)
    friend main_scene_handler;
    friend ui_score_board;
    friend main_view_handler;

    __GAME_GLOBAL_VARIABLE_CREATE(godot::Camera2D*, main_scene_camera)
    __GAME_GLOBAL_VARIABLE_CREATE(godot::SubViewport*, main_scene_viewport);

    __GAME_GLOBAL_VARIABLE_CREATE(godot::RayCast3D*, main_view_pointer)
    __GAME_GLOBAL_VARIABLE_CREATE(godot::Camera3D*, main_view_camera)

    __GAME_GLOBAL_VARIABLE_CREATE(godot::Rect2, main_scene_viewport_size)
    __GAME_GLOBAL_VARIABLE_CREATE(godot::Vector3, main_scene_viewport_pos)

    GD_PROPERTY_VAR(godot::String, combo_tracker_data_json)
    GD_PROPERTY_VAR(godot::String, player_init_data_json)

    private:
      godot::ResourceLoader* _res_loader;
      std::map<uint32_t, ui_combo_tracker_data_struct> _combo_tracker_data_map;
      std::vector<uint32_t> _weapon_data_list;

      void _process_combo_tracker_data_dict(godot::Dictionary* _data_dict);

      void _process_player_init_data_dict(godot::Dictionary* _data_dict);

    protected:
      static void _bind_methods();

    public:
      game_global_variable();

      void _ready() override;
      
      const ui_combo_tracker_data_struct* get_ui_combo_tracker_data(uint32_t type);

      bool is_in_game_scene();
  };
}

#endif