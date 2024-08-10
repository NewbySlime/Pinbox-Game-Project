#include "algorithm_item_parse.h"
#include "defines.h"
#include "godot_error.h"

#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"


using namespace Game::Error;
using namespace Game::Misc::Algorithm;
using namespace godot;



/* parse_item_id_result functions defining */
void parse_item_id_result::add_warning(const String& warning_msg){
  _warning_message += warning_msg;
  if(_warning_exist)
    _warning_message += "\n";

  _warning_exist = true;
}

bool parse_item_id_result::is_warning_exist(){
  return _warning_exist;
}

String parse_item_id_result::get_warning(){
  return _warning_message;
}


void parse_item_id_result::set_item_id(const String& item_id){
  _item_id = item_id;
}

void parse_item_id_result::set_item_type(const String& item_type){
  _item_type = item_type;
}


String parse_item_id_result::get_item_id(){
  return _item_id;
}

String parse_item_id_result::get_item_type(){
  return _item_type;
}





parse_item_id_result Game::Misc::Algorithm::parse_full_item_id(String full_id){
  Array _paramarr;
  PackedStringArray _split_key = full_id.split(CHARSYMBOL_KEY_SPLIT);

  parse_item_id_result _result;

  // for logging purposes
  _paramarr.clear();{
    _paramarr.append(full_id);
  }

  // there's no fucking way this would happen, but for safety measures      
  if(_split_key.size() <= 0)
    throw gdstr_exception(String("No Item ID contained.").format(_paramarr));
  else if(_split_key.size() > 2)
    // give warning
    _result.add_warning(String("Multiple Subcategory (key: '{0}') is invalid, but will use original format to continue.").format(_paramarr));

  if(_split_key.size() == 1)
    _result.add_warning(String("No type included for '{0}'.").format(_paramarr));

  _result.set_item_type(_split_key[0]);
  _result.set_item_id(_split_key[1]);

  return _result;
}