#ifndef I_ANIMATION_HEADER
#define I_ANIMATION_HEADER

#include "interface_helper.h"


namespace Game::Animation{
  class I_animation{
  INTERFACE_DEFINE_INTERFACE_CLASS(I_animation)

    public:
      virtual ~I_animation(){}

      virtual void play(godot::StringName name = "", float custom_blend = -1, float custom_speed = 1, bool from_end = false){}
      virtual void play_backwards(godot::StringName name = "", float custom_blend = -1){}

      virtual void pause(){}
      virtual void stop(){}

      virtual bool is_playing() const{ return false; }

      virtual float get_speed_scale() const{ return 0; }
      virtual void set_speed_scale(float speed){}

      virtual void seek(float seconds, bool update = false, bool update_only = false){}
      virtual float get_animation_length() const{ return 0; }
      virtual float get_animation_position() const{ return 0; }

      virtual bool get_looping() const{ return false; }
      virtual void set_looping(bool looping){}


      virtual bool get_centered() const{ return false; }
      virtual void set_centered(bool centered){}

      virtual bool get_flip_h() const{ return false; }
      virtual void set_flip_h(bool flip){}

      virtual bool get_flip_v() const{ return false; }
      virtual void set_flip_v(bool flip){}

      virtual godot::Vector2 get_offset() const{ return godot::Vector2(0,0); }
      virtual void set_offset(godot::Vector2 offset){}

      virtual void on_update(godot::StringName anim_name){}
  };
}

#endif