#include "algorithm_node.h"
#include "animation_interfacer.h"

#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/sprite_frames.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "vector"


using namespace Game;
using namespace Game::Animation;
using namespace Game::Misc::Algorithm;
using namespace GameUtils::Memory;
using namespace godot;


ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_DEFINE(animation_interfacer)
ATTR_BASE_INTERFACE_HELPER_DEFINE(animation_interfacer)
ATTR_INTERFACE_HELPER_DEFINE(animation_interfacer)
ATTR_PARENT_CHILD_WATCHER_HELPER_DEFINE(animation_interfacer)


// MARK: Helper class #1
class _animation_interface_animation_player: public I_animation{
  private:
    AnimationPlayer* _this_node;

  public:
    ~_animation_interface_animation_player(){}

    static I_animation* create_this_interface(Node* this_node){
      if(this_node->get_class() != "AnimationPlayer")
        return NULL;

      _animation_interface_animation_player* _new_node = new _animation_interface_animation_player();
      _new_node->_this_node = (AnimationPlayer*)this_node;

      return _new_node;
    }


    void play(StringName name, float custom_blend, float custom_speed, bool from_end) override{
      _this_node->play(name, custom_blend, custom_speed, from_end);
    }

    void play_backwards(StringName name, float custom_blend) override{
      _this_node->play(name, custom_blend);
    }

    
    void pause() override{
      _this_node->pause();
    }

    void stop() override{
      _this_node->stop();
    }


    bool is_playing() const override{
      return _this_node->is_playing();
    }


    float get_speed_scale() const override{
      return _this_node->get_speed_scale();
    }

    void set_speed_scale(float speed) override{
      _this_node->set_speed_scale(speed);
    }


    void seek(float seconds, bool update, bool update_only) override{
      _this_node->seek(seconds, update);
    }

    float get_animation_length() const override{
      return _this_node->get_current_animation_length();
    }

    float get_animation_position() const override{
      return _this_node->get_current_animation_position();
    }
};

class _animation_interface_animated_sprite2d: public I_animation{
  private:
    struct _sprite_frame_metadata{
      int _frame_count;
      float _animation_speed;
      float _total_frame_duration;

      std::vector<float> _duration_list;
    };

    AnimatedSprite2D* _this_node;

    std::map<String, _sprite_frame_metadata*> _sprite_frames_metadata_map;

    void _generate_metadata(StringName anim){
      Ref<SpriteFrames> _frames = _this_node->get_sprite_frames();
      if(_frames.is_valid()){
        if(!_frames->has_animation(anim))
          return;

        _sprite_frame_metadata* _metadata = NULL;
        {auto _iter = _sprite_frames_metadata_map.find(anim);
          if(_iter == _sprite_frames_metadata_map.end()){
            _metadata = new _sprite_frame_metadata();
            _sprite_frames_metadata_map[anim] = _metadata;
          }
          else
            _metadata = _iter->second;
        }

        _metadata->_frame_count = _frames->get_frame_count(anim);
        _metadata->_animation_speed = _frames->get_animation_speed(anim);
        _metadata->_total_frame_duration = 0;
        _metadata->_duration_list.resize(_metadata->_frame_count);
        for(int i = 0; i < _metadata->_frame_count; i++){
          float _frame_duration = _frames->get_frame_duration(anim, i);
          _metadata->_duration_list[i] = _frame_duration;
          _metadata->_total_frame_duration += _frame_duration;
        }
      }
    }

    _sprite_frame_metadata* _get_metadata_const(StringName anim) const{
      auto _iter = _sprite_frames_metadata_map.find(anim);
      if(_iter == _sprite_frames_metadata_map.end())
        return NULL;

      return _iter->second;
    }

    _sprite_frame_metadata* _get_metadata(StringName anim){
      auto _iter = _sprite_frames_metadata_map.find(anim);
      if(_iter == _sprite_frames_metadata_map.end()){
        _generate_metadata(anim);
        _iter = _sprite_frames_metadata_map.find(anim);
      }

      if(_iter == _sprite_frames_metadata_map.end())
        return NULL;

      return _iter->second;
    }


