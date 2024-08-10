#ifndef STATIC_VARIABLE_STORAGE_HEADER
#define STATIC_VARIABLE_STORAGE_HEADER

#include "algorithm_node.h"
#include "custom_memory.h"

#include "godot_cpp/classes/node.hpp"

#include "map"


#define GET_STATIC_DATA(static_data_type, target_shared_var) \
  Game::static_variable_storage* _static_storage = Game::Misc::Algorithm::find_autoload<Game::static_variable_storage>(this, NULL); \
  if(_static_storage){ \
    if(!_static_storage->get_static_variable<static_data_type>()) \
      _static_storage->create_static_variable<static_data_type>(); \
    \
    target_shared_var = _static_storage->get_static_variable_shared<static_data_type>(); \
  } \
  else{ \
    INIT_ASSERT() \
    ERR_ASSERT("Cannot find static_variable_storage.") \
  }


namespace Game{
  class static_variable_storage: public godot::Node{
  GDCLASS(static_variable_storage, godot::Node)

    private:
      std::map<godot::String, std::shared_ptr<GameUtils::Memory::custom_memory>> _static_variable_map;

    protected:
      static void _bind_methods();

    public:
      // T should have godot::String get_id() function
      template<typename T> bool create_static_variable(){
        auto _iter = _static_variable_map.find(T::get_id());
        if(_iter == _static_variable_map.end())
          return false;

        _static_variable_map[T::get_id()] = GameUtils::Memory::custom_memory::create_shared<T>();
        return true;
      }


      template<typename T> T* get_static_variable(){
        auto _iter = _static_variable_map.find(T::get_id());
        if(_iter == _static_variable_map.end())
          return NULL;

        return _iter->second->get_data<T>();
      }

      template<typename T> std::shared_ptr<GameUtils::Memory::custom_memory> get_static_variable_shared(){
        auto _iter = _static_variable_map.find(T::get_id());
        if(_iter == _static_variable_map.end())
          return std::make_shared<GameUtils::Memory::custom_memory>();

        return _iter->second;
      }
  };
}

#endif