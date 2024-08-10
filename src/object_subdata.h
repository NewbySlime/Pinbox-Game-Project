#ifndef OBJECT_SUBDATA_HEADER
#define OBJECT_SUBDATA_HEADER

#include "subdata_container.h"


namespace GameUtils::DataStorage{
  class object_subdata: public GameUtils::ParsingHelper::subdata_container{
    public:
      template<typename subdata_class> void set_subdata(subdata_class* from){
        __set_subdata<subdata_class>(from);
      }

      template<typename subdata_class, typename... constructor_args> void create_subdata(constructor_args... args){
        __create_subdata<subdata_class, constructor_args...>(args...);
      }


      template<typename subdata_class> const subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<const GameUtils::Memory::custom_memory> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }


      template<typename subdata_class> subdata_class* get_subdata_mutable(){
        return __get_subdata<subdata_class>();
      }
      
  };
}

#endif