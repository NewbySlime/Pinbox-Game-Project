#ifndef ITEM_DATA_HEADER
#define ITEM_DATA_HEADER

#include "subdata_container.h"


namespace Game::Item{
  class item_database;
  class item_data: public GameUtils::ParsingHelper::subdata_container{
    private:
      friend Game::Item::item_database;
      godot::String _item_id;
      godot::String _item_type;

      godot::String _item_name;
      godot::String _item_desc;

      
      template<typename subdata_class> void _add_new_subdata(godot::Dictionary* _data_dict){
        return __add_new_subdata<subdata_class>(_data_dict);
      }

    public:
      godot::String get_item_id() const;
      godot::String get_item_type() const;

      godot::String get_item_name() const;
      godot::String get_item_description() const;


      template<typename subdata_class> subdata_class* get_subdata_mutable(){
        return __get_subdata<subdata_class>();
      }
      
      template<typename subdata_class> const subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<GameUtils::Memory::custom_memory> get_subdata_shared_mutable(){
        return __get_subdata_shared<subdata_class>();
      }

      template<typename subdata_class> const std::shared_ptr<GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }
  };
}

#endif