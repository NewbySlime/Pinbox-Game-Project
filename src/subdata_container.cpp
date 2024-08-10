#include "subdata_container.h"


using namespace GameUtils::Memory;
using namespace GameUtils::ParsingHelper;
using namespace godot;


void subdata_container::__copy_subdata_to_subdata(subdata_container* container) const{
  for(auto _pair: _subdata_map){
    String _subdata_name = _pair.first;

    // NOTE: better not to replace directly the custom_memory, as that might still be used
    _subdata_metadata _metadata;{
      _metadata._is_reference = _pair.second._is_reference;
      _metadata._subdata_copy_function = NULL;
    }

    if(_pair.second._is_reference){
      _metadata._subdata_ref = _pair.second._subdata_ref;
    }
    else{
      _metadata._subdata_memory = custom_memory::create_shared();
      _metadata._subdata_copy_function = _pair.second._subdata_copy_function;

      (this->*_pair.second._subdata_copy_function)(_pair.second._subdata_memory.get(), _metadata._subdata_memory.get());
    }

    container->_subdata_map[_subdata_name] = _metadata;
  }
}