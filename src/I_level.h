#ifndef I_LEVEL_HEADER
#define I_LEVEL_HEADER

#include "interface_helper.h"


namespace Game{
  class I_level{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_level)

    public:
      virtual void initiate_level(){}
      virtual void clear_level(){}

      virtual void game_start(){}
  };
}

#endif