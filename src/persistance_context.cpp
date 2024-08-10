#include "logger.h"
#include "persistance_context.h"
#include "static_variable_storage.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/array.hpp"


using namespace Game;
using namespace Game::Persistance;
using namespace GameUtils::Memory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(persistance_context)
ATTR_BASE_INTERFACE_HELPER_DEFINE(persistance_context)
ATTR_INTERFACE_HELPER_DEFINE(persistance_context)


class _dmp_null_persistance_data: public I_persistance_data{
  public:
    bool is_available() override{
      return false;
    }
};


struct static_data: public custom_data{
  public:
    static String get_id(){ return "persistance_context.static_data"; }

    _dmp_null_persistance_data _dmp_pdata;
};


// persistance_context define code
void persistance_context::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(persistance_context)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(persistance_context)
  ATTR_INTERFACE_HELPER_BIND_GODOT(persistance_context)
}


void persistance_context::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data)
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data.")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){

  }
}


persistance_context::persistance_context(){
  _current_id = FALSE_PERSISTANCE_CTX_ID;
}


void persistance_context::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
  }
}


I_persistance_data* persistance_context::get_current_persistance_data(){
  static_data* _static = _static_data->get_data<static_data>();

  if(_current_id == FALSE_PERSISTANCE_CTX_ID)
    return &_static->_dmp_pdata;

  // TODO
  return NULL;
}


bool persistance_context::change_persistance(persistance_id id){
  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD_INITIATED);

  // TODO load the game save

  persistance_context_on_load_param _param;{

  }

  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD, GDVAR_CUSTOM_FROM(_param));

  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_LOAD_DONE);

  return true;
}


void persistance_context::trigger_save(){
  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE_INITIATED);

  persistance_context_on_save_param _param;{

  }

  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE, GDVAR_CUSTOM_FROM(_param));

  // TODO then do save to files

  emit_signal(SIGNAL_PERSISTANCE_CONTEXT_ON_SAVE_DONE);
}





// persisntace_metadata define code
String persistance_context::persistance_metadata::get_save_name(){
  return _save_name;
}



String persistance_context::persistance_metadata::get_formatted_last_save_time(){
  // ex: Fri, Jan 6 2013 - 20:13
  const char* _time_format = "aaa, bbb dd yyyy - hh:mm";

  String _res = "-";
  uint64_t _time_size = strlen(_time_format);

  // with null terminated char
  char* _time_str = new char[_time_size+1];
  tm* _localtime = localtime(&_unix_time_last_save);
  if(strftime(_time_str, _time_size, _time_format, _localtime))
    _res = _time_str;

  delete _time_str;
  return _res;
}