#ifndef RUNTIME_ID_CONTAINER_HEADER
#define RUNTIME_ID_CONTAINER_HEADER

#include "thread_error.h"

#include "map"
#include "set"


namespace GameUtils::Identification{
  // for id_object, it has to accomodate function operator++ and equal comparison operator
  // id_object also has default constructor for the invalid id
  // therefore, id_object is best as a custom class, not c++ basic types
  // for item_object, it has to accomodate comparison operators used by std::map
  template<typename id_object, typename item_object> class runtime_id_container{
    public:
      enum class_error{
        rtic_ce_ok,
        id_not_exist,
        id_already_exist,
        id_is_excluded,
        index_out_of_range,
        cannot_create_new_id
      };

    private:
      std::map<id_object, item_object> _item_data;
      std::set<id_object> _excluded_id;

      Game::Error::thread_error<class_error> _error_data = Game::Error::thread_error<class_error>(rtic_ce_ok);


      void _set_error(class_error error){
        _error_data.add_last_error(error);
      }


    public:
      class_error get_last_error(){
        return _error_data.get_last_error();
      }


      void add_to_excluded_id(id_object id){
        _excluded_id.insert(id);
      }

      void remove_from_excluded_id(id_object id){
        _excluded_id.erase(id);
      }


      // if cannot get new id, returns the default id_object
      // for user, check get_last_error function
      id_object find_new_id(){
        id_object _res_id = id_object();

        id_object _begin_id = _item_data.begin()->second;
        bool _id_found = false;
        for(auto _current_iter = _item_data.begin(); !_id_found && _current_iter != _item_data.end(); _current_iter++){
          auto _next_iter = _current_iter+1;
          // if it's the end of the map
          if(_next_iter == _item_data.end()){
            id_object _current_id = _current_iter->second;
            while(true){
              _current_id++;

              // if _current_id is the same with the id contained in _excluded_id
              auto _iter_ex_id = _excluded_id.find(_current_id);
              if(_iter_ex_id != _excluded_id.end())
                continue;

              // if _current_id is the same as the beginning id (looping)
              if(_current_id == _begin_id){
                _set_error(cannot_create_new_id);
                _id_found = true; // just to break from first loop
                break;
              }

              // don't check from _item_data since this is the last iter

              _res_id = _current_id;
              _id_found = true;
              break;
            }
          }
          // when there's more iterators to check
          else{
            id_object _last_id_check = _next_iter->second;
            id_object _current_id = _current_iter->second;
            while(true){
              _current_id++;
              if(_current_id == _last_id_check)
                break;

              auto _iter_ex_id = _excluded_id.find(_current_id);
              if(_iter_ex_id != _excluded_id.end())
                continue;

              _res_id = _current_id;
              _id_found = true;
              break;
            }
          }
        }

        return _res_id;
      }


      item_object get_item(id_object id, item_object default_item = item_object()){
        auto _iter = _item_data.find(id);
        if(_iter == _item_data.end()){
          _set_error(id_not_exist);
          return default_item;
        }

        return _iter->second;
      }

      void add_item(item_object data, bool as_reserved = false, id_object reserved_id = id_object()){
        id_object _new_id;
        if(as_reserved){
          auto _iter_ex_id = _excluded_id.find(reserved_id);
          if(_iter_ex_id != _excluded_id.end()){
            _set_error(id_is_excluded);
            return;
          }

          auto _iter_id = _item_data.find(reserved_id);
          if(_iter_id != _item_data.end()){
            _set_error(id_already_exist);
            return;
          }

          _new_id = reserved_id;
        }
        else
          _new_id = find_new_id();

        _item_data[_new_id] = data;
      }

      item_object delete_item(id_object id, item_object default_item = item_object()){
        auto _iter = _item_data.find(id);
        if(_iter == _item_data.end()){
          _set_error(id_not_exist);
          return default_item;
        }

        _item_data.erase(_iter);
        return _iter->second;
      }


      uint64_t get_size(){
        return _item_data.size();
      }

      item_object get_item_index(uint64_t index, item_object default_item = item_object()){
        if(index >= _item_data.size() || index < 0){
          _set_error(index_out_of_range);
          return default_item;
        }

        return (_item_data.begin()+index)->second;
      }
  };
}

#endif