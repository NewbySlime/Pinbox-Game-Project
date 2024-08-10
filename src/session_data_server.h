#ifndef SESSION_DATA_SERVER_HEADER
#define SESSION_DATA_SERVER_HEADER

#include "subautoload_container.h"
#include "subdata_container.h"


namespace Game{
  class session_data_server: public godot::Node,
    public GameUtils::ParsingHelper::subdata_container,
    public GameUtils::Interface::I_interface_class,
    public Game::I_subautoload_compatible{
  GDCLASS(session_data_server, godot::Node)
  ATTR_SUBAUTOLOAD_COMPATIBLE_CLASS_INHERIT(session_data_server)
  ATTR_BASE_INTERFACE_HELPER_INHERIT(session_data_server)
  ATTR_INTERFACE_HELPER_INHERIT(session_data_server)

    protected:
      static void _bind_methods();
      

    public:
      void _notification(int code);
    
      void remove_all_subdata();


      template<typename subdata_class> subdata_class* get_subdata() const{
        return __get_subdata<subdata_class>();
      }

      template<typename subdata_class> std::shared_ptr<subdata_class> get_subdata_shared() const{
        return __get_subdata_shared<subdata_class>();
      }


      template<typename subdata_class> void set_subdata(subdata_class* from){
        __set_subdata<subdata_class>(from);
      }

      
      template<typename subdata_class> void remove_subdata(){
        __remove_subdata<subdata_class>():
      }
  };
}

#endif