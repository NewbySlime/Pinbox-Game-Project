#include "I_item_factory.h"
#include "item_server.h"

using namespace Game::Item;
using namespace Game::Item::Factory;


item_object* I_item_factory::_get_item_object(item_server* server, item_inst_id id){
  return server->_get_item_object_mutable(id);
}