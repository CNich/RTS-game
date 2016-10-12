#include "src/Levels/InfoHud.h"

bool InfoHud::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	goldLabel = cocos2d::LabelTTF::create("0", "fonts/Marker Felt.ttf", 38.0f, cocos2d::Size(50, 50),
			cocos2d::TextHAlignment::RIGHT);
	goldLabel->setColor(cocos2d::Color3B(255, 255, 0));
	int margin = 10;
	goldLabel->setPosition(
			visibleSize.width / 2 - (goldLabel->getDimensions().width / 2) - margin,
			goldLabel->getDimensions().height / 2 + margin);
	this->addChild(goldLabel);

	return true;
}

void InfoHud::updateGoldUI() {
	char showStr[20];
	sprintf(showStr, "%d", gold);
	goldLabel->setString(showStr);
}
