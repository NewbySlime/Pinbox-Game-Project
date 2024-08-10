#ifndef ITEM_DESCRIPTION_POPOUT_HEADER
#define ITEM_DESCRIPTION_POPOUT_HEADER

#include "godot_macro.h"

#include "item_object.h"

#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/rich_text_label.hpp"
#include "godot_cpp/classes/scroll_container.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "godot_cpp/classes/v_box_container.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/string.hpp"


namespace Game::Item{
  class item_description_popout: public godot::Control{
  GDCLASS(item_description_popout, godot::Control)

    GD_PROPERTY_VAR(godot::NodePath, animation_player)
    GD_PROPERTY_VAR(godot::NodePath, scroll_container)
    GD_PROPERTY_VAR(godot::NodePath, label_container)
    GD_PROPERTY_VAR(godot::NodePath, rich_text_label)

    GD_PROPERTY_VAR_WITHOUT_DEF_FUNC(double, set_vscroll_norm)
    GD_PROPERTY_VAR(double, scroll_speed)

    private:
      godot::AnimationPlayer* _animation_player;
      godot::ScrollContainer* _scroll_container;
      godot::Control* _label_container;
      godot::RichTextLabel* _rt_label;

      bool _currently_transitioning = false;
      godot::String _next_animation;

      void _on_animation_done(godot::String name);
      void _on_animation_started(godot::String name);

      double _get_scroll_speed_anim_scale();

      double _get_current_anim_length_normalized();
      void _set_current_anim_length_normalized(double _val);

    protected:
      static void _bind_methods();

    public:
      item_description_popout();

      void _ready() override;

      void bind_item_data(const Game::Item::item_object* item);

      void do_pop();
      void do_hide();
  };
}

#endif