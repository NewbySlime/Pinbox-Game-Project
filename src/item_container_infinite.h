#ifndef ITEM_CONTAINER_INFINITE_HEADER
#define ITEM_CONTAINER_INFINITE_HEADER

#include "item_container.h"


namespace Game::Item{
  class item_container_infinite: public Game::Item::item_container{
  GDCLASS(item_container_infinite, Game::Item::item_container)
  
    protected:
      static void _bind_methods();

      int _get_new_index();

      uint64_t _remove_item(int index);
      int _add_item(uint64_t id, int index = -1, bool force = false, uint64_t* _removed_id = NULL);

    public:
      int get_container_size();
      void resize_container(int new_size);

      uint32_t get_container_type() override{ return ict_infinite; }
  };
}

#endif