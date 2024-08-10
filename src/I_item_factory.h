#ifndef I_ITEM_FACTORY_HEADER
#define I_ITEM_FACTORY_HEADER

#include "id_types.h"
#include "interface_helper.h"
#include "item_object.h"


namespace Game::Item{
  class item_server;

  namespace Factory{
    class I_item_factory{
    INTERFACE_DEFINE_INTERFACE_CLASS(I_item_factory)

      protected:
        Game::Item::item_object* _get_item_object(Game::Item::item_server* server, Game::Item::item_inst_id id);

      public:
        virtual void prep_item(Game::Item::item_inst_id id){}
        
        virtual void randomize_item_stats(Game::Item::item_inst_id id){}
    };
  }
}

#endif