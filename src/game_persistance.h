#ifndef GAME_PERSISTANCE_HEADER
#define GAME_PERSISTANCE_HEADER

#include "godot_macro.h"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/variant/string.hpp"

#include "map"


namespace Game::Persistance{
  class game_persistance: public godot::Node{
  GDCLASS(game_persistance, godot::Node)

    GD_PROPERTY_VAR(godot::String, save_helper_pckscene)
    GD_PROPERTY_VAR(godot::String, load_helper_pckscene)

    private:
      std::map<uint64_t, godot::Node> _helper_instance;      

    protected:
      static void _bind_methods();

    public:
      
  };
}

#endif