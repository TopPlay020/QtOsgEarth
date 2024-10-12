#pragma once

#include <osgEarth/Layer>

class LayerEventListener {
public:
    virtual ~LayerEventListener() = default;

    virtual void onLayerAdd(osgEarth::Layer* layer) = 0;
    virtual void onLayerUpdate(osgEarth::Layer* layer) = 0;
    virtual void onLayerRemove(osgEarth::Layer* layer) = 0;
};