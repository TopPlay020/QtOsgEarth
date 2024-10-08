#ifndef UIUPDATEABLE_H
#define UIUPDATEABLE_H

class UiUpdateAble {
public:
    virtual ~UiUpdateAble() = default;  // Virtual destructor for proper cleanup
    virtual void updateUi() = 0;        // Method to update UI, to be implemented by subclasses
};

#endif // UIUPDATEABLE_H
