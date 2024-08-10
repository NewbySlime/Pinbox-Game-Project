#include "algorithm_bezier.h"
#include "algorithm_node.h"
#include "defines.h"
#include "item_picker_area.h"
#include "item_world_entity_2d_handler.h"

#include "godot_cpp/classes/circle_shape2d.hpp"
#include "godot_cpp/classes/collision_shape2d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/shape2d.hpp"


using namespace Game;
using namespace Game::Misc::Algorithm;
using namespace Game::Item;
using namespace godot;


void item_picker_area::_item_container_on_added(uint64_t _param){
  INIT_ASSERT()
  LOG_ASSERT("item added")
  _check_container();
}

void item_picker_area::_item_container_on_removed(uint64_t _param){
  _check_container();
}


void item_picker_area::_area_on_body_entered(Node* node){
  INIT_ASSERT()
  LOG_ASSERT("body entered")

  if(!node->is_class(item_world_entity_2d::get_class_static()))
    return;

  item_world_entity_2d* _entity = (item_world_entity_2d*)node;
  _entity->add_picker_gravity(_item_picker_id);
}

void item_picker_area::_area_on_body_exited(Node* node){
  INIT_ASSERT()
  LOG_ASSERT("body exited")

  if(!node->is_class(item_world_entity_2d::get_class_static()))
    return;

  item_world_entity_2d* _entity = (item_world_entity_2d*)node;
  _entity->remove_picker_gravity(_item_picker_id);
}


void item_picker_area::_collector_on_body_entered(Node* node){
  if(_is_container_full)
    return;

  if(!node->is_class(item_world_entity_2d::get_class_static()))
    return;

  INIT_ASSERT()
  LOG_ASSERT("collecting item")
  item_world_entity_2d* _iwe2d_object = (item_world_entity_2d*)node;
  if(_iwe2d_object->is_pickable())
    _iwe2d_handler->pick_item(_iwe2d_object->get_dropped_item_id(), get_id());
}


void item_picker_area::_unbind_container(I_item_container* container, Node* container_node){
  container_node->disconnect(SIGNAL_ITEM_CONTAINER_ON_INDEX_ADD, Callable(this, "_item_container_on_added"));
  container_node->disconnect(SIGNAL_ITEM_CONTAINER_ON_INDEX_REMOVED, Callable(this, "_item_container_on_removed"));
}

void item_picker_area::_bind_container(I_item_container* container, Node* container_node){
  container_node->connect(SIGNAL_ITEM_CONTAINER_ON_INDEX_ADD, Callable(this, "_item_container_on_added"));
  container_node->connect(SIGNAL_ITEM_CONTAINER_ON_INDEX_REMOVED, Callable(this, "_item_container_on_removed"));
}


void item_picker_area::_check_container(){
  if(_bound_container_id == ITEM_CONTAINER_ID_NULL)
    return;

  INIT_ASSERT()
  Array _paramarr;{
    _paramarr.append(_bound_container->empty_slot_count());
  }

  LOG_ASSERT(String("empty slot count {0}").format(_paramarr))

  _is_container_full = _bound_container->empty_slot_count() <= 0;
}


void item_picker_area::_bind_methods(){
  GD_PROPERTY_BIND(item_picker_area, Variant::FLOAT, max_dragged_speed)
  GD_PROPERTY_BIND(item_picker_area, Variant::PACKED_VECTOR2_ARRAY, drag_speed_bezier_graph)

  GD_PROPERTY_BIND(item_picker_area, Variant::NODE_PATH, picker_area_area)
  GD_PROPERTY_BIND(item_picker_area, Variant::NODE_PATH, picker_collector_area)

  ClassDB::bind_method(
    D_METHOD("_item_container_on_added",
      "param_data"
    ),

    &item_picker_area::_item_container_on_added
  );

  ClassDB::bind_method(
    D_METHOD("_item_container_on_removed",
      "param_data"
    ),

    &item_picker_area::_item_container_on_removed
  );

  ClassDB::bind_method(
    D_METHOD("_area_on_body_entered",
      "node"
    ),

    &item_picker_area::_area_on_body_entered
  );

  ClassDB::bind_method(
    D_METHOD("_area_on_body_exited",
      "node"
    ),

    &item_picker_area::_area_on_body_exited
  );

  ClassDB::bind_method(
    D_METHOD("_collector_on_body_entered",
      "node"
    ),

    &item_picker_area::_collector_on_body_entered
  );
}


void item_picker_area::bind_item_container(uint64_t container){
  I_item_container* _current_container = _item_server->get_item_container(container);
  Node* _current_container_node = _item_server->get_item_container_node(container);
  if(!_current_container)
    return;

  unbind_item_container();

  _bind_container(_current_container, _current_container_node);
  _bound_container = _current_container;
  _bound_container_id = container;

  _check_container();
}

void item_picker_area::unbind_item_container(){
  if(_bound_container_id == ITEM_CONTAINER_ID_NULL)
    return;
  
  Node* _bound_container_node = _item_server->get_item_container_node(_bound_container_id);
  if(!_bound_container_node)
    return;

  _bound_container = NULL;
  _bound_container_id = ITEM_CONTAINER_ID_NULL;
  _unbind_container(_bound_container, _bound_container_node);
}


item_picker_area::item_picker_area(){
  _iwe2d_handler = NULL;
  _item_server = NULL;

  _bound_container = NULL;
  _bound_container_id = ITEM_CONTAINER_ID_NULL;

  _max_dragged_radius = 0;
  _is_container_full = false;
}


void item_picker_area::_notification(int code){
  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      if(_iwe2d_handler)
        _iwe2d_handler->delete_item_picker_area(_item_picker_id);
    }
  }
}


void item_picker_area::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    //set_collision_layer(0);
    //set_collision_mask(COLLISION_LAYER_ITEM_LAYER);

    CREATE_GET_NODE_TESTER(_picker_area, picker_area_area, Area2D);
    CREATE_GET_NODE_TESTER(_picker_collector_area, picker_collector_area, Area2D)

    _picker_area->connect("body_entered", Callable(this, "_area_on_body_entered"));
    _picker_area->connect("body_exited", Callable(this, "_area_on_body_exited"));

    _picker_collector_area->connect("body_entered", Callable(this, "_collector_on_body_entered"));

    CollisionShape2D* _picker_area_colshp = find_child_node_type<CollisionShape2D>(_picker_area);
    if(!_picker_area_colshp){
      _paramarr.clear();{
        _paramarr.append(_picker_area->get_name());
      }

      ERR_ASSERT(String("Cannot find CollisionShape2D in {0}.").format(_paramarr))
    }

    CircleShape2D* _cirshape = get_collision_shape<CircleShape2D>(_picker_area_colshp);
    if(!_cirshape){
      ERR_ASSERT("Collision Shape is not a circle. Other than circle is not yet implemented.")
    }

    _max_dragged_radius = _cirshape->get_radius();
  }
}


double item_picker_area::get_picker_gravity_speed(Vector2 from_position, const item_object* item){
  if(_bound_container_id == ITEM_CONTAINER_ID_NULL || _is_container_full || !_bound_container->is_item_can_exist(item))
    return 0;

  double _dist = (get_global_position()-from_position).length();
  Vector2 _val = bezier_curve_function(&drag_speed_bezier_graph, _dist/_max_dragged_radius, true, true);

  return _val.x * max_dragged_speed;
}


item_picker_inst_id item_picker_area::get_id(){
  return _item_picker_id;
}