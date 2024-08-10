#ifndef ALGORITHM_CONTAINER_HEADER
#define ALGORITHM_CONTAINER_HEADER

#include "map"
#include "set"


namespace Game::Misc::Algorithm{
  /// @brief Helper function to create the reversed types of the key and value of the provided map container.
  /// @tparam key Key map type
  /// @tparam value Value map type
  /// @param original_map The provided map that will be reversed
  /// @return Reversed map's key and value
  template<typename key, typename value> std::map<value, key> create_inverse_of_map(std::map<key, value> original_map){
    std::map<value, key> _result;
    for(auto _pair: original_map)
      _result[_pair.second] = _pair.first;

    return _result;
  }


  /// @brief Helper function to create a container filled with only the key
  /// @tparam key Key map type
  /// @tparam value Value map type
  /// @param from The provided map that will be stripped
  /// @return Set of keys from the provided map container
  template<typename key, typename value> std::set<key> create_set_of_key(const std::map<key, value>& from){
    std::set<key> _result;
    for(auto _pair: from)
      _result.insert(_pair.first);

    return _result;
  }

  /// @brief Helper function to create a container filled with only the value
  /// @tparam key Key map type
  /// @tparam value Value map type
  /// @param from The provided map that will be stripped
  /// @return Set of values from the provided map container
  template<typename key, typename value> std::set<value> create_set_of_value(const std::map<key, value>& from){
    std::set<value> _result;
    for(auto _pair: from)
      _result.insert(_pair.second);

    return _result;
  }
}

#endif