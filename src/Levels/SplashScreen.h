#ifndef __SPLASH_SCREEN__
#define __SPLASH_SCREEN__

#include "cocos2d.h"

class SplashScreen : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScreen);
	
	private:
		void goToMainMenuScene (float dt);
};

#endif // __SPLASH_SCENE_H__

