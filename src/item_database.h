#ifndef ITEM_DATABASE_HEADER
#define ITEM_DATABASE_HEADER

#include "godot_macro.h"
#include "item_data.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"
#include "memory"
#include "string"


namespace Game::Item{
  class item_database: public godot::Node{
  GDCLASS(item_database, godot::Node)
    GD_PROPERTY_VAR(godot::String, item_data_folder)

    private:
      godot::Dictionary _item_database_dict;

      void _process_item_data_dict(godot::Dictionary* _data_dict);

      // return_first parameter means returns first valid object
      std::shared_ptr<std::vector<Item::item_data*>> _get_data_filter(godot::Dictionary* data_filter, bool return_first = false);

    protected:
      static void _bind_methods();

    public:
      void _notification(int code);

      void _ready() override;

      Item::item_data* get_item_data(const godot::String& item_type, const godot::String& item_id);

      std::shared_ptr<std::vector<Game::Item::item_data*>> find_type(const godot::String& item_type);
  };
}

#endif