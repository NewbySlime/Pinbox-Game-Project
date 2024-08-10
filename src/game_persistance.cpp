#include "game_persistance.h"

using namespace Game;
using namespace Game::Persistance;
using namespace godot;


void game_persistance::_bind_methods(){
  GD_PROPERTY_BIND_HINT(game_persistance, Variant::STRING, save_helper_pckscene, PropertyHint::PROPERTY_HINT_FILE)
  GD_PROPERTY_BIND_HINT(game_persistance, Variant::STRING, load_helper_pckscene, PropertyHint::PROPERTY_HINT_FILE)
}