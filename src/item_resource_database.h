#ifndef ITEM_RESOURCE_DATABASE_HEADER
#define ITEM_RESOURCE_DATABASE_HEADER

#include "custom_memory.h"
#include "item_resource_data.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "memory"


namespace Game::Item{
  class item_resource_database: public godot::Node{
  GDCLASS(item_resource_database, godot::Node)

    GD_PROPERTY_VAR(godot::String, item_resource_data_folder)

    private:
      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      godot::Dictionary _item_res_database_dict;

      void _process_item_res_data_dict(godot::Dictionary* _data_dict);

      std::shared_ptr<std::vector<Item::item_resource_data*>> _get_data_filter(godot::Dictionary* data_filter, bool return_first = false);

    protected:
      static void _bind_methods();

    public:
      void _notification(int code);

      void _ready() override;

      Item::item_resource_data* get_item_res_data(const godot::String& item_type, const godot::String& item_id);

      std::shared_ptr<std::vector<Item::item_resource_data*>> find_type(const godot::String& item_type);

  };
}

#endif