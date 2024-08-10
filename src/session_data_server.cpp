#include "session_data_server.h"


using namespace Game;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(session_data_server)
ATTR_BASE_INTERFACE_HELPER_DEFINE(session_data_server)
ATTR_INTERFACE_HELPER_DEFINE(session_data_server)


void session_data_server::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(session_data_server)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(session_data_server)
  ATTR_INTERFACE_HELPER_BIND_GODOT(session_data_server)
}


void session_data_server::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


void session_data_server::remove_all_subdata(){
  _subdata_map.clear();
}