#ifndef THREAD_CONTEXT_DATA_HEADER
#define THREAD_CONTEXT_DATA_HEADER

#include "map"
#include "thread"


namespace GameUtils::Threading{
  template<typename T_data> class thread_context_data{
    private:
      T_data _def_data;
      std::map<std::thread::id, T_data> _data_thread_map;

    protected:
      virtual void _on_this_deleted_data_iteration(T_data _thread_data){}

    public:
      thread_context_data(T_data default_data){
        _def_data = default_data;
      }

      ~thread_context_data(){
        for(auto _pair: _data_thread_map)
          _on_this_deleted_data_iteration(_pair.second);

        _data_thread_map.clear();
      }

      void add_last_data(T_data data){
        _data_thread_map[std::this_thread::get_id()] = data;
      }

      T_data get_last_data(){
        auto _iter = _data_thread_map.find(std::this_thread::get_id());
        if(_iter == _data_thread_map.end())
          return _def_data;

        return _iter->second;
      }
  };
};


#endif