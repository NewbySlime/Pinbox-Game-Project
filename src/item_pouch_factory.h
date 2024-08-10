#ifndef ITEM_POUCH_FACTORY_HEADER
#define ITEM_POUCH_FACTORY_HEADER

#include "I_item_factory.h"
#include "interface_helper.h"
#include "item_server.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"


namespace Game::Item::Factory{
  class item_pouch_factory: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::Item::Factory::I_item_factory,
    public Game::I_subautoload_compatible{
  GDCLASS(item_pouch_factory, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_pouch_factory)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_pouch_factory)
  ATTR_INTERFACE_HELPER_INHERIT(item_pouch_factory)

    private:
      Game::Item::item_server* _item_server;

      bool _is_item_prepped(Game::Item::item_object* _item);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;

    public:
      void _notification(int code);

      void prep_item(Game::Item::item_inst_id id) override;
  };
}

#endif