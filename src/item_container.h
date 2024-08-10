#ifndef ITEM_CONTAINER_HEADER
#define ITEM_CONTAINER_HEADER

#include "godot_cpp/classes/node.hpp" 

#include "item_object.h"
#include "I_item_container.h"
#include "item_location_data_struct.h"

#include "vector"

namespace Game::Item{
  class item_server;
  class item_container: public godot::Node, public Game::Item::I_item_container{
  GDCLASS(item_container, godot::Node)

    protected:
      static void _bind_methods();

      friend item_server;
      item_server* _server;
      uint64_t _container_id;

      uint32_t _item_filter_type;

      std::vector<uint64_t> _item_list;
      int _item_count;

      void _emit_signal_index_removed(Game::Item::item_position_data& pos_data, uint64_t item_id);
      void _emit_signal_index_added(Game::Item::item_position_data& pos_data, uint64_t item_id);
      void _emit_signal_index_changed(Game::Item::item_position_data& pos_data);
      void _emit_signal_container_resized();

      bool _check_index(int index) override;
      virtual int _get_new_index() override;

      virtual uint64_t _remove_item(int index) override;
      virtual int _add_item(const Game::Item::item_object* item, int index = -1, bool force = false, uint64_t* _removed_id = NULL) override;

    public:
      item_container();

      const Game::Item::item_object* get_item(int index) override;
      uint64_t get_item_id(int index) override;

      uint64_t get_container_id() override;
      uint32_t get_container_type() override{ return ict_generic; }

      bool is_item_exist(int index) override;
      bool is_item_can_exist(const Game::Item::item_object* item) override;
      bool is_item_can_exist(uint64_t item_id) override;

      bool set_item_filter_type(uint32_t new_type) override;
      uint32_t get_item_filter_type() override;

      int empty_slot_count() override;
      virtual int get_container_size() override;
      virtual void resize_container(int new_size) override;

      bool add_to_container(Game::Item::item_inst_id id, int index = -1) override;
      Game::Item::item_inst_id remove_from_container(int index) override;

      bool swap_item(int index, Game::Item::item_position_data& pos_data) override;

      bool drop_item(int index) override;

      void drop_all_item() override;
      void drop_all_item_filter(uint32_t filter) override;
  };
}

#endif