#include "src/Levels/SplashScreen.h"
#include "src/Levels/HelloWorldScene.h"
#include "src/Utilities/GlobalVariables.h"

USING_NS_CC;

Scene* SplashScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce( schedule_selector(SplashScreen::goToMainMenuScene),  DISPLAY_TIME_SPLASH_SCREEN);

	cocos2d::Label * label = cocos2d::Label::createWithTTF(
			"Loading (just kidding it already loaded but I am testing)","fonts/Marker Felt.ttf", 64);
	label->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	this->addChild(label);

    return true;
}

void SplashScreen::goToMainMenuScene(float dt){
	auto scene = HelloWorld::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

/*
void SplashScreen::GoToMainMenuScene( float dt )
{
    auto scene = MainMenuScene::createScene();

    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}
*/
