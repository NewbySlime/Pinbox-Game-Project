#ifndef I_ITEM_HANDLER_HEADER
#define I_ITEM_HANDLER_HEADER

#include "interface_helper.h"
#include "item_object.h"


namespace Game::Item{
  class I_item_handler{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_item_handler)
    public:
      // NOTE: can be passed as NULL object to notify that the item_object doesn't exist anymore
      // since the child doesn't know if the item_object is deleted by the item_server, it is up to the parent to notify the childreb (interface) whenever the item_server giving the signal when the item_object is deleted. 
      virtual void set_item_object(const Game::Item::item_object* _item_object = NULL){}
      
  };
};

#endif