#include "ofxSocketIOData.h"

// Constructeur par défaut
ofxSocketIOData::ofxSocketIOData() : _data(nullptr) {
}

// Constructeur avec données
ofxSocketIOData::ofxSocketIOData(sio::message::ptr const& data) : _data(data) {
}

void ofxSocketIOData::setData(sio::message::ptr const& data) {
  _data = data;
}

void ofxSocketIOData::setNullData() {
  _data = nullptr;
}

std::vector<std::shared_ptr<sio::message>>& ofxSocketIOData::getRawVector() {
  return _data->get_vector();
}

std::vector<std::shared_ptr<std::string>> ofxSocketIOData::getVector() const {
  std::vector<std::shared_ptr<std::string>> result;
  
  if (!_data || _data->get_flag() != sio::message::flag_array) {
      return result;
  }
  
  const auto& array = _data->get_vector();
  for (const auto& element : array) {
      if (element->get_flag() == sio::message::flag_string) {
          result.push_back(std::make_shared<std::string>(element->get_string()));
      }
  }
  
  return result;
}

std::string ofxSocketIOData::getStringValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || _data->get_map().at(key)->get_flag() != sio::message::flag_string) {
      return "";
  }
  return _data->get_map().at(key)->get_string();
}

int ofxSocketIOData::getIntValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || 
      (_data->get_map().at(key)->get_flag() != sio::message::flag_integer &&
       _data->get_map().at(key)->get_flag() != sio::message::flag_double)) {
      return 0;
  }
  
  if (_data->get_map().at(key)->get_flag() == sio::message::flag_integer) {
      return _data->get_map().at(key)->get_int();
  } else {
      return static_cast<int>(_data->get_map().at(key)->get_double());
  }
}

float ofxSocketIOData::getFloatValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || 
      (_data->get_map().at(key)->get_flag() != sio::message::flag_integer &&
       _data->get_map().at(key)->get_flag() != sio::message::flag_double)) {
      return 0.0f;
  }
  
  if (_data->get_map().at(key)->get_flag() == sio::message::flag_double) {
      return static_cast<float>(_data->get_map().at(key)->get_double());
  } else {
      return static_cast<float>(_data->get_map().at(key)->get_int());
  }
}

double ofxSocketIOData::getDoubleValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || 
      (_data->get_map().at(key)->get_flag() != sio::message::flag_integer &&
       _data->get_map().at(key)->get_flag() != sio::message::flag_double)) {
      return 0.0;
  }
  
  if (_data->get_map().at(key)->get_flag() == sio::message::flag_double) {
      return _data->get_map().at(key)->get_double();
  } else {
      return static_cast<double>(_data->get_map().at(key)->get_int());
  }
}

bool ofxSocketIOData::getBoolValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || _data->get_map().at(key)->get_flag() != sio::message::flag_boolean) {
      return false;
  }
  return _data->get_map().at(key)->get_bool();
}

ofxSocketIOData ofxSocketIOData::getNestedValue(std::string const& key) const {
  if (!_data || !_data->get_map().count(key) || _data->get_map().at(key)->get_flag() != sio::message::flag_object) {
      return ofxSocketIOData();
  }
  return ofxSocketIOData(_data->get_map().at(key));
}
