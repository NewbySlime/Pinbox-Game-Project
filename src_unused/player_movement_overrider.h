#ifndef PLAYER_MOVEMENT_OVERRIDER_HEADER
#define PLAYER_MOVEMENT_OVERRIDER_HEADER

#include "overrider.h"

#include "game_custom_input.h"
#include "subautoload_compatibles.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"


namespace Game::Overrider{
  class player_movement_overrider: public godot::Node, overrider{
  GDCLASS(player_movement_overrider, godot::Node)
    private:
      Game::game_custom_input* _custom_input;

    protected:
      static void _bind_methods();

      void _on_added() override;
      void _on_removed() override;

      void _bind_subautoload(Game::subautoload_container* container);
      void _on_gamenode_ready();

    public:
      void custom_input(godot::Object* _event);

      uint32_t get_overrider_type(){ return ote_movement_overrider; } 
  };
}

#endif