#include "src/Levels/HelloWorldHud.h"

bool HelloWorldHud::init() {
	if (!Layer::init()) {
		return false;
	}
	//auto visibleSize = Director::getInstance()->getVisibleSize();
	/*label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 38.0f, Size(50, 50),
			TextHAlignment::RIGHT);
	label->setColor(Color3B(255, 255, 0));
	int margin = 10;
	label->setPosition(
			visibleSize.width / 2 - (label->getDimensions().width / 2) - margin,
			label->getDimensions().height / 2 + margin);
	this->addChild(label);*/

	return true;
}

void HelloWorldHud::setNinja(){
	cocos2d::ui::Button * button = cocos2d::ui::Button::create("buttons/Button_Normal.png", "buttons/Button_Press.png", "buttons/Button_Disable.png");
	button->setTitleText("Button Text");
	button->setScale(4);

	CCLOG("HERERERERERERE");

	CCLOG("WHAST IS THIS THIS?");
	CCLOG("WHAST IS THIS THIS %p", this);
	CCLOG("WHAST IS THIS THIS %p", button);
	//CCLOG("WHAST IS THIS THIS %p", Level1);

	button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				CCLOG("Shots fired");
				this->Level1->ninja->shootFireBall();
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				CCLOG("button clicked");
				break;
			default:
				break;
		}
	});


	this->addChild(button, 10000);
}

void HelloWorldHud::numCollectedChanged(int numCollected) {
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
}
