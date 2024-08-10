#ifndef GAME_CUSTOM_INPUT_HEADER
#define GAME_CUSTOM_INPUT_HEADER

#include "godot_macro.h"
#include "subautoload_compatibles.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/camera2d.hpp"
#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/classes/ray_cast3d.hpp"
#include "godot_cpp/classes/sub_viewport.hpp"
#include "godot_cpp/classes/input_event_mouse.hpp"

#include "godot_cpp/variant/transform3d.hpp"
#include "godot_cpp/variant/rect2.hpp"

#include "godot_cpp/variant/node_path.hpp"

#define SIGNAL_GAME_CUSTOM_INPUT_CUSTOM_INPUT "sci"


namespace Game{
  class game_custom_input: public godot_sac::Node_sac{
  GDCLASS(game_custom_input, godot_sac::Node_sac)

    private:
      godot::Vector3 _main_view_mouse_global_pos;   // mouse position in 3D space (viewer)
      godot::Vector2 _main_scene_mouse_global_pos;  // mouse position in main_scene

      void _update_mouse_pos();

    protected:
      static void _bind_methods();
      
      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

    public:
      void _input(const godot::Ref<godot::InputEvent>& _event) override;
      void _physics_process(double delta) override;

      godot::Vector2 get_main_scene_global_pos();
  };
}

#endif