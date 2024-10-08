#include"globals.h"

class MouseEventHandler : public osgGA::GUIEventHandler {
public:
    MouseEventHandler(OsgEarthManager* osgEarthManager) : osgEarthManager(osgEarthManager) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override {
        if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE) { // Mouse move event
            // Call the method to update coordinates
            osgEarthManager->onUpdateMouseCoordinates(ea.getX(), ea.getY());
        }
        return false;
    }

private:
    OsgEarthManager* osgEarthManager; // Reference to OsgEarthManager
};
