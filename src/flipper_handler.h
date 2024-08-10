#ifndef FLIPPER_HANDLER_HEADER
#define FLIPPER_HANDLER_HEADER

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/input_event.hpp"

#define SIGNAL_FLIPPER_HANDLER_ON_INPUT "sfhoi"


namespace Game{
  namespace Handlers{
    class flipper_handler: public godot::Node{
    GDCLASS(flipper_handler, godot::Node)

      public:
        enum flipper_input_flag{
          left_hand_flipper = 0x1,
          right_hand_flipper = 0x2
        };

      private:
        int _input_flag;

      protected:
        static void _bind_methods();

      public:
        flipper_handler();

        void _ready() override;

        void _input(const godot::Ref<godot::InputEvent>& event) override;

        int get_input_flag();
    };
  }
}

#endif