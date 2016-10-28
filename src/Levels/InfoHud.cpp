#include "src/Levels/InfoHud.h"

bool InfoHud::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	int margin = 10;

	goldLabel = cocos2d::Label::createWithTTF("0","fonts/Marker Felt.ttf",64);
	goldLabel->setPosition(visibleSize.width / 2 - (goldLabel->getBoundingBox().size.width / 2) - margin,
			goldLabel->getBoundingBox().size.height / 2 + margin);
	this->addChild(goldLabel);
	goldLabel->setTextColor(cocos2d::Color4B::YELLOW);

	timerLabel = cocos2d::Label::createWithTTF("90", "fonts/Marker Felt.ttf", 38.0f);
	timerLabel->setTextColor(cocos2d::Color4B::WHITE);

	timerLabel->setPosition(visibleSize.width / 2 - (timerLabel->getDimensions().width / 2) - margin,
			visibleSize.height - timerLabel->getBoundingBox().size.height - margin);
	this->addChild(timerLabel);

	this->scheduleUpdate();

	return true;
}

void InfoHud::updateGoldUI() {
	goldLabel->setString(cocos2d::__String::createWithFormat("%d", gold)->getCString());
	CCLOG("gold: %d.", gold);
}

void InfoHud::update(float dt) {
	timer -= dt;

	if(timerInt != (int)timer){
		timerInt = (int)timer;
		timerLabel->setString(cocos2d::__String::createWithFormat("%.0f", timer)->getCString());
	}
}
