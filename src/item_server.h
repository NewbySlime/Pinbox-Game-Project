#ifndef ITEM_SERVER_HEADER
#define ITEM_SERVER_HEADER

#include "godot_macro.h"
#include "I_item_container.h"
#include "I_item_factory.h"
#include "interface_helper.h"
#include "item_container.h"
#include "item_container_premade_data.h"
#include "item_data.h"
#include "item_database.h"
#include "item_location_data_struct.h"
#include "item_object.h"
#include "item_resource_data.h"
#include "item_resource_database.h"
#include "item_world_entity_2d.h"
#include "logger.h"
#include "subautoload_container.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"  
#include "vector"


#define SIGNAL_ITEM_SERVER_ITEM_REMOVED "sisir"
#define SIGNAL_ITEM_SERVER_ITEM_CONTAINER_REMOVING "sisicr"


namespace Game::Item{
  enum premade_item_container_type{
    pict_player = 1,
    pict_player_weapon = 2
  };



  class item_server: public godot::Node,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(item_server, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(item_server)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(item_server)
  ATTR_INTERFACE_HELPER_INHERIT(item_server)

    GD_PROPERTY_VAR(godot::String, item_container_premade_data_json)

    GD_PROPERTY_VAR(godot::String, item_world_entity_2d_handler_pckscene)
    GD_PROPERTY_VAR(godot::PackedStringArray, item_container_pckscene_array)

    private:
      struct _item_metadata{
        public:
          std::map<uint64_t, item_position_data> container_ids;
          uint64_t _main_container_id;

          item_object* _self;
      };

      struct _item_container_metadata{
        public:
          I_item_container* _container_obj;

          godot::Node* _node_obj;
      };

      std::shared_ptr<GameUtils::Memory::custom_memory> _static_data;

      std::map<uint32_t, godot::Ref<godot::PackedScene>> _item_container_type_pckscene;


      Game::Item::item_world_entity_2d_handler* _iwe2d_handler;
      Game::Item::item_database* _item_database;
      Game::Item::item_resource_database* _item_res_database;

      std::map<uint32_t, Game::Item::item_container_premade_data*> _item_cont_premade_data_map;

      std::map<uint64_t, _item_container_metadata*> _container_map;
      uint64_t _current_container_id_iter;
      uint64_t _container_count;

      std::map<uint64_t, _item_metadata*> _item_metadata_map;
      uint64_t _current_item_id_iter;
      uint64_t _item_count;


      uint64_t _find_new_item_id();
      uint64_t _add_new_item(Game::Item::item_object* _item);

      uint64_t _find_new_container_id();
      I_item_container* _create_new_container(int size, uint32_t type, uint64_t new_id = ITEM_CONTAINER_ID_NULL);

      bool _add_to_container(Game::Item::I_item_container* _container, uint64_t item_id, int index_pos = -1);
      uint64_t _remove_from_container(Game::Item::I_item_container* _container, int index_pos);

      bool _clear_container(Game::Item::I_item_container* _container);

      void _parse_item_container_premade_json(godot::Dictionary* _data_dict);

    protected:
      static void _bind_methods();

      void _on_gamenode_ready() override;

      friend Game::Item::Factory::I_item_factory;
      Game::Item::item_object* _get_item_object_mutable(Game::Item::item_inst_id id);

    public:
      item_server();

      void _notification(int code);

      const Game::Item::item_object* get_item_object(uint64_t id);
      Game::Item::I_item_container* get_item_container(uint64_t id);
      godot::Node* get_item_container_node(uint64_t id);

      uint64_t find_container_has_item(uint64_t item_id);

      bool set_item_container_filter_type(Game::Item::item_container_inst_id id, uint32_t new_type);

      bool swap_item(Game::Item::item_position_data& pos_from, Game::Item::item_position_data& pos_to);

      bool add_to_container(uint64_t container_id, uint64_t item_id, int index = -1);

      void remove_from_all_container(uint64_t item_id, std::set<uint64_t> _exception = std::set<uint64_t>());
      uint64_t remove_from_container(uint64_t container_id, uint64_t item_id);
      uint64_t remove_from_container_index(uint64_t container_id, int index);
      
      bool delete_item(Game::Item::item_inst_id item_id);
      bool delete_item_container(Game::Item::item_container_inst_id container_id);

      bool drop_item(Game::Item::item_position_data& item_pos);

      const Game::Item::item_object* create_item_object(uint64_t type, const godot::String& item_id);

      Game::Item::I_item_container* create_item_container(int container_size, uint32_t container_type, uint64_t* new_id = NULL);

      Game::Item::item_world_entity_2d_handler* get_iwe2d_handler();
  };

  class item_server_item_removed_param{
    private:
      friend item_server;
      Game::Item::item_inst_id _id;

    public:
      Game::Item::item_inst_id get_id(){ return _id; }
  };

  class item_server_item_container_removed_param{
    private:
      friend item_server;
      Game::Item::item_container_inst_id _id;

    public:
      Game::Item::item_container_inst_id get_container_id(){ return _id; }
  };
}

#endif