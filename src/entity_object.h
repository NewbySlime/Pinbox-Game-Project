#ifndef ENTITY_OBJECT_HEADER
#define ENTITY_OBJECT_HEADER

#include "entity_data.h"
#include "entity_database.h"
#include "entity_resource_data.h"
#include "I_entity_object.h"
#include "id_types.h"
#include "object_subdata.h"
#include "interface_helper.h"
#include "subdata_container.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"


#define SIGNAL_ENTITY_OBJECT_ON_DESPAWNED "seoods"

#define USAGE_GENERIC_ENTITY_ACTION "ACTION_OVERRIDER"
#define USAGE_GENERIC_ENTITY_MOVEMENT "MOVEMENT_OVERRIDER"


namespace Game::Entity{
  class entity_server;
  class entity_object: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::Entity::I_entity_object,
    public Game::I_subautoload_compatible{
  GDCLASS(entity_object, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(entity_object)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(entity_object)
  ATTR_INTERFACE_HELPER_INHERIT(entity_object)

    GD_PROPERTY_VAR(godot::NodePath, space2d_pivot_point)
    GD_PROPERTY_VAR(godot::NodePath, space3d_pivot_point)

    private:
      friend Game::Entity::entity_server;
      entity_server* _this_server;

      std::map<uint64_t, godot::Node2D*> _space2d_child_list;
      std::map<uint64_t, godot::Node3D*> _space3d_child_list;

      godot::Node2D* _space2d_point;
      godot::Node3D* _space3d_point;

      Game::Entity::entity_inst_id _inst_id;

      const Game::Entity::entity_data* _this_data;
      const Game::Entity::entity_resource_data* _this_res_data;
      const GameUtils::DataStorage::object_subdata* _entity_subdata;


      void _on_child_entered(godot::Node* child);
      void _on_child_exited(godot::Node* child);

    protected:
      static void _bind_methods();

      // when creating save files, make sure to create the virtual function (the derived class can use)
      // and the original one (the server can use)

      void _on_gamenode_ready() override;

    public:
      entity_object();

      void _physics_process(double delta) override;

      void _notification(int code);

      godot::Node3D* get_space3d_pivot_node() const override;
      godot::Node2D* get_space2d_pivot_node() const override;

      Game::Entity::entity_inst_id get_instance_id() const override;
      const Game::Entity::entity_data* get_entity_data() const override;
      const Game::Entity::entity_resource_data* get_entity_resource_data() const override;
      const GameUtils::DataStorage::object_subdata* get_entity_subdata() const override;

  };
}

#endif