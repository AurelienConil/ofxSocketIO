//
//  ofxSocketIO.h
//
//  Created by hugohil on 05/02/16.
//  Updated with modern Socket.IO Client C++ API
//
#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"
#include <functional>
#include <map>
#include <string>
#include <mutex>

// Forward declaration pour éviter l'inclusion circulaire
class ofxSocketIOData;

class ofxSocketIO {
private:
  sio::client _io;
  std::mutex _mutex;
  std::map<std::string, sio::socket::ptr> _sockets;
  std::string _currentStatus;

  void onConnect();
  void onClose(sio::client::close_reason const& reason);
  void onFail();
  void onTryReconnect();
  
  // Helper pour obtenir ou créer un socket
  sio::socket::ptr getSocketForNamespace(std::string const& nsp);
  
  // Implémentation réelle du binding d'événements
  template<typename T>
  void bindEventInternal(ofEvent<T>& event, std::string const& eventName, std::string const& nsp);

public:
  ofxSocketIO();
  ~ofxSocketIO();

  std::string getStatus() const;

  void setup(std::string const& address);
  void setup(std::string const& address, std::map<std::string,std::string> const& query);

  // Binding d'événements
  template<typename T>
  void bindEvent(ofEvent<T>& event, std::string const& eventName) {
      bindEventInternal(event, eventName, "");
  }
  
  template<typename T>
  void bindEvent(ofEvent<T>& event, std::string const& eventName, std::string const& nsp) {
      bindEventInternal(event, eventName, nsp);
  }

  ofEvent<void> connectionEvent;
  ofEvent<std::string> notifyEvent;

  void emit(std::string const& eventName);
  void emit(std::string const& eventName, std::string const& data);
  void emit(std::string const& eventName, std::string const& data, std::string const& nsp);
  void emitBinary(std::string const& eventName, std::shared_ptr<std::string> const& bStr, std::string nsp = "");

  void closeConnection();
  void openConnection(std::string const& address);
};
