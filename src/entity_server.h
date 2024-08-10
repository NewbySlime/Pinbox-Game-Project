#ifndef ENTITY_SERVER_HEADER
#define ENTITY_SERVER_HEADER

#include "entity_database.h"
#include "entity_item_container_ref.h"
#include "entity_object.h"
#include "entity_resource_database.h"
#include "object_subdata.h"
#include "godot_macro.h"
#include "id_types.h"
#include "item_server.h"
#include "interface_helper.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "map"



#define USAGE_ENTITY_ICONT_MAIN_INVENTORY "MAIN_INV"
#define USAGE_ENTITY_ICONT_LOADOUT_INVENTORY "LOADOUT_INV"


namespace Game::Entity{
  class entity_server: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(entity_server, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(entity_server)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(entity_server)
  ATTR_INTERFACE_HELPER_INHERIT(entity_server)
  
    GD_PROPERTY_VAR(godot::Dictionary, list_entity_packed_scenes)

    private:
      struct _entity_metadata{
        public:
          Game::Entity::entity_object* _this_object;

          GameUtils::DataStorage::object_subdata* _entity_subdata;
      };

      Game::Entity::entity_database* _entity_database;
      Game::Entity::entity_resource_database* _entity_res_database;
      
      Game::Item::item_server* _item_server;

      std::map<Game::Entity::entity_inst_id, _entity_metadata> _object_list_map;
      
      std::map<Game::Item::item_container_inst_id, entity_inst_id> _item_container_owner_refback;


      Game::Item::I_item_container* _create_item_container(Game::Item::inventory_config* config);

      void _init_entity_item_container(Game::Entity::entity_inst_id id);
      void _init_entity_iwe_tools(Game::Entity::entity_inst_id id);

      void _on_item_container_removed(gdvar_custom_object param);


    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

    public:
      void _notification(int code);

      Game::Entity::entity_object* create_entity(uint64_t type, godot::String entity_id);
      Game::Entity::entity_object* create_entity_on(uint64_t type, godot::String entity_id, godot::Node* parent);

      Game::Entity::entity_object* get_entity(Game::Entity::entity_inst_id instance_id);

      bool remove_entity(Game::Entity::entity_inst_id instance_id);

    
    private:
      friend Game::Entity::entity_object;
      void _on_entity_deinstantiated(Game::Entity::entity_object* _object);

  };
} 

#endif