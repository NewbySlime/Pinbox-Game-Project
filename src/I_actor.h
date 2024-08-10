#ifndef I_ACTOR_HEADER
#define I_ACTOR_HEADER

#include "godot_cpp/classes/node.hpp"

#include "entity_data.h"
#include "I_action_class.h"
#include "I_movement.h"
#include "I_vital_object.h"
#include "id_types.h"
#include "interface_helper.h"


namespace Game{
  /// @brief An interface class to gain or setting information about certain actors.
  class I_actor{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_actor)

    public:
      ~I_actor(){}

      virtual void bind_to_entity(Game::Entity::entity_inst_id entity_id){}

  };
}

#endif