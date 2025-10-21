#!/bin/bash

# Script pour installer la bibliothèque Socket.IO Client C++ moderne
# et l'intégrer à ofxSocketIO

# Vérifier si Git est installé
if ! command -v git &> /dev/null
then
    echo "Git n'est pas installé. Veuillez l'installer et réessayer."
    exit 1
fi

# Vérifier si CMake est installé
if ! command -v cmake &> /dev/null
then
    echo "CMake n'est pas installé. Veuillez l'installer et réessayer."
    exit 1
fi

# Emplacement actuel
CURRENT_DIR=$(pwd)
echo "Emplacement actuel: $CURRENT_DIR"

# Créer le répertoire libs s'il n'existe pas
mkdir -p libs
cd libs

# Cloner le dépôt Socket.IO Client C++
echo "Clonage de Socket.IO Client C++..."
git clone --recurse-submodules https://github.com/socketio/socket.io-client-cpp.git

# Compiler Socket.IO Client C++
echo "Compilation de Socket.IO Client C++..."
cd socket.io-client-cpp
mkdir -p build
cd build
cmake ..
make -j4
make install

# Retourner au répertoire de départ
cd "$CURRENT_DIR"

echo "Installation terminée!"
echo "Vous pouvez maintenant utiliser ofxSocketIOModern dans votre projet OpenFrameworks."