#ifndef OVERRIDER_BASE_HEADER
#define OVERRIDER_BASE_HEADER

#include "interface_helper.h"

// don't forget godot::String _get_overrider_static_id()
#define ATTR_OVERRIDER_BASE_INHERIT(origin_class) \
  protected: \
    godot::String ___overrider_base_get_custom_id_from_static() override; \
 \
  public: \
    static godot::String get_overrider_id_static(); \
 \
  private:


#define ATTR_OVERRIDER_BASE_DEFINE(origin_class) \
  godot::String origin_class::get_overrider_id_static(){ \
    return origin_class::_get_overrider_static_id(); \
  } \
 \
  godot::String origin_class::___overrider_base_get_custom_id_from_static(){ \
    return origin_class::_get_overrider_static_id(); \
  }
  

#define ATTR_OVERRIDER_BASE_BIND_GODOT(origin_class)


#define ATTR_OVERRIDER_BASE_ON_NOTIFICATION(code)




namespace Game{
  class overrider_base{
  INTERFACE_DEFINE_INTERFACE_CLASS(overrider_base)

    private:
      godot::String _static_id;
      godot::String _custom_id;

    protected:
      virtual godot::String ___overrider_base_get_custom_id_from_static(){ return "NIL"; }
      void _set_overrider_custom_id(godot::String custom_id);

    public:
      overrider_base();

      godot::String get_overrider_id();

  };
}

#endif