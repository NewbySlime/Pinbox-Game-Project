#ifndef GODOT_ERROR_HEADER
#define GODOT_ERROR_HEADER

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/variant/string.hpp"

namespace Game::Error{
  class gdstr_exception{
    private:
      godot::String _what_str;
      godot::Error _error_equivalent;

    public:
      gdstr_exception(godot::String what, godot::Error error_equivalent = godot::Error::FAILED){
        _what_str = what;
        _error_equivalent = error_equivalent;
      }

      godot::String& what(){
        return _what_str;
      }

      godot::Error get_gderror_equivalent(){
        return _error_equivalent;
      }
  };
}

#endif