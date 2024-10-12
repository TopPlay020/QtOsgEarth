#pragma once
#pragma once

#include <QList>
#include <osgEarth/Layer>
#include "LayerEventListener.h"

class LayerEventProvider {
    QList<LayerEventListener*> listeners;

public:
    void addLayerEventListener(LayerEventListener* listener) {
        listeners.append(listener);
    }

    void removeLayerEventListener(LayerEventListener* listener) {
        listeners.removeAll(listener);
    }

    void notifyLayerAdd(osgEarth::Layer* layer) {
        for (auto* listener : listeners) {
            listener->onLayerAdd(layer);
        }
    }
    
    void notifyLayerUpdate(osgEarth::Layer* layer) {
        for (auto* listener : listeners) {
            listener->onLayerUpdate(layer);
        }
    }

    void notifyLayerRemove(osgEarth::Layer* layer) {
        for (auto* listener : listeners) {
            listener->onLayerRemove(layer);
        }
    }
};
