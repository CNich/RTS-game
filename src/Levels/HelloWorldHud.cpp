#include "src/Levels/HelloWorldHud.h"

bool HelloWorldHud::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	label = cocos2d::LabelTTF::create("0", "fonts/Marker Felt.ttf", 38.0f, cocos2d::Size(50, 50),
			cocos2d::TextHAlignment::RIGHT);
	label->setColor(cocos2d::Color3B(255, 255, 0));
	int margin = 10;
	label->setPosition(
			visibleSize.width / 2 - (label->getDimensions().width / 2) - margin,
			label->getDimensions().height / 2 + margin);
	this->addChild(label);

	return true;
}

void HelloWorldHud::setNinja(){
	this->removeUI();
	cocos2d::ui::Button * button = cocos2d::ui::Button::create("buttons/Button_Normal.png", "buttons/Button_Press.png", "buttons/Button_Disable.png");
	cocos2d::ui::Button * buttonRed = cocos2d::ui::Button::create("buttons/Button_Normal_Red.png", "buttons/Button_Press.png", "buttons/Button_Disable.png");
	cocos2d::ui::Button * buttonGreen = cocos2d::ui::Button::create("buttons/Button_Normal_Green.png", "buttons/Button_Press.png", "buttons/Button_Disable.png");
	cocos2d::ui::Button * buttonBrigand = cocos2d::ui::Button::create("buttons/buy_Brigand_128x128_normal.png", "buttons/buy_Brigand_128x128_pressed.png", "buttons/Button_Disable.png");
	button->setTitleText("Button Text");
	button->setScale(4);
	buttonRed->setScale(4);
	buttonGreen->setScale(4);

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

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

	buttonRed->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				this->Level1->createUnitGroup(0);
				break;
			default:
				break;
		}
	});

	buttonGreen->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				this->Level1->createUnitGroup(1);
				break;
			default:
				break;
		}
	});

	buttonBrigand->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				this->Level1->createUnitGroup(3);
				break;
			default:
				break;
		}
	});

	this->addChild(button, 10000);
	this->addChild(buttonRed, 10000);
	this->addChild(buttonGreen, 10000);
	this->addChild(buttonBrigand, 10000);

	button->setPosition({button->getContentSize().width * button->getScale() / 2.0f, button->getContentSize().height * button->getScale() / 2.0f});
	buttonRed->setPosition({button->getContentSize().width * buttonRed->getScale() / 2.0f, buttonRed->getContentSize().height * buttonRed->getScale() * 1.5});
	buttonGreen->setPosition({button->getContentSize().width * buttonGreen->getScale() / 2.0f, buttonGreen->getContentSize().height * buttonGreen->getScale() * 2.5});
	buttonBrigand->setPosition({button->getContentSize().width * buttonGreen->getScale() / 2.0f, buttonGreen->getContentSize().height * buttonGreen->getScale() * 3.5});
}

void HelloWorldHud::setBasicUnit(){
	this->removeUI();
	cocos2d::ui::Button * button = cocos2d::ui::Button::create("buttons/Button_Normal_Aqua.png", "buttons/Button_Normal.png", "buttons/Button_Disable.png");
	button->setTitleText("Button Text");
	button->setScale(4);

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				this->Level1->_infoHud->addGold(1000);
				break;
			default:
				break;
		}
	});

	this->addChild(button, 10000);

	button->setPosition({button->getContentSize().width * button->getScale() / 2.0f, button->getContentSize().height * button->getScale() / 2.0f});
}

void HelloWorldHud::removeUI(){
	this->removeAllChildren();
}



void HelloWorldHud::numCollectedChanged(int numCollected) {
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
}
