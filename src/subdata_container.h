#ifndef SUBDATA_CONTAINER_HEADER
#define SUBDATA_CONTAINER_HEADER

#include "custom_memory.h"

#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"


#define SUBDATA_OVERRIDE_NAME(origin_class, new_class_name, new_subdata_name) \
class new_class_name: public origin_class{ \
  public: \
    static std::string get_subdata_name_stdstr(){ return new_subdata_name; } \
    static godot::String get_subdata_name(){ return new_subdata_name; } \
};



namespace GameUtils::ParsingHelper{
  class subdata_container{
    protected:
      typedef void(subdata_container::*_subdata_function_copy)(GameUtils::Memory::custom_memory* from, GameUtils::Memory::custom_memory* to) const;
      struct _subdata_metadata{
        public:
          bool _is_reference = false;

          std::weak_ptr<void> _subdata_ref;

          std::shared_ptr<GameUtils::Memory::custom_memory> _subdata_memory;
          _subdata_function_copy _subdata_copy_function = NULL;
      };

      std::map<godot::String, _subdata_metadata> _subdata_map;

      void __copy_subdata_to_subdata(subdata_container* container) const;


    public:
      // don't worry about deinstantiate this class
      // since std::map will deinstantiate all objects contained
      // and the custom data uses std::shared_ptr
      // ~subdata_container();


    // Why all functions in protected? The scope of this function is based on the inheriting class
    protected:
      template<typename subdata_class> void __create_copy_function(GameUtils::Memory::custom_memory* from, GameUtils::Memory::custom_memory* to) const{
        subdata_class* _subdata = from->get_data<subdata_class>();
        _subdata->copy_subdata_to(to);
      }
    
      // if there's an error, it will throw an object of gdstr_exception
      template<typename subdata_class> void __add_new_subdata(godot::Dictionary* _data_dict){
        using namespace GameUtils::Memory;
        godot::String _subdata_name = subdata_class::get_subdata_name();
        _subdata_metadata _metadata;{
          _metadata._is_reference = false;
          _metadata._subdata_memory = custom_memory::create_shared();
          _metadata._subdata_copy_function = &subdata_container::__create_copy_function<subdata_class>;
        }

        subdata_class::parse_dictionary_to(_metadata._subdata_memory.get(), _data_dict);
        _subdata_map[_subdata_name] = _metadata;
      }


      template<typename subdata_class> void __set_subdata(subdata_class* from){
        using namespace GameUtils::Memory;
        godot::String _subdata_name = subdata_class::get_subdata_name();
        _subdata_metadata _metadata;{
          _metadata._is_reference = false;
          _metadata._subdata_memory = custom_memory::create_shared();
          _metadata._subdata_copy_function = &subdata_container::__create_copy_function<subdata_class>;
        }
        
        from->copy_subdata_to(_metadata._subdata_memory.get());
        _subdata_map[_subdata_name] = _metadata;
      }

      template<typename subdata_class> void __set_subdata_ref(std::shared_ptr<subdata_class> ref_ptr){
        using namespace GameUtils::Memory;
        godot::String _subdata_name = subdata_class::get_subdata_name();
        _subdata_metadata _metadata;{
          _metadata._is_reference = true;
          _metadata._subdata_ref = ref_ptr;
          _metadata._subdata_copy_function = NULL;
        }

        _subdata_map[_subdata_name] = _metadata;
      }


      template<typename subdata_class, typename... constructor_args> void __create_subdata(constructor_args... args){
        using namespace GameUtils::Memory;
        godot::String _subdata_name = subdata_class::get_subdata_name();
        _subdata_metadata _metadata;{
          _metadata._is_reference = false;
          _metadata._subdata_memory = custom_memory::create_shared<subdata_class, constructor_args...>(args...);
          _metadata._subdata_copy_function = &subdata_container::__create_copy_function<subdata_class>;
        }

        _subdata_map[_subdata_name] = _metadata;
      }

      
      template<typename subdata_class> subdata_class* __get_subdata() const{
        godot::String _subdata_name = subdata_class::get_subdata_name();
        auto _iter = _subdata_map.find(_subdata_name);
        if(_iter == _subdata_map.end())
          return NULL;

        if(_iter->second._is_reference)
          return (subdata_class*)_iter->second._subdata_ref.lock().get();
        else
          return (subdata_class*)_iter->second._subdata_memory.get();
      }

      // this will return null if the subdata is not reference
      template<typename subdata_class> std::weak_ptr<subdata_class> __get_subdata_weak() const{
        godot::String _subdata_name = subdata_class::get_subdata_name();
        auto _iter = _subdata_map.find(_subdata_name);
        if(_iter == _subdata_map.end())
          return std::weak_ptr<subdata_class>();

        return _iter->second._subdata_ref;
      }

      // this will return null if the subdata is as reference
      template<typename subdata_class> std::shared_ptr<GameUtils::Memory::custom_memory> __get_subdata_shared() const{
        godot::String _subdata_name = subdata_class::get_subdata_name();
        auto _iter = _subdata_map.find(_subdata_name);
        if(_iter == _subdata_map.end())
          return std::shared_ptr<GameUtils::Memory::custom_memory>();

        return _iter->second._subdata_memory;
      }


      template<typename subdata_class> void __remove_subdata(){
        godot::String _subdata_name = subdata_class::get_subdata_name();
        auto _iter = _subdata_map.find(_subdata_name);
        if(_iter == _subdata_map.end())
          return;

        _subdata_map.erase(_iter);
      }
  };
}

#endif