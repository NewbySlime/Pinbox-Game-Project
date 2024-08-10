#ifndef ITEM_WORLD_ENTITY_2D_HANDLER_HEADER
#define ITEM_WORLD_ENTITY_2D_HANDLER_HEADER

#include "game_handler.h"
#include "godot_macro.h"
#include "godot_signal.h"
#include "interface_helper.h"
#include "item_dropper.h"
#include "item_location_data_struct.h"
#include "item_picker_area.h"
#include "item_world_entity_2d.h"
#include "rng_server.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "map"
#include "set"


#define SIGNAL_IWE2DH_ON_ENTITY_DELETED "siwe2dhoned"
#define SIGNAL_IWE2DH_ON_PICKER_DELETED "siwe2dhonpd"
#define SIGNAL_IWE2DH_ON_DROPPER_DELETED "siwe2dhondd"


namespace Game::Item{
  class item_server;
  class item_world_entity_2d_handler: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_world_entity_2d_handler, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_world_entity_2d_handler)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_world_entity_2d_handler)
  ATTR_INTERFACE_HELPER_INHERIT(item_world_entity_2d_handler)

    GD_PROPERTY_VAR(double, random_speed)
    GD_PROPERTY_VAR(double, maximum_throw_speed)

    GD_PROPERTY_VAR(godot::String, item_dropper_pckscene)
    GD_PROPERTY_VAR(godot::String, item_picker_area_pckscene)
    GD_PROPERTY_VAR(godot::String, item_world_entity_pckscene)

    private:
      struct _item_dropper_metadata{
        uint64_t _item_dropper_id;
        std::set<uint64_t> _bound_containers_id;

        Game::Item::item_dropper* _node;
      };

      struct _item_picker_area_metadata{
        uint64_t _item_picker_id;
        uint64_t _bound_container_id;

        Game::Item::item_picker_area* _node;
      };

      struct _item_world_entity_metadata{
        uint64_t _item_id;

        Game::Item::item_world_entity_2d* _node;
      };


      Game::game_handler* _game_handler;
      Game::rng_server* _rng_server;


      friend Game::Item::item_server;
      Game::Item::item_server* _item_server;

      uint64_t _drop_container_id;
      godot::Node2D* _root_drop;

      godot::Ref<godot::PackedScene> _item_dropper_pckscene;
      godot::Ref<godot::PackedScene> _item_picker_area_pckscene;
      godot::Ref<godot::PackedScene> _item_world_entity_pckscene;

      // these ids are based on their respective objects
      std::map<uint64_t, _item_dropper_metadata*> _item_dropper_map;
      std::map<uint64_t, _item_picker_area_metadata*> _item_picker_area_map;

      // ids are the bound containers from where the item would be dropped
      std::map<uint64_t, _item_dropper_metadata*> _bound_dropper_containers_map;

      // ids are the bound container where the item would be added when the picker picked something
      std::map<uint64_t, _item_picker_area_metadata*> _bound_picker_containers_map;

      // ids are the I_item-s id with respective world entity objects in it
      std::map<uint64_t, _item_world_entity_metadata*> _dropped_item_map;


      Game::Item::item_dropper_inst_id _get_new_dropper_id();
      Game::Item::item_picker_inst_id _get_new_picker_id();

      void _on_item_removed(gdsignal_param_ref item_id);

      void _on_item_container_deleted(gdsignal_param_ref container_id);

    protected:
      static void _bind_methods();

      void _bind_subautoload(Game::subautoload_container* sa_container) override;
      void _on_gamenode_ready() override;

      friend Game::Item::item_dropper;
      friend Game::Item::item_world_entity_2d;
      friend Game::Item::item_picker_area;

      bool _delete_dropper(Game::Item::item_dropper_inst_id id, bool inst_already_deleted = false);
      bool _delete_iwe2d(Game::Item::iwe2d_inst_id id, bool inst_already_deleted = false);
      bool _delete_picker_area(Game::Item::item_picker_inst_id id, bool inst_already_deleted = false);

    public:
      item_world_entity_2d_handler();

      void _notification(int code);
      
      void drop_item(uint64_t id, godot::Vector2 additional_speed = godot::Vector2(0, 0));
      void drop_item_random(uint64_t id);

      bool pick_item(Game::Item::iwe2d_inst_id id, Game::Item::item_picker_inst_id picker_id);

      void bind_container_dropper(uint64_t container_id, uint64_t dropper_id);
      void bind_container_picker(uint64_t container_id, uint64_t picker_id);

      void unbind_container_dropper(uint64_t container_id, uint64_t dropper_id);
      void unbind_container_picker(uint64_t container_id, uint64_t picker_id);

      Game::Item::item_dropper* create_item_dropper();
      Game::Item::item_picker_area* create_item_picker_area();

      Game::Item::item_dropper* get_item_dropper(uint64_t dropper_id);
      Game::Item::item_picker_area* get_item_picker_area(uint64_t picker_id);

      bool delete_item_dropper(Game::Item::item_dropper_inst_id id);
      bool delete_item_picker_area(Game::Item::item_picker_inst_id id);
  };


  class iwe2d_handler_dropper_deleted_param{
    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::item_dropper_inst_id _id;

    public:
      Game::Item::item_dropper_inst_id get_id(){ return _id; }
  };
  
  class iwe2d_handler_entity_deleted_param{
    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::iwe2d_inst_id _id;

    public:
      Game::Item::iwe2d_inst_id get_id(){ return _id; }
  };

  class iwe2d_handler_picker_deleted_param{
    private:
      friend Game::Item::item_world_entity_2d_handler;
      Game::Item::item_picker_inst_id _id;

    public:
      Game::Item::item_picker_inst_id get_id(){ return _id; }
  };
}

#endif