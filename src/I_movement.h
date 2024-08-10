#ifndef I_MOVEMENT_HEADER
#define I_MOVEMENT_HEADER

#include "interface_helper.h"
#include "I_action_class.h"


namespace Game{
  class I_movement{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_movement)

    public:
      virtual void move_up();
      virtual void move_down();
      virtual void move_left();
      virtual void move_right();

      virtual void move_stop_up();
      virtual void move_stop_down();
      virtual void move_stop_left();
      virtual void move_stop_right();

      virtual void on_terniary_priming(){}
      virtual void on_terniary_continuing(){}
      virtual void on_terniary_cancelled(){}

      virtual void on_update(const action_class_update_data& update_data){}
  };
}


#endif