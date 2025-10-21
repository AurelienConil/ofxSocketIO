# ofxSocketIOModern

Une version modernisée de l'addon ofxSocketIO utilisant la bibliothèque officielle Socket.IO Client C++ (https://github.com/socketio/socket.io-client-cpp).

## Installation

1. Exécuter le script d'installation:
   ```bash
   chmod +x install_socketio.sh
   ./install_socketio.sh
   ```

2. Ajouter les fichiers suivants à votre projet OpenFrameworks:
   - `ofxSocketIOModern.h`
   - `ofxSocketIOModern.cpp`

3. Mettre à jour votre fichier `addons.make` pour inclure `ofxSocketIO`

## Utilisation

L'API est conçue pour être compatible avec l'ancien addon ofxSocketIO, mais utilise la nouvelle bibliothèque Socket.IO Client C++ en interne.

### Exemple de base

```cpp
#include "ofxSocketIOModern.h"

// Dans votre classe
ofxSocketIO socketIO;
ofEvent<ofxSocketIOData&> customEvent;

// Dans la fonction setup()
socketIO.setup("http://localhost:8888");
ofAddListener(socketIO.connectionEvent, this, &YourClass::onConnection);

// Fonction appelée lors de la connexion
void YourClass::onConnection() {
    // Bind des événements
    socketIO.bindEvent(customEvent, "custom-event");
    ofAddListener(customEvent, this, &YourClass::onCustomEvent);
    
    // Émettre un événement
    socketIO.emit("hello", "world");
}

// Gestionnaire d'événement
void YourClass::onCustomEvent(ofxSocketIOData& data) {
    // Accéder aux données
    std::string message = data.getStringValue("message");
    int value = data.getIntValue("value");
    
    ofLogNotice() << "Message reçu: " << message << ", valeur: " << value;
}
```

### Fonctionnalités principales

- `setup(address)` - Se connecter au serveur Socket.IO
- `setup(address, query)` - Se connecter avec des paramètres de requête
- `emit(event)` - Émettre un événement sans données
- `emit(event, data)` - Émettre un événement avec des données
- `emit(event, data, namespace)` - Émettre un événement sur un namespace spécifique
- `bindEvent(event, eventName)` - Lier un événement OF à un événement Socket.IO
- `bindEvent(event, eventName, namespace)` - Lier un événement sur un namespace

### Gestion des données

La classe `ofxSocketIOData` encapsule les données reçues et offre des méthodes pour extraire différents types:

- `getStringValue(key)` - Obtenir une valeur string
- `getIntValue(key)` - Obtenir une valeur int
- `getFloatValue(key)` - Obtenir une valeur float
- `getBoolValue(key)` - Obtenir une valeur bool
- `getNestedValue(key)` - Obtenir un objet imbriqué
- `getVector()` - Obtenir un tableau de strings

## Compatibilité

Cette implémentation est compatible avec:
- Socket.IO v3.x et v4.x côté serveur
- OpenFrameworks 0.12.0 et ultérieur