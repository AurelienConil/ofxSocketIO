cmake_minimum_required(VERSION 3.12)
project(ofxSocketIO_example)

set(CMAKE_CXX_STANDARD 11)

# Trouver OpenFrameworks
find_package(OpenFrameworks REQUIRED)

# Répertoire racine de l'addon
set(OFXSOCKETIO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Déterminer la plateforme
if(APPLE)
    set(PLATFORM "osx")
elseif(UNIX AND NOT APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "linux64")
    else()
        set(PLATFORM "linux")
    endif()
    
    # Vérifier si nous sommes sur ARM
    execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(ARCH MATCHES "arm")
        if(ARCH MATCHES "armv6")
            set(PLATFORM "linuxarmv6l")
        elseif(ARCH MATCHES "armv7")
            set(PLATFORM "linuxarmv7l")
        endif()
    endif()
endif()

message(STATUS "Detected platform: ${PLATFORM}")

# Options
option(USE_PRECOMPILED_LIBS "Use precompiled libraries instead of building from source" ON)

# Sources de l'application exemple
set(APP_SOURCES
    src/main.cpp
    src/ofApp.cpp
)

# Sources de ofxSocketIO
set(OFXSOCKETIO_SOURCES
    ${OFXSOCKETIO_ROOT}/src/ofxSocketIO.cpp
    ${OFXSOCKETIO_ROOT}/src/ofxSocketIOData.cpp
)

# Créer l'application exemple
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${OFXSOCKETIO_SOURCES})

# Ajouter les dépendances et les chemins d'inclusion
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${OFXSOCKETIO_ROOT}/src
        ${OFXSOCKETIO_ROOT}/libs/socket.io-client/include
        ${OF_INCLUDE_DIRS}
)

# Utiliser les bibliothèques précompilées ou compiler depuis les sources
if(USE_PRECOMPILED_LIBS AND EXISTS "${OFXSOCKETIO_ROOT}/libs/socket.io-client/lib/${PLATFORM}/libsioclient.a")
    message(STATUS "Using precompiled Socket.IO Client C++ libraries")
    
    # Trouver les bibliothèques précompilées
    set(SIOCLIENT_LIB "${OFXSOCKETIO_ROOT}/libs/socket.io-client/lib/${PLATFORM}/libsioclient.a")
    set(BOOST_SYSTEM_LIB "${OFXSOCKETIO_ROOT}/libs/socket.io-client/lib/${PLATFORM}/libboost_system-mt.a")
    set(BOOST_DATE_TIME_LIB "${OFXSOCKETIO_ROOT}/libs/socket.io-client/lib/${PLATFORM}/libboost_date_time-mt.a")
    set(BOOST_RANDOM_LIB "${OFXSOCKETIO_ROOT}/libs/socket.io-client/lib/${PLATFORM}/libboost_random-mt.a")
    
    # Lier avec les bibliothèques précompilées
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${SIOCLIENT_LIB}
            ${BOOST_SYSTEM_LIB}
            ${BOOST_DATE_TIME_LIB}
            ${BOOST_RANDOM_LIB}
            ${OF_LIBRARIES}
    )
else()
    message(STATUS "Building Socket.IO Client C++ from source")
    
    # Options de Socket.IO Client C++
    option(BUILD_UNIT_TESTS "Build Socket.IO C++ client unit tests" OFF)
    option(BUILD_EXAMPLES "Build Socket.IO C++ client examples" OFF)
    
    # Ajouter Socket.IO Client C++ comme sous-module
    add_subdirectory(${OFXSOCKETIO_ROOT}/libs/socket.io-client-cpp)
    
    # Lier avec la bibliothèque compilée
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            sioclient
            ${OF_LIBRARIES}
    )
endif()

# Définir les options de compilation
target_compile_options(${PROJECT_NAME} 
    PRIVATE 
        -Wall 
        -std=c++11
)

# Copier les assets si nécessaire
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/data
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)