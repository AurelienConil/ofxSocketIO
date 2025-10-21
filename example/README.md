# Socket.IO Client C++ pour OpenFrameworks

Ce sous-répertoire contient un exemple d'utilisation de l'addon ofxSocketIO avec Socket.IO Client C++ moderne.

## Installation

1. Exécutez le script d'installation à la racine de l'addon:
   ```bash
   cd ../
   chmod +x install_socketio.sh
   ./install_socketio.sh
   ```

2. Retournez dans le dossier example et compilez:
   ```bash
   cd example
   make
   ```

## Serveur de test

Un serveur de test est disponible dans le dossier `script/`:

```bash
cd script
npm install
node index.js
```

Cela démarre un serveur Socket.IO sur le port 8888.