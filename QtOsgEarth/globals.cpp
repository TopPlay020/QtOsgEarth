#include "globals.h"

MainWindow* g_mainWindow = nullptr;
MenuManager* g_menuManager = nullptr;
OsgEarthManager* g_osgEarthManager = nullptr;
GlobalEventProvider* g_globalEventProvider = new GlobalEventProvider();
MediaManager* g_mediaManager = new MediaManager();
OsgTreeViewManager* g_osgTreeViewManager = nullptr;
OsgEarthTaskExecutor* g_osgEarthTaskExecutor = nullptr;