#ifndef EQUIPPABLE_FACTORY_HEADER
#define EQUIPPABLE_FACTORY_HEADER

#include "equippable_object.h"
#include "godot_var_extended.h"
#include "id_types.h"
#include "interface_helper.h"
#include "item_object.h"
#include "item_server.h"
#include "subautoload_container.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"


namespace Game::Item::Factory{
  class equippable_factory: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(equippable_factory, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(equippable_factory)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(equippable_factory)
  ATTR_INTERFACE_HELPER_INHERIT(equippable_factory)

    GD_PROPERTY_VAR(godot::String, equippable_object_pckscene)

    GD_PROPERTY_VAR(godot::String, consumables_handler_pckscene)
    GD_PROPERTY_VAR(godot::String, weapon_handler_pckscene)

    private:
      Game::Item::item_server* _current_item_server = NULL;
      Game::Item::item_world_entity_2d_handler* _iwe2d_handler = NULL;

      godot::Ref<godot::PackedScene> _equipable_pckscene = NULL;

      godot::Ref<godot::PackedScene> _consumables_pckscene = NULL;
      godot::Ref<godot::PackedScene> _weapon_pckscene = NULL;

      std::map<Game::Item::item_inst_id, equippable_object*> _object_map;

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* container) override;
      void _on_gamenode_ready() override;

      friend Game::equippable_object;
      void _delete_object(Game::equippable_object* object, bool already_deleted);

    public:
      void _notification(int code);

      // return NULL if already exist, use get_equippable_object
      Game::equippable_object* create_equippable_object(Game::Item::item_inst_id id);
      // return NULL if not found
      Game::equippable_object* get_equippable_object(Game::Item::item_inst_id id);

      void delete_object(Game::equippable_object* object);
  };
}

#endif