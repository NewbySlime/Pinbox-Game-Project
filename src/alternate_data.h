#ifndef ALTERNATE_DATA_HEADER
#define ALTERNATE_DATA_HEADER

#include "custom_memory.h"
#include "subdata_container.h"

#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class alternate_data: public GameUtils::ParsingHelper::subdata_container, public GameUtils::Memory::custom_data{

    public:
      ~alternate_data(){}
    
      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);
      
      void copy_subdata_to(GameUtils::Memory::custom_memory* mem);


      template<typename subdata_class> const subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<const GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }
  };
}

#endif