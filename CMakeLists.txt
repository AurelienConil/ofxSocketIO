cmake_minimum_required(VERSION 3.12)
project(ofxSocketIO)

set(CMAKE_CXX_STANDARD 11)

# Déterminer la plateforme
# Récupérer l'architecture
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(APPLE)
    if(ARCH MATCHES "arm64")
        set(PLATFORM "osx_arm64")
    else()
        set(PLATFORM "osx_x86_64")
    endif()
    
    # Vérifier si les bibliothèques spécifiques à l'architecture existent, sinon utiliser le dossier générique osx
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libsioclient.a" AND
       EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/osx/libsioclient.a")
        set(PLATFORM "osx")
    endif()
elseif(UNIX AND NOT APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "linux64")
    else()
        set(PLATFORM "linux")
    endif()
    
    # Vérifier si nous sommes sur ARM
    if(ARCH MATCHES "armv6")
        set(PLATFORM "linuxarmv6l")
    elseif(ARCH MATCHES "armv7")
        set(PLATFORM "linuxarmv7l")
    elseif(ARCH MATCHES "aarch64")
        set(PLATFORM "linuxaarch64")
    endif()
endif()

message(STATUS "Detected platform: ${PLATFORM}")

# Options
option(USE_PRECOMPILED_LIBS "Use precompiled libraries instead of building from source" ON)
option(BUILD_UNIT_TESTS "Build Socket.IO C++ client unit tests" OFF)
option(BUILD_EXAMPLES "Build Socket.IO C++ client examples" OFF)

# Sources de ofxSocketIO
set(OFXSOCKETIO_SOURCES
    src/ofxSocketIO.cpp
    src/ofxSocketIOData.cpp
)

# Créer la bibliothèque ofxSocketIO
add_library(${PROJECT_NAME} STATIC ${OFXSOCKETIO_SOURCES})

# Ajouter les chemins d'inclusion
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/include
)

# Utiliser les bibliothèques précompilées ou compiler depuis les sources
if(USE_PRECOMPILED_LIBS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libsioclient.a")
    message(STATUS "Using precompiled Socket.IO Client C++ libraries")
    
    # Trouver les bibliothèques précompilées
    set(SIOCLIENT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libsioclient.a")
    set(BOOST_SYSTEM_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libboost_system-mt.a")
    set(BOOST_DATE_TIME_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libboost_date_time-mt.a")
    set(BOOST_RANDOM_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client/lib/${PLATFORM}/libboost_random-mt.a")
    
    # Lier avec les bibliothèques précompilées
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ${SIOCLIENT_LIB}
            ${BOOST_SYSTEM_LIB}
            ${BOOST_DATE_TIME_LIB}
            ${BOOST_RANDOM_LIB}
    )
else()
    message(STATUS "Building Socket.IO Client C++ from source")
    
    # Ajouter Socket.IO Client C++ comme sous-module
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/socket.io-client-cpp)
    
    # Lier avec la bibliothèque compilée
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            sioclient
    )
endif()

# Définir les options de compilation
target_compile_options(${PROJECT_NAME} 
    PRIVATE 
        -Wall 
        -std=c++11
)

# Optionnellement construire l'exemple
option(BUILD_EXAMPLE "Build the example application" OFF)
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()