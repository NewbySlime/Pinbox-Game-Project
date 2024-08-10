#ifndef ENTITY_RESOURCE_DATABASE_HEADER
#define ENTITY_RESOURCE_DATABASE_HEADER

#include "custom_memory.h"
#include "entity_resource_data.h"
#include "godot_macro.h"
#include "id_types.h"

#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/node.hpp"

#include "memory"


namespace Game::Entity{
  class entity_resource_database: public godot::Node{
  GDCLASS(entity_resource_database, godot::Node)

    GD_PROPERTY_VAR(godot::String, entity_resource_data_folder)

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;


      std::map<godot::String, Game::Entity::entity_resource_data*> _res_data_map;

      void _process_entity_res_data(godot::Dictionary* _data_dict);

      void _load_resource_data();

    protected:
      static void _bind_methods();

    public:
      void _notification(int code);

      void _ready() override;

      entity_resource_data* get_resource_data(uint64_t entity_type, godot::String res_id);
  };
}

#endif