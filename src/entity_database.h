#ifndef ENTITY_DATABASE_HEADER
#define ENTITY_DATABASE_HEADER

#include "custom_memory.h"
#include "entity_data.h"
#include "godot_macro.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"
#include "memory"
#include "vector"


namespace Game::Entity{
  class entity_database: public godot::Node{
  GDCLASS(entity_database, godot::Node)

    GD_PROPERTY_VAR(godot::String, entity_class_type_json)
    GD_PROPERTY_VAR(godot::String, entity_folder_data)

    private:
      class _entity_metadata{
        public:
          godot::Ref<godot::PackedScene> entity_pckscene;
          
          // using this because it uses subdata parsing
          std::shared_ptr<GameUtils::Memory::custom_memory> memory_data;
          Game::Entity::entity_data* data;
      };

      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      std::map<godot::String, godot::Ref<godot::PackedScene>> _entity_scene_list;
      
      std::map<
        uint64_t, 
        std::map<godot::String, _entity_metadata>*
      > _entity_metadata_map;


      void _parse_file_data(const godot::Dictionary& _data_part);

      void _load_entity_data_config();
      void _load_entity_scene_type();


    protected:
      static void _bind_methods();

    public:
      void _notification(int code);

      void _ready() override;

      Game::Entity::entity_data* get_entity_data(uint64_t type, const godot::String& entity_id);

      // returned PackedScene, always instantiate as entity_object
      godot::Ref<godot::PackedScene> get_entity_pckscene(uint64_t type, const godot::String& entity_id);

      std::shared_ptr<std::vector<Game::Entity::entity_data*>> find_entity_data(uint64_t type);

      godot::Ref<godot::PackedScene> get_entity_type_scene(const godot::String& scene_id);
  };
}

#endif