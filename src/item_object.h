#ifndef ITEM_OBJECT_HEADER
#define ITEM_OBJECT_HEADER

#include "id_types.h"

#include "item_data.h"
#include "item_resource_data.h"

#include "subdata_container.h"

#include "godot_cpp/variant/string.hpp"


#define ITEM_ID_KEY_WEAPON "weapon"
#define ITEM_ID_KEY_MATERIAL "material"


namespace Game::Item{
  class item_server;
  class item_object: GameUtils::ParsingHelper::subdata_container{
    private:
      friend Game::Item::item_server;

      const Game::Item::item_data* _this_item_data = NULL;
      const Game::Item::item_resource_data* _this_item_res = NULL;
      uint64_t _this_id = ITEM_ID_NULL;
      uint32_t _this_type = 0;

    public:
      godot::String get_item_id() const;

      godot::String get_item_type_str() const;
      uint32_t get_item_type() const;

      const Game::Item::item_data* get_item_data() const;
      const Game::Item::item_resource_data* get_resource_data() const;
      uint64_t get_id() const;

      const godot::String get_item_name() const;
      const godot::String get_description() const;


      template<typename subdata_class> void set_subdata(subdata_class* from){
        __set_subdata<subdata_class>(from);
      }

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