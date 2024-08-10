#ifndef MODIFIER_FACTORY_HEADER
#define MODIFIER_FACTORY_HEADER

#include "algorithm_random.h"
#include "custom_memory.h"
#include "I_item_factory.h"
#include "I_modifier.h"
#include "I_modifier_database_helper.h"
#include "interface_helper.h"
#include "item_server.h"
#include "modifier_container_item.h"
#include "modifier_database.h"
#include "rng_server.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"
#include "memory"
#include "string"



namespace Game::Item::Factory{
  class modifier_factory: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::Item::Factory::I_item_factory,
    public Game::I_subautoload_compatible{
  GDCLASS(modifier_factory, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(modifier_factory)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(modifier_factory)
  ATTR_INTERFACE_HELPER_INHERIT(modifier_factory)

    GD_PROPERTY_VAR(godot::String, modifier_factory_JSON)

    GD_PROPERTY_VAR(int, min_modifier_combination)
    GD_PROPERTY_VAR(int, max_modifier_combination)
  
    private:
      Game::rng_server* _rng_server;

      Game::Item::modifier_database* _modifier_database;
      Game::Item::item_server* _item_server;

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;

    public:
      void _notification(int code);

      void prep_item(Game::Item::item_inst_id id) override;

      void randomize_item_stats(Game::Item::item_inst_id id) override;
  };

  typedef std::pair<std::shared_ptr<Game::Modifiers::I_modifier>, Game::Item::I_modifier_database_helper*>(*create_modifier_callback)(Game::Item::Factory::modifier_factory* container);
  typedef GameUtils::Memory::custom_memory*(*parse_modifier_data_callback)(godot::Dictionary* json_data);
}

#endif