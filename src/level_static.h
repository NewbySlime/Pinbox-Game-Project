#ifndef LEVEL_STATIC_HEADER
#define LEVEL_STATIC_HEADER

#include "base_level.h"


namespace Game::Level{
  class level_static: public Game::Level::base_level{
  GDCLASS(level_static, Game::Level::base_level)

    protected:
      static void _bind_methods();

    
  }
}

#endif