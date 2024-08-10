#ifndef GAME_CONTEXT_LISTENER_HELPER_HEADER
#define GAME_CONTEXT_LISTENER_HELPER_HEADER

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "game_scene_handler.h"
#include "interface_helper.h"
#include "logger.h"
#include "node_free_filter.h"
#include "subautoload_container.h"
#include "structure_holder_node.h"


namespace GameUtils{
  class game_context_listener_helper: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(game_context_listener_helper, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(game_context_listener_helper)
  ATTR_NODE_FREE_FILTER_INHERIT(game_context_listener_helper)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(game_context_listener_helper)
  ATTR_INTERFACE_HELPER_INHERIT(game_context_listener_helper)
  
    private:
      godot::String _parent_class_name;

      Game::game_scene_handler* _game_scene_handler;

      void _on_game_context_changing(gdvar_custom_object _param);

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

    public:
      void _notification(int code);    

  };
}

#endif