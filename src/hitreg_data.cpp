#include "logger.h"

#include "defines.h"
#include "godot_error.h"
#include "hitreg_data.h"


using namespace Game;
using namespace Game::Error;
using namespace Game::Item;
using namespace GameUtils::Memory;
using namespace godot;




struct _tmp_hitreg_storage{
  hit_register_data data;
  std::vector<String> next_node;
};

// helper function to create a hitreg tree from "parsed" json data
void __create_hitreg_tree(
  std::map<String, _tmp_hitreg_storage*>& hitreg_lists,
  std::map<String, hit_register_data*>& node_map,
  hit_register_data_container& hitreg_container,
  
  hit_register_data* _parent_node,
  _tmp_hitreg_storage* _parent_data
){
  for(int i = 0; i < _parent_data->next_node.size(); i++){
    String _next_key = _parent_data->next_node[i];
    auto _iter_next_node = node_map.find(_next_key);
    auto _iter_next_data = hitreg_lists.find(_next_key);
    if(_iter_next_node != node_map.end()){
      // if there's a connection from before
    }
    else if(_iter_next_data != hitreg_lists.end()){
      // if the data hasn't been instantiated
      hit_register_data* _child_node = hitreg_container.add_node(_parent_node, &_iter_next_data->second->data);

      node_map[_next_key] = _child_node;
      __create_hitreg_tree(hitreg_lists, node_map, hitreg_container, _child_node, _iter_next_data->second);
    }
  }
};




String hitreg_data::get_subdata_name(){
  return VARNAME_WEAPON_STATS_HITREG_DATA;
}

void hitreg_data::parse_dictionary_to(custom_memory* mem, Dictionary* data){
  const std::map<String, hit_register_type_enum> hrte_str_map = {
    {"raycast", hrte_raycast}
  };


  mem->change_data<hitreg_data>();
  hitreg_data* _new_data = mem->get_data<hitreg_data>();

  Dictionary* _data_dict = data;
  Array _paramarr;
  Variant _v;

  std::map<String, _tmp_hitreg_storage*> _hitreg_lists;

  // try catch is needed for cleaning up stuff when bad happens
  try{
    // get all hitreg_data first
    Array _keys = _data_dict->keys();
    for(int i = 0; i < _keys.size(); i++){
      String _key = _keys[i];
      // creating new hitreg data with key
      _tmp_hitreg_storage* _currdata = new _tmp_hitreg_storage();
      _hitreg_lists[_key] = _currdata;

      _currdata->data.key = _key;

      _v = _data_dict->operator[](_key);
      _CREATE_CHECKER_DICT1(VARNAME_WEAPON_STATS_HITREG_DATA, _key, Variant::DICTIONARY)
      Dictionary _data_part = _v;

      _paramarr.clear();{
        _paramarr.append(VARNAME_WEAPON_STATS_HITREG_DATA);
        _paramarr.append(_key);
      }
      String _key_logstr = String("{0}:{1}").format(_paramarr);


      // getting hitreg type
      _CREATE_CHECKER_DICT2(_key_logstr, VARNAME_WEAPON_STATS_HITREG_DATA_TYPE, Variant::STRING)
      String _typestr = _v;

      auto _iter = hrte_str_map.find(_typestr);
      if(_iter == hrte_str_map.end()){
        _paramarr.clear();{
          _paramarr.append(_key_logstr);
          _paramarr.append(VARNAME_WEAPON_STATS_HITREG_DATA_TYPE);
          _paramarr.append(_typestr);
        }
        throw gdstr_exception(String("({0}:{1}) \"{2}\" hitreg type is invalid.").format(_paramarr));
      }
      _currdata->data.hitreg_type = _iter->second;

      // getting hitreg length
      _CREATE_CHECKER_DICT2(_key_logstr, VARNAME_WEAPON_STATS_HITREG_DATA_MAX_LENGTH, Variant::FLOAT)
      _currdata->data.length = _v;

      // getting hitreg time
      _CREATE_CHECKER_DICT2(_key_logstr, VARNAME_WEAPON_STATS_HITREG_DATA_TIME, Variant::FLOAT)
      _currdata->data.time = _v;

      // getting hitreg next node
      _CREATE_CHECKER_DICT2(_key_logstr, VARNAME_WEAPON_STATS_HITREG_DATA_NEXT_NODE, Variant::ARRAY)
      Array _nodes_link = _v;

      for(int i = 0; i < _nodes_link.size(); i++){
        String _node_key = _nodes_link[i];
        _currdata->next_node.insert(_currdata->next_node.end(), _node_key);
      }
    }


    // link the nodes
    auto _iter = _hitreg_lists.find(ITEM_DATA_HITREG_DATA_START_ID);
    if(_iter == _hitreg_lists.end()){
      _paramarr.clear();{
        _paramarr.append(VARNAME_WEAPON_STATS_HITREG_DATA);
      }
      throw gdstr_exception(String("({0}) hitreg data no starting point.").format(_paramarr));
    }

    _tmp_hitreg_storage* _start_node = _iter->second;
    _new_data->_hitreg_graph = std::shared_ptr<hit_register_data_container>(new hit_register_data_container(_start_node->data));

    std::map<String, hit_register_data*> _hitreg_nodes = {
      {_iter->first, _new_data->_hitreg_graph->get_head()}
    };


    // start create tree
    __create_hitreg_tree(_hitreg_lists, _hitreg_nodes, *_new_data->_hitreg_graph, _new_data->_hitreg_graph->get_head(), _start_node);
  }
  catch(gdstr_exception e){
    for(auto _pair: _hitreg_lists)
      delete _pair.second;

    throw e;
  }

  for(auto _pair: _hitreg_lists)
    delete _pair.second;
}


void hitreg_data::copy_subdata_to(custom_memory* mem){
  mem->change_data<hitreg_data>();
  hitreg_data* _new_data = mem->get_data<hitreg_data>();

  _new_data->_hitreg_graph = _hitreg_graph;
}


std::shared_ptr<hit_register_data_container> hitreg_data::get_hitreg_graph() const{
  return _hitreg_graph;
}