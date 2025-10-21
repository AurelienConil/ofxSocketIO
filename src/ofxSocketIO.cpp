//
//  ofxSocketIO.cpp
//
//  Created by hugohil on 05/02/16.
//  Updated with modern Socket.IO Client C++ API
//
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h" // Maintenant nécessaire à cause de la forward declaration

ofxSocketIO::ofxSocketIO() : _currentStatus("closed") {
}

ofxSocketIO::~ofxSocketIO() {
  _io.sync_close();
  _io.clear_con_listeners();
}

void ofxSocketIO::setup(std::string const& address) {
  std::map<std::string, std::string> query;
  setup(address, query);
}

void ofxSocketIO::setup(std::string const& address, std::map<std::string, std::string> const& query) {
  _currentStatus = "not connected";

  // Définir les listeners
  _io.set_open_listener(std::bind(&ofxSocketIO::onConnect, this));
  _io.set_close_listener(std::bind(&ofxSocketIO::onClose, this, std::placeholders::_1));
  _io.set_fail_listener(std::bind(&ofxSocketIO::onFail, this));
  _io.set_reconnect_listener(std::bind(&ofxSocketIO::onTryReconnect, this));
  
  // Se connecter au serveur
  if (query.empty()) {
      _io.connect(address);
  } else {
      _io.connect(address, query);
  }
}

void ofxSocketIO::onConnect() {
  _currentStatus = "connected";
  ofNotifyEvent(notifyEvent, _currentStatus);
  ofNotifyEvent(connectionEvent);
}

void ofxSocketIO::onClose(sio::client::close_reason const& reason) {
  _currentStatus = "closed";
  ofNotifyEvent(notifyEvent, _currentStatus);
}

void ofxSocketIO::onFail() {
  _currentStatus = "errored";
  ofNotifyEvent(notifyEvent, _currentStatus);
}

void ofxSocketIO::onTryReconnect() {
  _currentStatus = "reconnecting";
  ofNotifyEvent(notifyEvent, _currentStatus);
}

std::string ofxSocketIO::getStatus() const {
  return _currentStatus;
}

sio::socket::ptr ofxSocketIO::getSocketForNamespace(std::string const& nsp) {
  if (_sockets.find(nsp) == _sockets.end()) {
      _sockets[nsp] = _io.socket(nsp);
  }
  return _sockets[nsp];
}

void ofxSocketIO::emit(std::string const& eventName) {
  auto socket = getSocketForNamespace("/");
  if (socket) {
      socket->emit(eventName);
  }
}

void ofxSocketIO::emit(std::string const& eventName, std::string const& data) {
  emit(eventName, data, "/");
}

void ofxSocketIO::emit(std::string const& eventName, std::string const& data, std::string const& nsp) {
  auto socket = getSocketForNamespace(nsp);
  if (socket) {
      socket->emit(eventName, data);
  } else {
      ofLogWarning("ofxSocketIO", "socket is not available.");
  }
}

void ofxSocketIO::emitBinary(std::string const& eventName, std::shared_ptr<std::string> const& bStr, std::string nsp) {
  auto socket = getSocketForNamespace(nsp);
  if (socket) {
      socket->emit(eventName, bStr);
  } else {
      ofLogWarning("ofxSocketIO", "socket is not available.");
  }
}

void ofxSocketIO::closeConnection() {
  _currentStatus = "closed";
  _io.sync_close();
}

void ofxSocketIO::openConnection(std::string const& address) {
  _io.connect(address);
}

// Implémentation du template bindEventInternal - version générique
template<typename T>
void ofxSocketIO::bindEventInternal(ofEvent<T>& event, std::string const& eventName, std::string const& nsp) {
    // Cette version ne sera jamais utilisée, mais est nécessaire pour la compilation
    ofLogError("ofxSocketIO") << "Version générique de bindEventInternal appelée";
}

// Spécialisation du template pour ofxSocketIOData& (cas le plus courant)
template<>
void ofxSocketIO::bindEventInternal(ofEvent<ofxSocketIOData&>& event, std::string const& eventName, std::string const& nsp) {
    auto socket = getSocketForNamespace(nsp);
    
    socket->on(eventName, [&event, this, eventName](sio::event& e) {
        // Créer un nouveau pointeur partagé qui sera capturé par la lambda de l'événement
        auto dataPtr = std::make_shared<ofxSocketIOData>(e.get_message());
        
        // Notification pour l'événement de notifyEvent (string)
        std::string status = eventName;
        ofNotifyEvent(this->notifyEvent, status);
        
        // Notification avec référence à l'objet partagé
        ofxSocketIOData& dataRef = *dataPtr;
        ofNotifyEvent(event, dataRef);
    });
}

// Spécialisation du template pour void (pour les événements sans données)
template<>
void ofxSocketIO::bindEventInternal(ofEvent<void>& event, std::string const& eventName, std::string const& nsp) {
    auto socket = getSocketForNamespace(nsp);
    
    socket->on(eventName, [&event, this, eventName](sio::event& e) {
        // Notification pour l'événement de notifyEvent (string)
        std::string status = eventName;
        ofNotifyEvent(this->notifyEvent, status);
        
        // Notification pour l'événement void
        ofNotifyEvent(event);
    });
}

// Les spécialisations explicites sont définies ci-dessus, pas besoin d'explicitations supplémentaires
