#ifndef ENTITY_LEVEL_HANDLER_HEADER
#define ENTITY_LEVEL_HANDLER_HEADER

#include "entity_server.h"
#include "interface_helper.h"
#include "subautoload_container.h"


namespace Game::Entity{
  class entity_level_handler: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(entity_level_handler, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(entity_level_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(entity_level_handler)
  ATTR_INTERFACE_HELPER_INHERIT(entity_level_handler)

    private:
      Game::Entity::entity_server* _entity_server;
    
    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

  };
}

#endif