#ifndef SPRITE_RESOURCE_DATA_HEADER
#define SPRITE_RESOURCE_DATA_HEADER

#include "custom_memory.h"

#include "godot_cpp/classes/image_texture.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource.hpp"


namespace Game{
  class sprite_resource_data: public GameUtils::Memory::custom_data{
    private:
      godot::String _full_id;

      godot::Ref<godot::Resource> _image_res;
      godot::Ref<godot::Resource> _animation_library;
      godot::Ref<godot::PackedScene> _custom_object_sprite; 

      void _parse_sprite_resource_data_dict(godot::Dictionary* data);

    public:
      ~sprite_resource_data(){}

      static godot::String get_subdata_name();
      static void parse_dictionary_to(GameUtils::Memory::custom_memory* mem, godot::Dictionary* data);

      void copy_subdata_to(GameUtils::Memory::custom_memory* mem) const;

      godot::String get_full_id() const;

      godot::Ref<godot::Resource> get_image() const;
      godot::Ref<godot::Resource> get_animation_library() const;
      godot::Ref<godot::PackedScene> get_custom_object_sprite() const;
    
  };
}

#endif