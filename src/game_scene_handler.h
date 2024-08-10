#ifndef GAME_SCENE_HANDLER_HEADER
#define GAME_SCENE_HANDLER_HEADER

#include "interface_helper.h"
#include "subautoload_container.h"


#define SIGNAL_GAME_HANDLER_ON_CONTEXT_CHANGING "sghocc"
#define SIGNAL_GAME_HANDLER_ON_CONTEXT_DONE_CHANGING "sghocdc"

namespace Game{
  class game_handler;
  class game_scene_handler: public godot::Node, 
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(game_scene_handler, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(game_scene_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(game_scene_handler)
  ATTR_INTERFACE_HELPER_INHERIT(game_scene_handler)

    GD_PROPERTY_VAR(godot::String, INIT_game_contextID)
    GD_PROPERTY_VAR(godot::String, DEBUG_game_contextID)
    
    private:
      friend game_handler;

      // don't expose this to public, handle by listening to game UI
      // by handling by game_handler, spaghetti code will be avoided, as it handles by game_handler not by the UI or any other control nodes
      void _change_to_context(godot::String contextID);


    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_subautoload_ready();

    public:
      void _notification(int code);

  };

  class game_handler_on_context_changed{
    private:
      friend game_scene_handler;
      godot::String _context_ID;

    public:
      godot::String get_context_ID(){
        return _context_ID;
      }
  };
}

#endif