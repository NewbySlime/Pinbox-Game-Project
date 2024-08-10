#ifndef PREDELETE_NOTIFIER_HEADER
#define PREDELETE_NOTIFIER_HEADER

#include "godot_var_extended.h"

#include "godot_cpp/classes/node.hpp"

#include "map"

#define SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE "spdnopd"


#define ATTR_PREDELETE_NOTIFIER_INHERIT(origin_class)

#define ATTR_PREDELETE_NOTIFIER_DEFINE(origin_class)

#define ATTR_PREDELETE_NOTIFIER_BIND_GODOT(origin_class) \
  ADD_SIGNAL(godot::MethodInfo(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, godot::PropertyInfo(GDVAR_CUSTOM_VARIANT_TYPE, "param")));

#define ATTR_PREDELETE_NOTIFIER_ON_NOTIFICATION(code) \
  switch(code){ \
    break; case godot::Object::NOTIFICATION_PREDELETE:{ \
      predelete_notifier_signal_param _param(this); \
      emit_signal(SIGNAL_PREDELETE_NOTIFIER_ON_PREDELETE, GDVAR_CUSTOM_FROM(_param)); \
    } \
  }


class predelete_notifier_signal_param{
  private:
    godot::Node* _this_node;

  public:
    predelete_notifier_signal_param(godot::Node* node){
      _this_node = node;
    }

    godot::Node* get_node() const{
      return _this_node;
    }
};


#endif