#ifndef ID_TYPES_HEADER
#define ID_TYPES_HEADER

#include "stdint.h"

#define ENTITY_ID_NULL 0

#define ITEM_ID_NULL 0
#define ITEM_CONTAINER_ID_NULL 0

#define ITEM_DROPPER_ID_NULL 0
#define ITEM_PICKER_ID_NULL 0
#define ITEM_IWE2D_ID_NULL 0



namespace Game::Entity{
  typedef uint64_t entity_inst_id;

  enum entity_type{
    et_player =     0x1,
    et_npc =        0x2,
    et_enemy =      0x3
  };
}

namespace Game::Item{
  typedef uint64_t item_inst_id;
  typedef uint64_t item_container_inst_id;

  typedef uint64_t item_dropper_inst_id;
  typedef uint64_t item_picker_inst_id;
  typedef uint64_t iwe2d_inst_id;

  enum item_type{
    it_modifier =       0x1,
    it_weapon =         0x2,
    it_material =       0x3,
    it_consumables =    0x4,
    it_throwables =     0x5
  };

  enum item_container_type{
    ict_generic,
    ict_infinite,
    ict_delete
  };
}

#endif