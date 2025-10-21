# ofxSocketIO

### Description

`ofxSocketIO` est un addon pour OpenFrameworks qui intègre la bibliothèque officielle [`socket.io-client-cpp`](https://github.com/socketio/socket.io-client-cpp) pour la communication en temps réel avec Socket.IO.

### Installation

Pour installer et compiler la bibliothèque Socket.IO Client C++, utilisez le script d'installation fourni :

```bash
chmod +x install_socketio.sh
./install_socketio.sh
```

Ce script va automatiquement :
1. Détecter votre système d'exploitation (macOS, Linux, RaspberryPi)
2. Télécharger le dépôt Socket.IO Client C++ dans `libs/socket.io-client-cpp`
3. Compiler les bibliothèques nécessaires
4. Copier les fichiers d'en-tête dans `libs/socket.io-client/include`
5. Copier les bibliothèques compilées dans `libs/socket.io-client/lib/[plateforme]`

#### Options CMake

L'addon fournit également un fichier CMakeLists.txt qui peut être utilisé pour configurer et compiler votre projet. Deux options principales sont disponibles :

- `USE_PRECOMPILED_LIBS` (ON par défaut) : Utilise les bibliothèques précompilées si disponibles
- `BUILD_FROM_SOURCE` (OFF par défaut) : Compile les bibliothèques à partir des sources

#### Support multiplateforme

Les bibliothèques précompilées sont disponibles pour les plateformes suivantes :
- macOS (`osx`)
- Linux 64-bit (`linux64`) 
- Linux 32-bit (`linux`)
- Raspberry Pi (`linuxarmv6l`, `linuxarmv7l`)

#### TLS/SSL

La nouvelle implémentation inclut le support TLS/SSL par défaut.

#### Version moderne et version classique

Cette version de l'addon offre deux implémentations distinctes :

1. **ofxSocketIO** - Implémentation classique
2. **ofxSocketIOModern** - Implémentation moderne avec les dernières fonctionnalités Socket.IO

### Utilisation

Un exemple complet est disponible dans le dossier [example](https://github.com/AurelienConil/ofxSocketIO/tree/master/example).

Vous aurez besoin d'un [serveur Socket.IO](http://www.socket.io) pour communiquer. Un exemple de serveur de test est disponible dans [script](https://github.com/AurelienConil/ofxSocketIO/tree/master/script).

### Exemple de base (version classique)

```cpp
#include "ofxSocketIO.h"

// Dans votre classe
ofxSocketIO socketIO;
ofEvent<ofxSocketIOData> customEvent;

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

### Exemple avec la version moderne

```cpp
#include "ofxSocketIOModern.h"

// Dans votre classe
ofxSocketIOModern socketIO;
ofEvent<ofxSocketIOData> customEvent;

// Dans la fonction setup()
socketIO.setup("http://localhost:8888");
socketIO.onConnect([this](void) {
    ofLogNotice() << "Connecté au serveur";
    
    // Bind des événements avec la nouvelle API basée sur des lambdas
    socketIO.on("custom-event", [this](ofxSocketIOData& data) {
        std::string message = data.getStringValue("message");
        int value = data.getIntValue("value");
        ofLogNotice() << "Message reçu: " << message << ", valeur: " << value;
    });
    
    // Émettre un événement avec des données structurées
    socketIO.emit("hello", {
        {"message", "world"},
        {"value", 42},
        {"array", std::vector<std::string>{"item1", "item2"}}
    });
});

// Gestionnaire d'erreur
socketIO.onError([](void) {
    ofLogError() << "Erreur de connexion Socket.IO";
});
```

### API

#### ofxSocketIO (Version classique)

##### `setup(std::string const& address)`
&rarr; Crée une connexion websocket avec l'adresse distante.

##### `setup(std::string const& address, std::map<std::string,std::string> const& query)`
&rarr; Crée une connexion websocket avec l'adresse distante et des paramètres de requête.

##### `bindEvent(ofEvent<T>& event, std::string const& eventName)`
&rarr; Lie un événement `ofEvent` à un déclencheur Socket.IO.

##### `bindEvent(ofEvent<T>& event, std::string const& eventName, std::string const& nsp)`
&rarr; Lie un événement `ofEvent` à un déclencheur Socket.IO dans un namespace spécifique.

##### `getStatus()`
&rarr; Retourne l'état actuel de la connexion. Peut être `connected`, `closed`, `reconnecting` ou `errored`.

##### `connectionEvent`
&rarr; Un événement `ofEvent` déclenché lorsque le socket est connecté au serveur.

##### `notifyEvent`
&rarr; Un événement `ofEvent` déclenché lorsque l'état du socket change. Fournit un paramètre `std::string` indiquant l'état.

##### `emit(std::string const& eventName)`
&rarr; Émet un événement sans données.

##### `emit(std::string const& eventName, std::string const& data)`
&rarr; Émet un événement avec un nom et des données (string).

##### `emit(std::string const& eventName, std::string const& data, std::string const& nsp)`
&rarr; Émet un événement avec un nom et des données (string) dans un namespace spécifique.

##### `emitBinary(std::string const& eventName, std::shared_ptr<std::string> const& bStr, std::string nsp = "")`
&rarr; Émet un événement avec des données binaires.

##### `closeConnection()`
&rarr; Ferme la connexion de force.

##### `openConnection(std::string const& address)`
&rarr; Ouvre une nouvelle connexion à l'adresse spécifiée.

#### ofxSocketIOModern (Version moderne)

##### `setup(std::string const& address, std::map<std::string, std::string> const& query = {})`
&rarr; Crée une connexion websocket avec l'adresse distante et des paramètres de requête optionnels.

##### `onConnect(std::function<void(void)> callback)`
&rarr; Définit une fonction de rappel à exécuter lors de la connexion.

##### `onDisconnect(std::function<void(void)> callback)`
&rarr; Définit une fonction de rappel à exécuter lors de la déconnexion.

##### `onError(std::function<void(void)> callback)`
&rarr; Définit une fonction de rappel à exécuter en cas d'erreur.

##### `onReconnecting(std::function<void(int attempts)> callback)`
&rarr; Définit une fonction de rappel à exécuter lors d'une tentative de reconnexion.

##### `on(std::string const& eventName, std::function<void(ofxSocketIOData&)> callback)`
&rarr; Lie une fonction de rappel à un événement Socket.IO spécifique.

##### `on(std::string const& eventName, std::string const& nsp, std::function<void(ofxSocketIOData&)> callback)`
&rarr; Lie une fonction de rappel à un événement Socket.IO dans un namespace spécifique.

##### `emit(std::string const& eventName)`
&rarr; Émet un événement sans données.

##### `emit(std::string const& eventName, std::string const& data)`
&rarr; Émet un événement avec un nom et des données (string).

##### `emit(std::string const& eventName, const json& data)`
&rarr; Émet un événement avec un nom et des données JSON (utilise la bibliothèque nlohmann::json).

##### `emit(std::string const& eventName, std::string const& data, std::string const& nsp)`
&rarr; Émet un événement avec un nom et des données (string) dans un namespace spécifique.

##### `emit(std::string const& eventName, const json& data, std::string const& nsp)`
&rarr; Émet un événement avec un nom et des données JSON dans un namespace spécifique.

##### `emitBinary(std::string const& eventName, std::shared_ptr<std::string> const& bStr, std::string const& nsp = "")`
&rarr; Émet un événement avec des données binaires.

##### `isConnected()`
&rarr; Vérifie si le socket est actuellement connecté.

##### `close()`
&rarr; Ferme la connexion.

##### `open(std::string const& uri)`
&rarr; Ouvre une nouvelle connexion à l'adresse spécifiée.

#### ofxSocketIOData

##### `getStringValue(std::string const& key) const`
&rarr; Obtient la valeur `std::string` pour la clé spécifiée.

##### `getIntValue(std::string const& key) const`
&rarr; Obtient la valeur `int` pour la clé spécifiée.

##### `getFloatValue(std::string const& key) const`
&rarr; Obtient la valeur `float` pour la clé spécifiée.

##### `getDoubleValue(std::string const& key) const`
&rarr; Obtient la valeur `double` pour la clé spécifiée.

##### `getBoolValue(std::string const& key) const`
&rarr; Obtient la valeur `bool` pour la clé spécifiée.

##### `getNestedValue(std::string const& key) const`
&rarr; Obtient un objet imbriqué pour la clé spécifiée.

##### `getVector() const`
&rarr; Obtient un vecteur de strings si le message est un tableau.

##### `isNull(std::string const& key) const`
&rarr; Vérifie si une clé contient une valeur nulle.

##### `hasKey(std::string const& key) const`
&rarr; Vérifie si une clé existe dans les données.

##### `getJsonMessage() const` 
&rarr; Obtient le message JSON complet sous forme de chaîne.

### Troubleshooting

#### Ping pong

Vous pouvez rencontrer des problèmes avec les événements `'ping'` et `'pong'`, car ils sont utilisés en interne par socket.io :

```
- `ping` : Déclenché lorsqu'un paquet ping est envoyé au serveur.
- `pong` : Déclenché lorsqu'un pong est reçu du serveur.
```

*[source](https://github.com/socketio/socket.io-client/issues/1022)*

#### Problèmes de compilation

Si vous rencontrez des problèmes de compilation :

1. Assurez-vous que le script `install_socketio.sh` a été exécuté
2. Vérifiez que les bibliothèques pour votre plateforme sont présentes dans `libs/socket.io-client/lib/[plateforme]`
3. Si vous utilisez CMake, définissez l'option `USE_PRECOMPILED_LIBS=OFF` pour compiler depuis les sources

#### Support de iOS / Android

Cette version ne prend pas officiellement en charge iOS ou Android, bien que les bases du support soient présentes. Des contributions sont bienvenues pour améliorer le support de ces plateformes.

### Migration vers la version moderne

La version moderne `ofxSocketIOModern` est conçue pour être plus facile à utiliser et offre une meilleure intégration avec les fonctionnalités modernes de C++. Voici quelques différences clés :

1. Utilisation de lambdas et de fonctions de rappel au lieu d'événements OF
2. Support amélioré des données JSON avec la bibliothèque nlohmann::json
3. API plus intuitive pour la connexion et les événements
4. Meilleure gestion des erreurs et des types null

### Credits

`ofxSocketIO` a été initialement créé et maintenu par [Soixante circuits](http://www.soixantecircuits.fr) et [hugohil](http://github.com/hugohil).

Il existe grâce au excellent travail des [contributeurs de socket.io-client-cpp](https://github.com/socketio/socket.io-client-cpp/graphs/contributors).

Ces personnes ont également apporté des contributions utiles à `ofxSocketIO` :
- [muraji](https://github.com/muraji)
- [thorsonmscott](https://github.com/thorsonmscott)
- [smulube](https://github.com/smulube)
- [sy1vain](https://github.com/sy1vain)
- [jedahan](https://github.com/jedahan)
- [AurelienConil](https://github.com/AurelienConil) - Modernisation de la bibliothèque

### License

[MIT](https://github.com/soixantecircuits/ofxSocketIO/blob/master/LICENSE)
