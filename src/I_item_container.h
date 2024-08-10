#ifndef I_ITEM_CONTAINER_HEADER
#define I_ITEM_CONTAINER_HEADER

#include "id_types.h"
#include "interface_helper.h"
#include "item_location_data_struct.h"

#include "stdint.h"

#define SIGNAL_ITEM_CONTAINER_ON_INDEX_ADD "sicoia"
#define SIGNAL_ITEM_CONTAINER_ON_INDEX_CHANGED "sicoic"
#define SIGNAL_ITEM_CONTAINER_ON_INDEX_REMOVED "sicoir"

#define SIGNAL_ITEM_CONTAINER_ON_CONTAINER_RESIZED "sicocr"

namespace Game::Item{
  class item_server;
  class item_object;
  class I_item_container{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_item_container)
  
    protected:
      friend Game::Item::item_server;

      virtual bool _check_index(int index){ return false; }
      virtual int _get_new_index(){ return -1; }

      virtual uint64_t _remove_item(int index){ return 0; }
      virtual int _add_item(const Game::Item::item_object* item, int index = -1, bool force = false, uint64_t* _removed_id = NULL){ return -1; }

    public:
      virtual const Game::Item::item_object* get_item(int index){ return 0; }
      virtual uint64_t get_item_id(int index){ return 0; }

      virtual uint64_t get_container_id(){ return 0; }
      virtual uint32_t get_container_type(){ return 0; }

      virtual bool is_item_exist(int index){ return false; }
      virtual bool is_item_can_exist(const Game::Item::item_object* item){ return false; }
      virtual bool is_item_can_exist(uint64_t item_id){ return false; }

      virtual bool set_item_filter_type(uint32_t new_type){ return false; }
      virtual uint32_t get_item_filter_type(){ return 0; }

      virtual int empty_slot_count(){ return 0; }
      virtual int get_container_size(){ return 0; }
      virtual void resize_container(int new_size){}

      virtual bool add_to_container(Game::Item::item_inst_id id, int index = -1){ return false; }
      virtual Game::Item::item_inst_id remove_from_container(int index){ return ITEM_ID_NULL; }

      virtual bool swap_item(int index, Game::Item::item_position_data& pos_data){ return false; }

      virtual bool drop_item(int index){ return false; }

      virtual void drop_all_item(){}
      virtual void drop_all_item_filter(uint32_t type_filter){}
  };


  class item_container_on_index_changed_param{
    private:
      Game::Item::item_position_data _pos_item_data;
    
    public:
      item_container_on_index_changed_param(Game::Item::item_position_data pos_data){
        _pos_item_data = pos_data;
      }

      const Game::Item::item_position_data* get_item_pos(){
        return &_pos_item_data;
      }
  };

  class item_container_on_index_add_param{
    private:
      uint64_t _added_item_id;
      Game::Item::item_position_data _pos_item_data;

    public:
      item_container_on_index_add_param(Game::Item::item_position_data pos_data, uint64_t added_item_id){
        _added_item_id = added_item_id;
        _pos_item_data = pos_data;
      }

      const Game::Item::item_position_data* get_item_pos(){
        return &_pos_item_data;
      }

      uint64_t get_added_item_id(){
        return _added_item_id;
      }
  };

  class item_container_on_index_remove_param{
    private:
      uint64_t _removed_item_id;
      Game::Item::item_position_data _pos_item_data;

    public:
      item_container_on_index_remove_param(Game::Item::item_position_data pos_data, uint64_t removed_item_id){
        _pos_item_data = pos_data;
        _removed_item_id = removed_item_id;
      }

      const Game::Item::item_position_data* get_item_pos(){
        return &_pos_item_data;
      }

      uint64_t get_removed_item_id(){
        return _removed_item_id;
      }
  };
}

#endif