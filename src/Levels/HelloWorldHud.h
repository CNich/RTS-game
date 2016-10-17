#ifndef __HELLOWORLD_HUD_SCENE_H__
#define __HELLOWORLD_HUD_SCENE_H__

#include "cocos2d.h"
#include "src/Levels/HelloWorldScene.h"
#include "ui/CocosGUI.h"

//PathFinder<int> * pf;

class HelloWorld;

class HelloWorldHud : public cocos2d::Layer
{
public:
    void numCollectedChanged(int numCollected);
    virtual bool init();
    void setNinja();
    void createUnits();
    void setBasicUnit();
    void removeUI();
    CREATE_FUNC(HelloWorldHud);

    cocos2d::ui::Button * hudbutton;
    cocos2d::LabelTTF *label;
    HelloWorld * Level1;

    void setNumUI(char n){ numUI = n; };

private:
    char numUI = 0;
};

#endif // __HELLOWORLD_HUD_SCENE_H__