  public:
    _animation_interface_animated_sprite2d(AnimatedSprite2D* anim_node){
      Ref<SpriteFrames> _anim_sf = anim_node->get_sprite_frames();
      if(_anim_sf.is_null())
        return;

      PackedStringArray _anim_names = _anim_sf->get_animation_names();
      for(int i = 0; i < _anim_names.size(); i++)
        _generate_metadata(_anim_names[i]);
    }

    ~_animation_interface_animated_sprite2d(){
      for(auto _pair: _sprite_frames_metadata_map)
        delete _pair.second;
    }

    static I_animation* create_this_interface(Node* this_node){
      if(this_node->get_class() != "AnimatedSprite2D")
        return NULL;

      _animation_interface_animated_sprite2d* _new_node = new _animation_interface_animated_sprite2d((AnimatedSprite2D*)this_node);
      return _new_node;
    }


    void play(StringName name, float custom_blend, float custom_speed, bool from_end) override{
      _this_node->play(name, custom_speed, from_end);
    }

    void play_backwards(StringName name, float custom_blend) override{
      _this_node->play_backwards(name);
    }


    void pause() override{
      _this_node->pause();
    }

    void stop() override{
      _this_node->stop();
    }


    bool is_playing() const override{
      return _this_node->is_playing();
    }


    float get_speed_scale() const override{
      return _this_node->get_speed_scale();
    }

    void set_speed_scale(float speed) override{
      _this_node->set_speed_scale(speed);
    }


    // this will looped
    void seek(float seconds, bool update, bool update_only){
      StringName _anim_name = _this_node->get_animation();
      _sprite_frame_metadata* _metadata = _get_metadata(_anim_name);
      if(!_metadata)
        return;

      float _animation_speed = _metadata->_animation_speed * abs(_this_node->get_speed_scale());
      float _animation_duration = _metadata->_total_frame_duration / _animation_speed;
      
      {float _seek_loop = seconds / _animation_duration;
        _seek_loop -= floor(_seek_loop);
        seconds = _seek_loop * _animation_duration;
      }

      float _tmp_second = seconds;
      for(int i = 0; i < _metadata->_duration_list.size(); i++){
        float _duration_second = _metadata->_duration_list[i] / _animation_speed;
        if(_tmp_second >= _duration_second){
          _tmp_second -= _duration_second;
          continue;
        }
        else{
          _this_node->set_frame_and_progress(i, _tmp_second/_duration_second);
          return;
        }
      }

      // if the code reaches here, then there's something wrong with the metadata, try to check it
      _generate_metadata(_anim_name);
    }

    float get_animation_length() const override{
      StringName _anim_name = _this_node->get_animation();
      _sprite_frame_metadata* _metadata = _get_metadata_const(_anim_name);
      if(!_metadata)
        return 0;

      float _animation_speed = _metadata->_animation_speed * abs(_this_node->get_speed_scale());
      return _metadata->_total_frame_duration / _animation_speed;
    }

    float get_animation_position() const override{
      float _res = 0;
      StringName _anim_name = _this_node->get_animation();
      _sprite_frame_metadata* _metadata = _get_metadata_const(_anim_name);
      if(_metadata){
        // don't include the current frame duration, since still progressing
        int _current_frame_idx = _this_node->get_frame()-1;
        for(int i = 0; i < _current_frame_idx; i++)
          _res += _metadata->_duration_list[i];

        _res += _metadata->_duration_list[_current_frame_idx] * _this_node->get_frame_progress();

        float _animation_speed = _metadata->_animation_speed * abs(_this_node->get_speed_scale());
        _res = _res / _animation_speed;
      }

      return _res;
    }


    bool get_looping() const override{
      Ref<SpriteFrames> _anim_sf = _this_node->get_sprite_frames();
      if(_anim_sf.is_null())
        return false;

      StringName _anim_name = _this_node->get_animation();
      return _anim_sf->get_animation_loop(_anim_name);
    }

