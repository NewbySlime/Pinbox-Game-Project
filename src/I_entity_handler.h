#ifndef I_ENTITY_HANDLER_HEADER
#define I_ENTITY_HANDLER_HEADER

#include "entity_data.h"
#include "entity_resource_data.h"
#include "interface_helper.h"


namespace Game::Entity{
  class I_entity_handler{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_entity_handler)

    public:
      virtual void set_entity_data(const Game::Entity::entity_data* data){}
      virtual void set_entity_resource_data(const Game::Entity::entity_resource_data* res_data){}
  };
}

#endif