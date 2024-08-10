#ifndef ITEM_CONTAINER_DELETE_HEADER
#define ITEM_CONTAINER_DELETE_HEADER

#include "item_container.h"


namespace Game::Item{
  class item_container_delete: public Game::Item::item_container{
  GDCLASS(item_container_delete, Game::Item::item_container)

    protected:
      static void _bind_methods();

    public:
      void delete_items();

      uint32_t get_container_type() override{ return ict_delete; }
  };
}

#endif