#ifndef CUSTOM_MEMORY_HEADER
#define CUSTOM_MEMORY_HEADER

#include "stdint.h"
#include "stdlib.h"

#include "memory"


namespace GameUtils::Memory{
  class custom_data{
    public:
      custom_data(){}

      static void custom_data_check(){}

      virtual ~custom_data(){}
  };

  class custom_memory{
    private:
      uint32_t _cm_data_size;
      custom_data* _cm_data_address = NULL;

    public:
      template<typename T, typename... c_args> void change_data(c_args... args){
        // check class
        T::custom_data_check();

        if(_cm_data_address)
          delete _cm_data_address;

        _cm_data_size = sizeof(T);
        _cm_data_address = (custom_data*)new T(args...);
      }

      template<typename T> T* get_data(){
        return (T*)_cm_data_address;
      }

      uint32_t get_data_size(){
        return _cm_data_size;
      }


      template<typename T, typename... c_args> static std::shared_ptr<custom_memory> create_shared(c_args... args){
        // class check
        T::custom_data_check();

        std::shared_ptr<custom_memory> _res = std::make_shared<custom_memory>();
        _res->change_data<T, c_args...>(args...);

        return _res;
      }
    
      static std::shared_ptr<custom_memory> create_shared(){
        return create_shared<custom_data>();
      }


      template<typename T, typename... c_args> static custom_memory* create(c_args... args){
        // class check
        T::custom_data_check();

        custom_memory* _res = new custom_memory();
        _res->change_data<T, c_args...>(args...);

        return _res;
      }

      static custom_memory* create(){
        return create<custom_data>();
      }


      custom_memory(){
        change_data<custom_data>();
      }

      ~custom_memory(){
        delete _cm_data_address;
      }
  };
}

#endif