    void set_looping(bool looping) override{
      Ref<SpriteFrames> _anim_sf = _this_node->get_sprite_frames();
      if(_anim_sf.is_null())
        return;

      StringName _anim_name = _this_node->get_animation();
      _anim_sf->set_animation_loop(_anim_name, looping);
    }

    
    bool get_centered() const override{
      return _this_node->is_centered();
    }

    void set_centered(bool centered) override{
      _this_node->set_centered(centered);
    }


    bool get_flip_h() const override{
      return _this_node->is_flipped_h();
    }

    void set_flip_h(bool flip) override{
      _this_node->set_flip_h(flip);
    }


    bool get_flip_v() const override{
      return _this_node->is_flipped_v();
    }

    void set_flip_v(bool flip) override{
      _this_node->set_flip_v(flip);
    }


    Vector2 get_offset() const override{
      return _this_node->get_offset();
    }

    void set_offset(Vector2 offset){
      _this_node->set_offset(offset);
    }


    void on_update(StringName anim_name) override{
      _generate_metadata(anim_name);
    }
};


typedef I_animation*(*_animation_interface_instantiator)(Node*);

// MARK: Static data
struct static_data: public custom_data{
  public:
    static String get_id(){ return "animation_interfacer.static_data"; }

    std::map<std::string, _animation_interface_instantiator> _interface_instantiator_map = {
      {"AnimationPlayer", _animation_interface_animation_player::create_this_interface},
      {"AnimatedSprite2D", _animation_interface_animated_sprite2d::create_this_interface}
    };
};



// MARK: This class define

void animation_interfacer::_on_child_entered(Node* _node){
  static_data* _static = _static_data->get_data<static_data>(); 


  bool _using_own_interfacer = false;
  I_animation* _interface = NULL;

  auto _instantiator_iter = _static->_interface_instantiator_map.find(GDSTR_TO_STDSTR(_node->get_class()));
  if(_instantiator_iter != _static->_interface_instantiator_map.end()){
    _using_own_interfacer = true;
    _interface = _instantiator_iter->second(_node);
  }
  else{
    _using_own_interfacer = false;
    INTERFACE_GET(_node, I_animation, _interface);
  }

  if(!_interface)
    return;

  _interface_metadata _metadata;{
    _metadata._this_node = _node;
    _metadata._animation_interface = _interface;

    _metadata._using_own_interfacer = _using_own_interfacer;
  }

  _node_metadata[_node->get_instance_id()] = _metadata;
}

void animation_interfacer::_on_child_exited(Node* _node){
  if(_node->get_instance_id() == get_instance_id())
    _remove_all_metadata();

  auto _iter = _node_metadata.find(_node->get_instance_id());
  if(_iter == _node_metadata.end())
    return;

  if(_iter->second._using_own_interfacer)
    delete _iter->second._animation_interface;

  _node_metadata.erase(_iter);
}


void animation_interfacer::_remove_all_metadata(){
  auto _iter = _node_metadata.begin();
  while(_iter != _node_metadata.end()){
    if(_iter->second._using_own_interfacer)
      delete _iter->second._animation_interface;

    _node_metadata.erase(_iter);
    _iter = _node_metadata.begin();
  }
}


void animation_interfacer::_bind_methods(){
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_BIND_GODOT(animation_interfacer)
  ATTR_BASE_INTERFACE_HELPER_BIND_GODOT(animation_interfacer)
  ATTR_INTERFACE_HELPER_BIND_GODOT(animation_interfacer)
  ATTR_PARENT_CHILD_WATCHER_HELPER_BIND_GODOT(animation_interfacer)
}


void animation_interfacer::_on_gamenode_ready(){
  INIT_ASSERT()
  Array _paramarr;

  GET_STATIC_DATA(static_data, _static_data)
  static_data* _static = _static_data->get_data<static_data>();
  if(!_static){
    ERR_ASSERT("Cannot get static data.")
    throw ERR_UNCONFIGURED;
  }

  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){

    // TODO check if notification of parented is enough
    __child_watcher_change_parent(get_parent());
  }
}


