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
      template<typename T> void change_data(){
        // check class
        T::custom_data_check();

        if(_cm_data_address)
          delete _cm_data_address;

        _cm_data_size = sizeof(T);
        _cm_data_address = (custom_data*)new T();
      }

      template<typename T> T* get_data(){
        return (T*)_cm_data_address;
      }

      uint32_t get_data_size(){
        return _cm_data_size;
      }

      template<typename T> static std::shared_ptr<custom_memory> create_shared(){
        // class check
        T::custom_data_check();

        std::shared_ptr<custom_memory> _res = std::make_shared<custom_memory>();
        _res->change_data<T>();

        return _res;
      }

      template<typename T> static custom_memory* create(){
        // class check
        T::custom_data_check();

        custom_memory* _res = new custom_memory();
        _res->change_data<T>();

        return _res;
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