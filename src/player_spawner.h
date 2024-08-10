#ifndef PLAYER_SPAWNER_HEADER
#define PLAYER_SPAWNER_HEADER

#include "game_handler.h"
#include "I_spawner.h"
#include "interface_helper.h"
#include "subautoload_container.h"


namespace Game{
  class player_actor;
  class player_spawner: public godot::Node2D,
    public GameUtils::Interface::I_interface_class,
    public Game::I_spawner,
    public Game::I_subautoload_compatible{
  GDCLASS(player_spawner, godot::Node2D)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(player_spawner)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(player_spawner)
  ATTR_INTERFACE_HELPER_INHERIT(player_spawner)

    private:
      Game::game_handler* _game_handler;

      Game::player_actor* _player_actor;

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container);
      void _on_gamenode_ready() override;
    
    public:
      void _notification(int code);

      void spawn_entity() override;

      godot::Vector2 get_spawn_position() override;

  };
}

#endif