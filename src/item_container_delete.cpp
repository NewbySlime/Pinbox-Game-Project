#include "item_container_delete.h"

#include "item_server.h"


using namespace Game;
using namespace Game::Item;
using namespace godot;


void item_container_delete::_bind_methods(){

}


void item_container_delete::delete_items(){
  for(int i = 0; i < _item_list.size(); i++){
    uint64_t _id = _item_list[i];
    if(_id == ITEM_ID_NULL)
      continue;

    _server->delete_item(_id);
  }
}