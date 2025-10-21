#include "ofxSocketIOModern.h"

// Ajout du using namespace pour éviter les conflits avec l'ancien ofxSocketIO
// Si vous souhaitez utiliser la nouvelle version, utilisez cette inclusion 
// et ce namespace
namespace socketio_modern {
    // Exportation des classes principales
    using ofxSocketIO = ::ofxSocketIO;
    using ofxSocketIOData = ::ofxSocketIOData;
}