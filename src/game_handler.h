#ifndef GAME_HANDLER_HEADER
#define GAME_HANDLER_HEADER

#include "game_context_listener_helper.h"
#include "game_scene_handler.h"
#include "godot_macro.h"
#include "interface_helper.h"
#include "player_actor.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/sub_viewport.hpp"


#define SIGNAL_GAME_HANDLER_GAME_STARTING "sghgs"
#define SIGNAL_GAME_HANDLER_RESUMING "sghr"
#define SIGNAL_GAME_HANDLER_PAUSING "sgp"
#define SIGNAL_GAME_HANDLER_INITIALIZE_GAME_OBJECT "sghigo"


namespace Game{
  class game_handler: public godot::SubViewport,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(game_handler, godot::SubViewport)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(game_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(game_handler)
  ATTR_INTERFACE_HELPER_INHERIT(game_handler)
    
    private:
      Game::game_scene_handler* _game_scene_handler;
      GameUtils::game_context_listener_helper* _context_helper;

      void _game_scene_changing();
      void _game_scene_done_changing(gdvar_custom_object object);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

  };
}

#endif