void animation_interfacer::_notification(int code){
  Engine* _engine = Engine::get_singleton();
  if(!_engine->is_editor_hint()){
    ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_ON_NOTIFICATION(code)
    ATTR_BASE_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_INTERFACE_HELPER_ON_NOTIFICATION(code)
    ATTR_PARENT_CHILD_WATCHER_HELPER_ON_NOTIFICATION(code)

    switch(code){
      break; case NOTIFICATION_PARENTED:{
        __child_watcher_change_parent(get_parent());
      }

      break; case NOTIFICATION_UNPARENTED:{
        __child_watcher_change_parent(NULL);
      }
    }
  }

  switch(code){
    break; case NOTIFICATION_PREDELETE:{
      for(auto _pair: _node_metadata){
        if(_pair.second._using_own_interfacer)
          delete _pair.second._animation_interface;
      }
    }
  }
}


void animation_interfacer::play(StringName name, float custom_blend, float custom_speed, bool from_end){
  _start_voidfunc_interface(&I_animation::play, name, custom_blend, custom_speed, from_end);
}

void animation_interfacer::play_backwards(StringName name, float custom_blend){
  _start_voidfunc_interface(&I_animation::play_backwards, name, custom_blend);
}


void animation_interfacer::pause(){
  _start_voidfunc_interface(&I_animation::pause);
}

void animation_interfacer::stop(){
  _start_voidfunc_interface(&I_animation::stop);
}


std::map<uint64_t, bool> animation_interfacer::is_playing() const{
  return _start_funcconst_interface(&I_animation::is_playing);
}


std::map<uint64_t, float> animation_interfacer::get_speed_scale() const{
  return _start_funcconst_interface(&I_animation::get_speed_scale);
}

void animation_interfacer::set_speed_scale(float speed){
  _start_voidfunc_interface(&I_animation::set_speed_scale, speed);
}


void animation_interfacer::seek(float seconds, bool update, bool update_only){
  _start_voidfunc_interface(&I_animation::seek, seconds, update, update_only);
}

std::map<uint64_t, float> animation_interfacer::get_animation_length() const{
  return _start_funcconst_interface(&I_animation::get_animation_length);
}

std::map<uint64_t, float> animation_interfacer::get_animation_position() const{
  return _start_funcconst_interface(&I_animation::get_animation_position);
}


std::map<uint64_t, bool> animation_interfacer::get_looping() const{
  return _start_funcconst_interface(&I_animation::get_looping);
}

void animation_interfacer::set_looping(bool looping){
  _start_voidfunc_interface(&I_animation::set_looping, looping);
}


std::map<uint64_t, bool> animation_interfacer::get_centered() const{
  return _start_funcconst_interface(&I_animation::get_centered);
}

void animation_interfacer::set_centered(bool centered){
  _start_voidfunc_interface(&I_animation::set_centered, centered);
}


std::map<uint64_t, bool> animation_interfacer::get_flip_h() const{
  return _start_funcconst_interface(&I_animation::get_flip_h);
}

void animation_interfacer::set_flip_h(bool flip){
  _start_voidfunc_interface(&I_animation::set_flip_h, flip);
}


std::map<uint64_t, bool> animation_interfacer::get_flip_v() const{
  return _start_funcconst_interface(&I_animation::get_flip_v); 
}

void animation_interfacer::set_flip_v(bool flip){
  _start_voidfunc_interface(&I_animation::set_flip_v, flip);
}


std::map<uint64_t, Vector2> animation_interfacer::get_offset() const{
  return _start_funcconst_interface(&I_animation::get_offset);
}

void animation_interfacer::set_offset(Vector2 offset){
  _start_voidfunc_interface(&I_animation::set_offset, offset);
}


void animation_interfacer::on_update(StringName anim_name){
  _start_voidfunc_interface(&I_animation::on_update, anim_name);
}