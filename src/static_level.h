#ifndef STATIC_LEVEL_HEADER
#define STATIC_LEVEL_HEADER

#include "game_handler.h"
#include "I_level.h"
#include "interface_helper.h"
#include "parent_child_watcher_helper.h"
#include "player_spawner.h"
#include "subautoload_container.h"

#include "map"


namespace Game::Level{
  class static_level: public godot::Node2D,
    public GameUtils::Helper::parent_child_watcher_helper,
    public GameUtils::Interface::I_interface_class,
    public Game::I_level,
    public Game::I_subautoload_compatible{
  GDCLASS(static_level, godot::Node2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(static_level)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(static_level)
  ATTR_INTERFACE_HELPER_INHERIT(static_level)
  ATTR_PARENT_CHILD_WATCHER_HELPER_INHERIT(static_level)

    GD_PROPERTY_VAR(godot::NodePath, custom_player_spawner_nodepath)

    private:
      struct _spawner_metadata{
        godot::Node* _spawner_node;
        Game::I_spawner* _spawner_interface;
      };

      Game::game_handler* _game_handler;

      Game::player_spawner* _player_spawner;

      std::map<uint64_t, _spawner_metadata> _world_entity_spawner_map;

      void _on_child_exiting_tree(godot::Node* _node);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

      void _on_child_entered(godot::Node* node) override;
      void _on_child_exited(godot::Node* node) override;

    public:
      void _notification(int code);
      
      void initiate_level() override;
      void clear_level() override;

      void game_start() override;

  };
}

#endif