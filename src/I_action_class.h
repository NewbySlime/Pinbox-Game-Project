#ifndef I_ACTION_CLASS_HEADER
#define I_ACTION_CLASS_HEADER

#include "interface_helper.h"

#include "godot_cpp/variant/vector2.hpp"


namespace Game{
  struct action_class_update_data{
    godot::Vector2 global_pos;
    godot::Vector2 target_global_pos;
  };

  class I_action_class{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_action_class)

    public:
      virtual ~I_action_class(){}

      virtual void on_action_priming(){}
      virtual void on_action_continuing(){}
      virtual void on_action_cancelled(){}

      virtual bool is_alternate_viable(){return true;}
      virtual void on_alternate_action_priming(){}
      virtual void on_alternate_action_continuing(){}
      virtual void on_alternate_action_cancelled(){}

      virtual void on_update(const action_class_update_data &update_data){}
  };
}

#endif