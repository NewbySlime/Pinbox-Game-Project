#ifndef ITEM_LOCATION_DATA_STRUCT
#define ITEM_LOCATION_DATA_STRUCT

#include "stdint.h"


namespace Game::Item{
  #define ITEM_POSITION_DATA_CLEAR(pos_data) pos_data.container_id = ITEM_CONTAINER_ID_NULL; pos_data.index_position = -1
  #define ITEM_POSITION_DATA_CHECK(pos_data) (pos_data.container_id != ITEM_CONTAINER_ID_NULL && pos_data.index_position >= 0)
  struct item_position_data{
    uint64_t container_id;

    int index_position;
  };
}

#endif