#ifndef PLAYER_ACTOR_HEADER
#define PLAYER_ACTOR_HEADER

#include "entity_actor.h"
#include "I_actor.h"
#include "interface_helper.h"
#include "player_spawner.h"
#include "ui_container.h"

#include "godot_cpp/classes/node.hpp"


namespace Game{
  class player_actor: public Game::Entity::entity_actor{
  GDCLASS(player_actor, Game::Entity::entity_actor)

    GD_PROPERTY_VAR(godot::String, action_overrider_scene)
    GD_PROPERTY_VAR(godot::String, movement_overrider_scene)

    private:
      Game::Entity::entity_server* _entity_server;
      Game::UI::ui_container* _ui_container;
      
      Game::Entity::entity_inst_id _player_entity_id = ENTITY_ID_NULL;


      void _set_player_entity_object(Game::Entity::entity_inst_id entity_id);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void bind_to_entity(Game::Entity::entity_inst_id entity_id) override;

      Game::Entity::entity_object* get_player_entity();

      void spawn_player(Game::player_spawner* spawner);

  };
}

#endif