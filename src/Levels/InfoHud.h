#ifndef __INFO_HUD_SCENE_H__
#define __INFO_HUD_SCENE_H__

#include "cocos2d.h"
#include "src/Levels/HelloWorldScene.h"
#include "ui/CocosGUI.h"

class HelloWorld;

class InfoHud : public cocos2d::Layer
{
public:
	void addGold(int n) { gold += n; updateGoldUI(); };
    void spendGold(int n) { if(gold >= n) { gold -= n; } updateGoldUI(); };
	virtual bool init();
	HelloWorld * Level1;
	CREATE_FUNC(InfoHud);

private:
    int gold = 0;
	void updateGoldUI();
	cocos2d::LabelTTF *goldLabel;
};

#endif // __INFO_HUD_SCENE_H__
