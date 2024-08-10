#include "defines.h"
#include "item_description_popout.h"
#include "logger.h"

#include "godot_cpp/classes/engine.hpp"

#include "functional"
#include "map"
#include "string"


using namespace Game;
using namespace Game::Item;
using namespace godot;


#define ANIMATION_POPOUT "popout"
#define ANIMATION_HIDE "hide"
#define ANIMATION_SCROLL_UP "scroll_up"
#define ANIMATION_SCROLL_DOWN "scroll_down"
#define ANIMATION_WAITING_NEXT "waiting_next"



GD_PROPERTY_GETTER_FUNC(item_description_popout, double, set_vscroll_norm){
  if(!_scroll_container || !_label_container)
    return 0;

  double _label_size = _label_container->get_size().y;
  double _rendered_size = _scroll_container->get_size().y;
  if(_label_size < _rendered_size)
    return 0;

  return _scroll_container->get_v_scroll() / (_label_size - _rendered_size);
}

GD_PROPERTY_SETTER_FUNC(item_description_popout, double, set_vscroll_norm){
  if(!_scroll_container || !_label_container)
    return;

  double _label_size = _label_container->get_size().y;
  double _rendered_size = _scroll_container->get_size().y;
  if(_label_size < _rendered_size)
    return;

  _scroll_container->set_v_scroll(var_set_vscroll_norm * (_label_size - _rendered_size));
}


void item_description_popout::_on_animation_done(String name){
  const std::map<std::string, std::function<void(item_description_popout*)>> _anim_handler = {
    {ANIMATION_POPOUT, [](item_description_popout* _this){
      _this->_currently_transitioning = false;
      _this->_animation_player->play(ANIMATION_SCROLL_DOWN);
    }},

    {ANIMATION_HIDE, [](item_description_popout* _this){
      _this->_currently_transitioning = false;
      _this->set_visible(false);
    }},

    {ANIMATION_SCROLL_DOWN, [](item_description_popout* _this){
      _this->_animation_player->play(ANIMATION_WAITING_NEXT);
      _this->_next_animation = ANIMATION_SCROLL_UP;
    }},

    {ANIMATION_SCROLL_UP, [](item_description_popout* _this){
      _this->_animation_player->play(ANIMATION_WAITING_NEXT);
      _this->_next_animation = ANIMATION_SCROLL_DOWN;
    }},

    {ANIMATION_WAITING_NEXT, [](item_description_popout* _this){
      _this->_animation_player->play(_this->_next_animation);
    }}
  };


  auto _iter = _anim_handler.find(GDSTR_TO_STDSTR(name));
  if(_iter == _anim_handler.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(name);
    }

    WARN_ASSERT(String("No animation callback handler for '{0}'").format(_paramarr))
    return;
  }

  _iter->second(this);
}

void item_description_popout::_on_animation_started(String name){
  const std::map<std::string, std::function<void(item_description_popout*)>> _anim_handler = {
    {ANIMATION_POPOUT, [](item_description_popout* _this){
      _this->_currently_transitioning = true;
      _this->_animation_player->set_speed_scale(1);
    }},

    {ANIMATION_HIDE, [](item_description_popout* _this){
      _this->_currently_transitioning = true;
      _this->_animation_player->set_speed_scale(1);
    }},
    
    {ANIMATION_SCROLL_DOWN, [](item_description_popout* _this){
      _this->_currently_transitioning = false;
      _this->_animation_player->set_speed_scale(_this->_get_scroll_speed_anim_scale());
    }},

    {ANIMATION_SCROLL_UP, [](item_description_popout* _this){
      _this->_currently_transitioning = false;
      _this->_animation_player->set_speed_scale(_this->_get_scroll_speed_anim_scale());
    }},

    {ANIMATION_WAITING_NEXT, [](item_description_popout* _this){
      _this->_currently_transitioning = false;
      _this->_animation_player->set_speed_scale(1);
    }}
  };


  auto _iter = _anim_handler.find(GDSTR_TO_STDSTR(name));
  if(_iter == _anim_handler.end()){
    INIT_ASSERT()
    Array _paramarr;{
      _paramarr.append(name);
    }

    WARN_ASSERT(String("No animation callback handler for '{0}'").format(_paramarr))
    return;
  }

  _iter->second(this);
}


double item_description_popout::_get_scroll_speed_anim_scale(){
  return scroll_speed / _rt_label->get_size().y;
}


double item_description_popout::_get_current_anim_length_normalized(){
  return _animation_player->get_current_animation_position() / _animation_player->get_current_animation_length();
}

void item_description_popout::_set_current_anim_length_normalized(double _val){
  _animation_player->seek(_animation_player->get_current_animation_length() * _val);
}


void item_description_popout::_bind_methods(){
  GD_PROPERTY_BIND(item_description_popout, Variant::NODE_PATH, animation_player)
  GD_PROPERTY_BIND(item_description_popout, Variant::NODE_PATH, scroll_container)
  GD_PROPERTY_BIND(item_description_popout, Variant::NODE_PATH, label_container)
  GD_PROPERTY_BIND(item_description_popout, Variant::NODE_PATH, rich_text_label)

  GD_PROPERTY_BIND(item_description_popout, Variant::FLOAT, set_vscroll_norm)
  GD_PROPERTY_BIND(item_description_popout, Variant::FLOAT, scroll_speed)

  ClassDB::bind_method(
    D_METHOD("_on_animation_done",
      "anim_name"
    ),

    &item_description_popout::_on_animation_done
  );

  ClassDB::bind_method(
    D_METHOD("_on_animation_started",
      "anim_name"
    ),

    &item_description_popout::_on_animation_started
  );
}


item_description_popout::item_description_popout(){
  _animation_player = NULL;
  _scroll_container = NULL;
  _rt_label = NULL;
}


void item_description_popout::_ready(){
  INIT_ASSERT()
  Array _paramarr;

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    CREATE_GET_NODE_TESTER(_animation_player, animation_player, AnimationPlayer)
    CREATE_GET_NODE_TESTER(_scroll_container, scroll_container, ScrollContainer)
    CREATE_GET_NODE_TESTER(_label_container, label_container, Control)
    CREATE_GET_NODE_TESTER(_rt_label, rich_text_label, RichTextLabel)

    _animation_player->connect("animation_finished", Callable(this, "_on_animation_done"));
    _animation_player->connect("animation_started", Callable(this, "_on_animation_started"));
  }
}


void item_description_popout::bind_item_data(const item_object* item){
  // TODO check based on type
  _rt_label->set_text(item->get_description());
}


void item_description_popout::do_pop(){
  set_visible(true);

  double _val = 0;
  if(_currently_transitioning)
    _val = _get_current_anim_length_normalized();

  _animation_player->play(ANIMATION_POPOUT);
  _set_current_anim_length_normalized(_val);
}

void item_description_popout::do_hide(){
  double _val = 0;
  if(_currently_transitioning)
    _val = _get_current_anim_length_normalized();

  _animation_player->play(ANIMATION_HIDE);
  _set_current_anim_length_normalized(_val);
}