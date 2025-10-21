#pragma once

#include "ofMain.h"
#include "sio_client.h"
#include "sio_message.h"

class ofxSocketIOData : public ofEventArgs {
private:
  sio::message::ptr _data;

public:
  ofxSocketIOData();
  ofxSocketIOData(sio::message::ptr const& data);

  void setData(sio::message::ptr const& data);
  void setNullData();

  std::string getStringValue(std::string const& key) const;
  int getIntValue(std::string const& key) const;
  float getFloatValue(std::string const& key) const;
  double getDoubleValue(std::string const& key) const;
  bool getBoolValue(std::string const& key) const;
  ofxSocketIOData getNestedValue(std::string const& key) const;

  std::vector<std::shared_ptr<std::string>> getVector() const;
  std::vector<std::shared_ptr<sio::message>>& getRawVector();
};
