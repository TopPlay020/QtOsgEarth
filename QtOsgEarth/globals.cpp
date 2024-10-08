#include "globals.h"

MainWindow* g_mainWindow = nullptr;
MenuManager* g_menuManager = nullptr;
OsgEarthManager* g_osgEarthManager = nullptr;
ContextMenuManager* g_contextMenuManager = nullptr;
GlobalEventProvider* g_globalEventProvider = new GlobalEventProvider();
MediaManager* g_mediaManager = new MediaManager();