#ifndef I_ENTITY_OBJECT_HEADER
#define I_ENTITY_OBJECT_HEADER

#include "entity_data.h"
#include "entity_resource_data.h"
#include "id_types.h"
#include "interface_helper.h"
#include "object_subdata.h"

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/node3d.hpp"


namespace Game::Entity{
  class entity_data;
  class I_entity_object{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_entity_object)

    public:
      virtual godot::Node3D* get_space3d_pivot_node() const{
        return NULL;
      }

      virtual godot::Node2D* get_space2d_pivot_node() const{
        return NULL;
      }


      virtual Game::Entity::entity_inst_id get_instance_id() const{
        return ENTITY_ID_NULL; 
      }

      virtual const Game::Entity::entity_data* get_entity_data() const{
        return NULL;
      }

      virtual const Game::Entity::entity_resource_data* get_entity_resource_data() const{
        return NULL;
      }

      virtual const GameUtils::DataStorage::object_subdata* get_entity_subdata() const{
        return NULL;
      }
  };
}

#endif