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

# Déterminer l'OS actuel et l'architecture
ARCH=$(uname -m)

if [[ "$OSTYPE" == "darwin"* ]]; then
    if [[ "$ARCH" == "arm64" ]]; then
        PLATFORM="osx_arm64"
    else
        PLATFORM="osx_x86_64"
    fi
    # Pour la compatibilité arrière, on garde aussi le dossier osx générique
    PLATFORM_GENERIC="osx"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if [[ "$ARCH" == "x86_64" ]]; then
        PLATFORM="linux64"
    elif [[ "$ARCH" == "i686" ]]; then
        PLATFORM="linux"
    elif [[ "$ARCH" == "armv6l" ]]; then
        PLATFORM="linuxarmv6l"
    elif [[ "$ARCH" == "armv7l" ]]; then
        PLATFORM="linuxarmv7l"
    elif [[ "$ARCH" == "aarch64" ]]; then
        PLATFORM="linuxaarch64"
    else
        echo "Architecture non supportée: $ARCH"
        exit 1
    fi
else
    echo "Système d'exploitation non supporté: $OSTYPE"
    exit 1
fi

echo "Plateforme détectée: $PLATFORM"

# Emplacement actuel
CURRENT_DIR=$(pwd)
echo "Emplacement actuel: $CURRENT_DIR"

# Créer les répertoires nécessaires
mkdir -p libs/socket.io-client/include
mkdir -p libs/socket.io-client/lib/$PLATFORM
# Si c'est macOS, créer aussi le dossier générique osx pour la compatibilité
if [[ "$OSTYPE" == "darwin"* ]]; then
    mkdir -p libs/socket.io-client/lib/$PLATFORM_GENERIC
fi

# Cloner le dépôt Socket.IO Client C++
echo "Clonage de Socket.IO Client C++..."
TMP_DIR=$(mktemp -d)
cd "$TMP_DIR"
git clone --recurse-submodules https://github.com/socketio/socket.io-client-cpp.git
cd socket.io-client-cpp

# Compiler Socket.IO Client C++
echo "Compilation de Socket.IO Client C++..."
mkdir -p build
cd build
cmake .. -DBUILD_UNIT_TESTS=OFF -DBUILD_EXAMPLES=OFF
make -j4

# Copier les fichiers d'en-tête
echo "Copie des fichiers d'en-tête..."
cp -R ../include/* "$CURRENT_DIR/libs/socket.io-client/include/"

# Copier les bibliothèques compilées
echo "Copie des bibliothèques compilées..."

# Copier la bibliothèque sioclient dans le dossier spécifique à l'architecture
cp libsioclient.a "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM/"
cp libsioclient_tls.a "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM/" 2>/dev/null || true

if [[ "$OSTYPE" == "darwin"* ]]; then
    # Copier aussi dans le dossier générique osx pour la compatibilité
    cp libsioclient.a "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM_GENERIC/"
    cp libsioclient_tls.a "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM_GENERIC/" 2>/dev/null || true
    
    # Copier les bibliothèques Boost nécessaires
    BOOST_LIBS=("libboost_system-mt.a" "libboost_date_time-mt.a" "libboost_random-mt.a")
    
    # Vérifier les emplacements possibles de Boost
    BOOST_PATHS=("/usr/local/lib" "/opt/homebrew/lib" "/opt/local/lib")
    
    for lib in "${BOOST_LIBS[@]}"; do
        for path in "${BOOST_PATHS[@]}"; do
            if [ -f "$path/$lib" ]; then
                echo "Copie de $lib depuis $path"
                cp "$path/$lib" "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM/"
                cp "$path/$lib" "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM_GENERIC/"
                break
            fi
        done
    done
    
    # Si on n'a pas trouvé les bibliothèques, compiler Boost manuellement
    # (On n'implémente pas cette partie pour l'instant)
    
    echo "Bibliothèques compilées pour $PLATFORM ($(uname -m))"
elif [[ "$PLATFORM" == linux* ]]; then
    # Copier les bibliothèques pour Linux
    
    # Copier les bibliothèques Boost nécessaires si elles existent
    BOOST_LIBS=("libboost_system.a" "libboost_date_time.a" "libboost_random.a")
    BOOST_MT_LIBS=("libboost_system-mt.a" "libboost_date_time-mt.a" "libboost_random-mt.a")
    
    # Vérifier les emplacements possibles de Boost
    BOOST_PATHS=("/usr/lib" "/usr/local/lib" "/usr/lib/x86_64-linux-gnu")
    
    for lib in "${BOOST_LIBS[@]}" "${BOOST_MT_LIBS[@]}"; do
        for path in "${BOOST_PATHS[@]}"; do
            if [ -f "$path/$lib" ]; then
                echo "Copie de $lib depuis $path"
                cp "$path/$lib" "$CURRENT_DIR/libs/socket.io-client/lib/$PLATFORM/"
                break
            fi
        done
    done
    
    echo "Bibliothèques compilées pour $PLATFORM ($(uname -m))"
fi

# Nettoyer
echo "Nettoyage..."
cd "$CURRENT_DIR"
rm -rf "$TMP_DIR"

echo "Installation terminée!"
echo "Les bibliothèques Socket.IO Client C++ ont été installées dans $CURRENT_DIR/libs/socket.io-client/"
echo "Vous pouvez maintenant utiliser ofxSocketIO dans votre projet OpenFrameworks."