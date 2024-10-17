#pragma once
class OsgEarthTaskExecutor : public UiUpdateAble , public GlobalEventListener
{
public:
    OsgEarthTaskExecutor();
    void addLabelTask();
    void addLayerTask();

    void onMouseClick() override;

    void updateUi() override;

private:
    OsgLocationLabel* locationLabel;
    bool isAddLabelTaskRunning = false;
};

