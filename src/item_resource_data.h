#ifndef ITEM_RESOURCE_DATA_HEADER
#define ITEM_RESOURCE_DATA_HEADER

#include "subdata_container.h"

#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class item_resource_database;
  class item_resource_data: public GameUtils::ParsingHelper::subdata_container{
    private:
      friend Game::Item::item_resource_database;
      godot::String _item_res_full_id;

    public:
      godot::String get_item_res_full_id();


    private:
      template<typename subdata_class> void _add_new_subdata(godot::Dictionary* _data_dict){
        return __add_new_subdata<subdata_class>(_data_dict);
      }

    public:
      template<typename subdata_class> const subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<const GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }

      template<typename subdata_class> void add_new_subdata(godot::Dictionary* data_dict){
        __add_new_subdata<subdata_class>(data_dict);
      }

  };
}

#endif