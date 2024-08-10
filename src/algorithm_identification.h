#ifndef ALGORITHM_IDENTIFICATION_HEADER
#define ALGORITHM_IDENTIFICATION_HEADER

#include "runtime_id_container.h"

#include "stdint.h"

#include "map"


namespace Game::Misc::Algorithm{
  /// @brief To get new ID based on the map of the ID list.
  /// @tparam _value map value (not actually used)
  /// @param _map The map of IDs
  /// @param default_null_id The default value when there's no ID can be used
  /// @return The new ID
  template<typename _value> uint64_t get_new_id_from_map(std::map<uint64_t, _value>& _map, uint64_t default_null_id = NULL){
    if(_map.size() <= 0)
      return default_null_id+1;

    auto _iter = _map.begin();
    while(true){
      if(_iter == _map.end())
        break;

      auto _next_iter = _iter++;
      uint64_t _start_idx, _finish_idx;
      if(_iter == _map.begin() && 0 < _iter->first){
        _start_idx = 0;
        _finish_idx = _iter->first;
      }
      else if(_next_iter == _map.end()){
        _start_idx = _iter->first;
        _finish_idx = UINT64_MAX;

        _iter = _next_iter;
      }
      else{
        _start_idx = _iter->first;
        _finish_idx = _next_iter->first;

        _iter = _next_iter;
      }

      while(true){
        if(_start_idx != default_null_id)
          return _start_idx;
        
        if(_start_idx == _finish_idx)
          break;

        _start_idx++;
      }
    }

    return default_null_id;
  }
}

#endif