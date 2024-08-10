#ifndef GODOT_MACRO_HEADER
#define GODOT_MACRO_HEADER

#include "godot_cpp/core/class_db.hpp"


#ifndef ___MACRO_STRINGIFY
#define ___MACRO_STRINGIFY1(var) #var
#define ___MACRO_STRINGIFY(var) ___MACRO_STRINGIFY1(var)
#endif

#define JOIN_PATH_STRING_HELPER godot::String("{0}/{1}")


#define GD_PROPERTY_VAR(type, var_name) \
  private: \
    type var_name; \
  public: \
    void set_##var_name(const type var_##var_name){ \
      var_name = var_##var_name; \
    } \
    type get_##var_name() const{ \
      return var_name; \
    }

#define GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(type, var_name) \
  private: \
    type var_name; \
  public: \
    void set_##var_name(const type var_##var_name); \
    type get_##var_name() const;


#define GD_PROPERTY_SETTER_FUNC(class_name, type, var_name) \
  void class_name::set_##var_name(const type var_##var_name)


#define GD_PROPERTY_GETTER_FUNC(class_name, type, var_name) \
  type class_name::get_##var_name() const


#define GD_PROPERTY_BIND(class_name, variant_type, var_name) \
  godot::ClassDB::bind_method(D_METHOD(___MACRO_STRINGIFY(set_) #var_name, #var_name), &class_name::set_##var_name); \
  godot::ClassDB::bind_method(D_METHOD(___MACRO_STRINGIFY(get_) #var_name), &class_name::get_##var_name); \
  godot::ClassDB::add_property( \
    #class_name, \
    PropertyInfo(variant_type, #var_name), \
    ___MACRO_STRINGIFY(set_) #var_name, \
    ___MACRO_STRINGIFY(get_) #var_name \
  );

#define GD_PROPERTY_BIND_HINT(class_name, variant_type, var_name, property_hint) \
  godot::ClassDB::bind_method(D_METHOD(___MACRO_STRINGIFY(set_) #var_name, #var_name), &class_name::set_##var_name); \
  godot::ClassDB::bind_method(D_METHOD(___MACRO_STRINGIFY(get_) #var_name), &class_name::get_##var_name); \
  godot::ClassDB::add_property( \
    #class_name, \
    PropertyInfo(variant_type, #var_name, property_hint), \
    ___MACRO_STRINGIFY(set_) #var_name, \
    ___MACRO_STRINGIFY(get_) #var_name \
  );
  
#endif