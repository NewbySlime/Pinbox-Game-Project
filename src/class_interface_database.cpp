#include "class_interface_database.h"
#include "register_types.h"


using namespace Game;
using namespace godot;


void class_interface_database::_bind_methods(){}


class_interface_database::class_interface_database(){
  _dummy_interface_map = std::make_shared<std::set<String>>();
}


void class_interface_database::_ready(){
  register_types_interface(this);
}


bool class_interface_database::has_interface_type(const String& class_name, const String& interface_name){
  auto _class_iter = _class_interface_map.find(class_name);
  if(_class_iter == _class_interface_map.end())
    return false;

  auto _interface_iter = _class_iter->second->find(interface_name);
  if(_interface_iter == _class_iter->second->end())
    return false;

  return true;
}


std::shared_ptr<const std::set<String>> class_interface_database::get_interface_list(const String& origin_class_name){
  auto _class_iter = _class_interface_map.find(origin_class_name);
  if(_class_iter == _class_interface_map.end())
    return _dummy_interface_map;

  return _class_iter->second;
}