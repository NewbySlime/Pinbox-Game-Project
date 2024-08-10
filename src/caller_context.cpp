#include "caller_context.h"


using namespace GameUtils::Function;
using namespace godot;


caller_data* caller_context::_create_data(const caller_data& from_data){
  caller_data* _new_data = new caller_data();
  *_new_data = from_data;

  return _new_data;
}

void caller_context::_delete_data(GameUtils::Function::caller_data* _caller_data){
  delete _caller_data;
}


void caller_context::_on_this_deleted_data_iteration(caller_data* _caller_data){
  _delete_data(_caller_data);
}


void caller_context::add_current_context(const caller_data& caller_context){
  delete_current_context();

  caller_data* _new_data = _create_data(caller_context);
  add_last_data(_new_data);
}

void caller_context::delete_current_context(){
  caller_data* _last_data = get_last_data();
  if(_last_data)
    _delete_data(_last_data);

  add_last_data(NULL);
}


const caller_data* caller_context::get_current_context(){
  return get_last_data();
}