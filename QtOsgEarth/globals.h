#pragma once

#include <MainWindow.h>
#include <MenuManager.h>
#include <osgEarthManager.h>
#include <GlobalEventProvider.h>
#include <MediaManager.h>
#include <OsgTreeViewManager.h>
#include <OsgEarthTaskExecutor.h>
#include <OsgSqlLiteManager.h>



extern MainWindow*			g_mainWindow;
extern MenuManager*			g_menuManager;
extern OsgEarthManager*		g_osgEarthManager;
extern GlobalEventProvider* g_globalEventProvider;
extern MediaManager*		g_mediaManager;
extern OsgTreeViewManager*  g_osgTreeViewManager;
extern OsgEarthTaskExecutor* g_osgEarthTaskExecutor;
extern OsgSqlLiteManager* g_osgSqlLiteManager;
