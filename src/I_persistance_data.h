#ifndef I_PERSISTANCE_DATA_HEADER
#define I_PERSISTANCE_DATA_HEADER


namespace Game::Persistance{
  class I_persistance_data{
    public:
      virtual ~I_persistance_data(){}

      virtual bool is_available(){ return false; }
  };
};

#endif