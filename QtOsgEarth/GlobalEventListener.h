#pragma once


class GlobalEventListener {
public:
    virtual void onMouseMove() = 0;
    virtual void onContextMenu() = 0;
    virtual void onMouseLeftRelese() = 0;
};

